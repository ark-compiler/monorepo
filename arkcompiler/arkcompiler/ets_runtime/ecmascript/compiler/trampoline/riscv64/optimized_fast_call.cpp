#include "ecmascript/compiler/trampoline/riscv64/common_call.h"

#include "ecmascript/compiler/assembler/assembler.h"
#include "ecmascript/compiler/argument_accessor.h"
#include "ecmascript/compiler/common_stubs.h"
#include "ecmascript/compiler/rt_call_signature.h"
#include "ecmascript/deoptimizer/deoptimizer.h"
#include "ecmascript/ecma_runtime_call_info.h"
#include "ecmascript/frames.h"
#include "ecmascript/js_function.h"
#include "ecmascript/method.h"
#include "ecmascript/js_thread.h"
#include "ecmascript/message_string.h"
#include "ecmascript/runtime_call_id.h"

namespace panda::ecmascript::riscv64 
{
using Label = panda::ecmascript::Label;
#define __ assembler->

void OptimizedFastCall::OptimizedFastCallEntry(ExtendedAssembler *assembler)
{
    __ BindAssemblerStub(RTSTUB_ID(OptimizedFastCallEntry));
    Register glueReg(A0);
    Register argc(A1);
    Register argV(A2);
    Register prevFpReg(A3);
    Register sp(SP);

    OptimizedCall::PushJSFunctionEntryFrame (assembler, prevFpReg);

    __ Addi(Register(A3), argc, Immediate(0)); //mv a3, argc
    __ Addi(Register(A4), argV, Immediate(0)); 
    Register tmpArgc(A3);
    Register tmpArgV(A4);

    __ Addi(Register(T0), glueReg, Immediate(0));
    __ Ld(Register(A1), tmpArgV, 0);
    __ Ld(Register(A2), tmpArgV, FRAME_SLOT_SIZE);
    __ Addi(tmpArgV, tmpArgV, Immediate(DOUBLE_SLOT_SIZE));

    __ CallAssemblerStub(RTSTUB_ID(JSFastCallWithArgV), false);
    __ Addi(Register(A2), Register(T0), Immediate(0));
    OptimizedCall::PopJSFunctionEntryFrame(assembler, Register(A2));
    __ Jalr(ZERO, RA, 0); // TODO can change with Ret() Pseudo Operations
}

void OptimizedFastCall::OptimizedFastCallAndPushUndefined(ExtendedAssembler *assembler)
{
    __ BindAssemblerStub(RTSTUB_ID(OptimizedFastCallAndPushUndefined));
    Register glue(A0);
    Register actualNumArgs(A1);
    Register jsfunc(A2);
    Register codeAddr(A3);
    Register sp(SP);
    Register currentSp = T2;
    Register op = T2;

    Label call; 
    Label arg4;
    Label arg5;
    Label arg6;
    Label argc;
    Label checkExpectedArgs; 

    // construct frame
    OptimizedCall::PushOptimizedArgsConfigFrame(assembler);

    __ Addi(Register(T4), Register(A1), Immediate(0));
    __ Addi(Register(T5), sp, Immediate(4 * FRAME_SLOT_SIZE));
    Register actualNumArgsReg = T4;
    Register argV = T5; 

    Register method = T2;
    Register expectedNumArgs = T3; 
    __ Ld(method, jsfunc, JSFunction::METHOD_OFFSET); 
    __ Ld(expectedNumArgs, method, Method::CALL_FIELD_OFFSET);
    Register tmpi = T6;
    __ Addi(tmpi, Register(ZERO), MethodLiteral::NumArgsBits::START_BIT);
    __ Srl(expectedNumArgs, expectedNumArgs, tmpi); //  >> START_BIT:32 bit
    __ Andi(expectedNumArgs, expectedNumArgs, Immediate((1LU <<  MethodLiteral::NumArgsBits::SIZE) - 1)); 
    __ Addi(expectedNumArgs, expectedNumArgs, Immediate(NUM_MANDATORY_JSFUNC_ARGS)); 

    Label arg7;
    Label arg8;
    __ Addi(Register(A1), Register(A2), Immediate(0)); // func move to argc(A1 <= jsfunc)
    __ Addi(Register(A2), Register(A4), Immediate(0)); // this move to func(A2 <= this)
    jsfunc = Register(A1); 

    __ Addi(tmpi, Register(ZERO), Immediate(3));
    __ Bne(actualNumArgsReg, tmpi, __ LinkAndGetInstOffsetToLabel(&arg4)); 
    __ Addi(Register(A3), Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED)); 
    __ Addi(Register(A4), Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
    __ Addi(Register(A5), Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
    __ Addi(Register(A6), Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
    __ Addi(Register(A7), Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
    __ Jal(Register(ZERO), __ LinkAndGetInstOffsetToLabel(&checkExpectedArgs)); 

    __ Bind(&arg4); 
    {
        __ Addi(Register(A3), Register(A5), Immediate(0));
        __ Addi(Register(T6), Register(T6), Immediate(1));
        __ Bne(actualNumArgsReg, tmpi, __ LinkAndGetInstOffsetToLabel(&arg5));
        __ Addi(Register(A4), Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Addi(Register(A5), Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Addi(Register(A6), Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Addi(Register(A7), Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Jal(Register(ZERO), __ LinkAndGetInstOffsetToLabel(&checkExpectedArgs));
    }

    __ Bind(&arg5); 
    {
        __ Addi(Register(A4), Register(A6), Immediate(0));
        __ Addi(Register(T6), Register(T6), Immediate(1));
        __ Bne(actualNumArgsReg, tmpi, __ LinkAndGetInstOffsetToLabel(&arg6));
        __ Addi(Register(A5), Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Addi(Register(A6), Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Addi(Register(A7), Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Jal(Register(ZERO), __ LinkAndGetInstOffsetToLabel(&checkExpectedArgs));
    }

    __ Bind(&arg6); 
    {
        __ Addi(Register(A4), Register(A6), Immediate(0));
        __ Addi(Register(T6), Register(T6), Immediate(1));
        __ Bne(actualNumArgsReg, tmpi, __ LinkAndGetInstOffsetToLabel(&arg7));
        __ Addi(Register(A6), Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Addi(Register(A7), Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Jal(Register(ZERO), __ LinkAndGetInstOffsetToLabel(&checkExpectedArgs));
    }

    __ Bind(&arg7); 
    {
        __ Ld(op, argV, 0); 
        __ Addi(Register(A6), op, Immediate(0)); 
        __ Addi(argV, argV, Immediate(FRAME_SLOT_SIZE)); 
        __ Addi(Register(T6), Register(T6), Immediate(1)); 
        __ Bne(actualNumArgsReg, tmpi, __ LinkAndGetInstOffsetToLabel(&arg8));
        __ Addi(Register(A7), Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Jal(Register(ZERO), __ LinkAndGetInstOffsetToLabel(&checkExpectedArgs));
    }

    __ Bind(&arg8); 
    {
        __ Ld(op, argV, 0); 
        __ Addi(Register(X7), op, Immediate(0)); 
        __ Addi(argV, argV, Immediate(FRAME_SLOT_SIZE));
        __ Addi(Register(T6), Register(T6), Immediate(1));
        __ Bne(actualNumArgsReg, tmpi, __ LinkAndGetInstOffsetToLabel(&argc));
        __ Jal(Register(ZERO), __ LinkAndGetInstOffsetToLabel(&checkExpectedArgs));
    }

    __ Bind(&argc); 
    {
        __ Addi(expectedNumArgs, expectedNumArgs, Immediate(-8)); // 8 : register save 8 arg
        __ Addi(actualNumArgsReg, actualNumArgsReg, Immediate(-8)); // 8 : register save 8 arg
        OptimizedCall::IncreaseStackForArguments(assembler, expectedNumArgs, currentSp); 
        TempRegister1Scope scope1(assembler); 
        TempRegister2Scope scope2(assembler);
        Register tmp = __ TempRegister1(); 
        Register undefinedValue = __ TempRegister2();
        __ Sub(tmp, expectedNumArgs, actualNumArgsReg); 
        PushUndefinedWithArgc(assembler, glue, tmp, undefinedValue, currentSp, nullptr, nullptr); 
        PushArgsWithArgv(assembler, glue, actualNumArgsReg, argV, undefinedValue, currentSp, nullptr, nullptr); 
        __ Jal(Register(ZERO), __ LinkAndGetInstOffsetToLabel(&call));
    }

    __ Bind(&checkExpectedArgs);
    {
        __ Bge(tmpi, expectedNumArgs, __ LinkAndGetInstOffsetToLabel(&call)); // 8>=expected
        __ Addi(expectedNumArgs, expectedNumArgs, Immediate(-8)); // 8 : register save 8 arg
        OptimizedCall::IncreaseStackForArguments(assembler, expectedNumArgs, currentSp); 
        TempRegister2Scope scope2(assembler);
        Register undefinedValue = __ TempRegister2();
        PushUndefinedWithArgc(assembler, glue, expectedNumArgs, undefinedValue, currentSp, nullptr, nullptr); 
        __ Jal(Register(ZERO), __ LinkAndGetInstOffsetToLabel(&call));
    }
    __ Bind(&call);
    TempRegister1Scope scope1(assembler);
    Register method1 = __ TempRegister1();
    __ Ld(method1, jsfunc, JSFunction::METHOD_OFFSET); 
    __ Ld(Register(T6), method1, Method::CODE_ENTRY_OFFSET); 
    // Jump to code entry and store the return address +4(different from arm64) 
    // in the ra, i.e. call jsfunc
    __ Jalr(Register(RA), Register(T6), Immediate(0)); 

    __ Addi(Register(SP), Register(FP),  Immediate(0));
    __ RestoreFpAndRa();
    __ Jalr(ZERO, RA, 0); // TODO can change with Ret() Pseudo Operations
}


// * uint64_t JSFastCallWithArgV(uintptr_t glue, uint32_t argc, JSTaggedType calltarget,
//                                JSTaggedType this, argV)
// * cc calling convention call js function()
// * arguments:
//              %a0 - glue
//              %a1 - call-target
//              %a2 - this
//              %a3 - artual argc
//              %a4 - argv
void OptimizedFastCall::JSFastCallWithArgV(ExtendedAssembler *assembler)
{
    __ BindAssemblerStub(RTSTUB_ID(JSFastCallWithArgV));
    Register sp(SP);
    Register zero(X0);
    Register glue(A0);
    Register actualNumArgs(A3);
    Register jsfunc(A1);
    Register thisObj(A2);
    Register currentSp = __ AvailableRegister1();
    Register callsiteSp = __ AvailableRegister2();
    Label call;
    // using add instruction in risc-v to replace mov instruction in aarch64 
    __ Add(callsiteSp, sp, zero);
    OptimizedCall::PushOptimizedUnfoldArgVFrame(assembler, callsiteSp);
    TempRegister2Scope scope2(assembler);
    Register op = __ TempRegister2();
    Register argC = __ AvailableRegister3();
    Register argV = __ AvailableRegister4();
    __ Add(argC, actualNumArgs, zero);
    __ Add(argV, Register(A4), zero);
    __ Beq(argC, zero, __ LinkAndGetInstOffsetToLabel(&call));
    __ Ld(op, argV, 0);
    __ Add(Register(A3), op, zero); // first arg
    __ Addi(argV, argV, Immediate(FRAME_SLOT_SIZE));
    __ Addi(argC, argC, Immediate(-1));

    __ Beq(argC, zero, __ LinkAndGetInstOffsetToLabel(&call));
    __ Ld(op, argV, 0);
    __ Add(Register(A4), op, zero); // second arg
    __ Addi(argV, argV, Immediate(FRAME_SLOT_SIZE));
    __ Addi(argC, argC, Immediate(-1));

    __ Beq(argC, zero, __ LinkAndGetInstOffsetToLabel(&call));
    __ Ld(op, argV, 0);
    __ Add(Register(A5), op, zero); // third arg
    __ Addi(argV, argV, Immediate(FRAME_SLOT_SIZE));
    __ Addi(argC, argC, Immediate(-1));

    __ Beq(argC, zero, __ LinkAndGetInstOffsetToLabel(&call));
    __ Ld(op, argV, 0);
    __ Add(Register(A6), op, zero);
    __ Addi(argV, argV, Immediate(FRAME_SLOT_SIZE));
    // __ Subi(argC, argC, Immediate(1));
    __ Addi(argC, argC, Immediate(-1));

    __ Beq(argC, zero, __ LinkAndGetInstOffsetToLabel(&call));
    __ Ld(op, argV, 0);
    __ Add(Register(A7), op, zero);
    __ Addi(argV, argV, Immediate(FRAME_SLOT_SIZE));
    __ Addi(argC, argC, Immediate(-1));

    __ Beq(argC, zero, __ LinkAndGetInstOffsetToLabel(&call));
    OptimizedCall::IncreaseStackForArguments(assembler, argC, currentSp);

    CommonCall::PushArgsWithArgv(assembler, glue, argC, argV, op, currentSp, nullptr, nullptr);

    __ Bind(&call);
    TempRegister1Scope scope1(assembler);
    Register method = __ TempRegister1();

    // __ Ld(method, MemoryOperand(jsfunc, JSFunction::METHOD_OFFSET));
    __ Ld(method, jsfunc, JSFunction::METHOD_OFFSET);

    // __ Ld(T1, MemoryOperand(method, Method::CODE_ENTRY_OFFSET));
    __ Ld(T1, method, Method::CODE_ENTRY_OFFSET);
    __ Jalr(X0, T1, Immediate(0));

    __ Add(Register(SP), Register(FP), zero);
    __ RestoreFpAndRa();
    __ Ret();
}

// * Arguments:
//        %a0 - glue
//        %a1 - func
//        %a2 - this
//        %a3 - actualNumArgs
//        %a4 -  argv
//        %a5 -  expectedNumArgs
void OptimizedFastCall::JSFastCallWithArgVAndPushUndefined(ExtendedAssembler *assembler)
{
    __ BindAssemblerStub(RTSTUB_ID(JSFastCallWithArgVAndPushUndefined));
    Register sp(SP);
    Register zero(X0);
    Register glue(A0);
    Register jsfunc(A1);
    Register thisObj(A2);
    Register counter(T0);
    Register currentSp = __ AvailableRegister1();
    Register op = __ AvailableRegister1();
    Register callsiteSp = __ AvailableRegister2();
    Label call;
    Label arg1;
    Label arg2;
    Label arg3;
    Label arg4;
    Label arg5;
    Label argc;
    Label checkExpectedArgs;
    __ Add(callsiteSp, sp, zero);
    OptimizedCall::PushOptimizedUnfoldArgVFrame(assembler, callsiteSp);
    Register actualNumArgsReg = __ AvailableRegister3();
    // actualNumArgsReg a3
    Register argV = __ AvailableRegister4();
    Register expectedNumArgs = __ AvailableRegister2();
    __ Add(actualNumArgsReg, Register(A3), zero);
    //        %a3 - actualNumArgs
    __ Add(argV, Register(A4), zero);
    //        %a4 -  argv
    __ Add(expectedNumArgs, Register(A5), zero);
    //        %a5 -  expectedNumArgs

    __ Bne(actualNumArgsReg, zero, __ LinkAndGetInstOffsetToLabel(&arg1));
    __ Addi(counter, zero, Immediate(1));
    __ Addi(Register(A3), zero, Immediate(JSTaggedValue::VALUE_UNDEFINED));
    __ Addi(Register(A4), zero, Immediate(JSTaggedValue::VALUE_UNDEFINED));
    __ Addi(Register(A5), zero, Immediate(JSTaggedValue::VALUE_UNDEFINED));
    __ Addi(Register(A6), zero, Immediate(JSTaggedValue::VALUE_UNDEFINED));
    __ Addi(Register(A7), zero, Immediate(JSTaggedValue::VALUE_UNDEFINED));
    __ Jal(zero, __ LinkAndGetInstOffsetToLabel(&checkExpectedArgs));

    __ Bind(&arg1);
    {
        // __ Ld(op, MemoryOperand(argV, 0));
        __ Ld(op, argV, 0);
        __ Add(Register(A3), op, zero);
        __ Addi(argV, argV, Immediate(FRAME_SLOT_SIZE));
        __ Bne(actualNumArgsReg, counter, __ LinkAndGetInstOffsetToLabel(&arg2));
        __ Addi(counter, counter ,Immediate(1));
        __ Addi(Register(A4), zero, Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Addi(Register(A5), zero, Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Addi(Register(A6), zero, Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Addi(Register(A7), zero, Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Jal(zero, __ LinkAndGetInstOffsetToLabel(&checkExpectedArgs));
    }

    __ Bind(&arg2);
    {
        // __ Ld(op, MemoryOperand(argV, 0));
        __ Ld(op, argV, 0);
        __ Add(Register(A4), op, zero);
        __ Addi(argV, argV, Immediate(FRAME_SLOT_SIZE));
        __ Bne(actualNumArgsReg, counter, __ LinkAndGetInstOffsetToLabel(&arg3));
        __ Addi(counter, counter ,Immediate(1));
        __ Addi(Register(A5), zero, Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Addi(Register(A6), zero, Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Addi(Register(A7), zero, Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Jal(zero, __ LinkAndGetInstOffsetToLabel(&checkExpectedArgs));
    }

    __ Bind(&arg3);
    {
        // __ Ld(op, MemoryOperand(argV, 0));
        __ Ld(op, argV, 0);
        __ Add(Register(A5), op, zero);
        __ Addi(argV, argV, Immediate(FRAME_SLOT_SIZE));
        __ Bne(actualNumArgsReg, counter, __ LinkAndGetInstOffsetToLabel(&arg4));
        __ Addi(counter, counter ,Immediate(1));
        __ Addi(Register(A6), zero, Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Addi(Register(A7), zero, Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Jal(zero, __ LinkAndGetInstOffsetToLabel(&checkExpectedArgs));
    }

    __ Bind(&arg4);
    {
        // __ Ld(op, MemoryOperand(argV, 0));
        __ Ld(op, argV, 0);
        __ Add(Register(A6), op, zero);
        __ Addi(argV, argV, Immediate(FRAME_SLOT_SIZE));
        __ Bne(actualNumArgsReg, counter, __ LinkAndGetInstOffsetToLabel(&arg5));
        __ Addi(counter, counter ,Immediate(1));
        __ Addi(Register(A7), zero, Immediate(JSTaggedValue::VALUE_UNDEFINED));
        __ Jal(zero, __ LinkAndGetInstOffsetToLabel(&checkExpectedArgs));
    }

    __ Bind(&arg5);
    {
        // __ Ld(op, MemoryOperand(argV, 0));
        __ Ld(op, argV, 0);
        __ Add(Register(A7), op, zero);
        __ Addi(argV, argV, Immediate(FRAME_SLOT_SIZE));
        __ Bne(actualNumArgsReg, counter, __ LinkAndGetInstOffsetToLabel(&argc));
        __ Jal(zero, __ LinkAndGetInstOffsetToLabel(&checkExpectedArgs));
    }

    __ Bind(&argc);
    {
        __ Addi(expectedNumArgs, expectedNumArgs, Immediate(-5)); // 5 : register save 5 arg
        __ Addi(actualNumArgsReg, actualNumArgsReg, Immediate(-5)); // 5 : register save 5 arg
        OptimizedCall::IncreaseStackForArguments(assembler, expectedNumArgs, currentSp);
        TempRegister1Scope scope1(assembler);
        TempRegister2Scope scope2(assembler);
        Register tmp = __ TempRegister1();
        Register undefinedValue = __ TempRegister2();
        __ Sub(tmp, expectedNumArgs, actualNumArgsReg);
        CommonCall::PushUndefinedWithArgc(assembler, glue, tmp, undefinedValue, currentSp, nullptr, nullptr);
        CommonCall::PushArgsWithArgv(assembler, glue, actualNumArgsReg, argV, undefinedValue, currentSp, nullptr, nullptr);
        __ Jal(zero, __ LinkAndGetInstOffsetToLabel(&call));
    }

    __ Bind(&checkExpectedArgs);
    {
        __ Addi(counter, zero ,Immediate(5));
        __ Blt(expectedNumArgs, counter, __ LinkAndGetInstOffsetToLabel(&call));

        __ Addi(expectedNumArgs, expectedNumArgs, Immediate(-5)); // 5 : register save 5 arg
        OptimizedCall::IncreaseStackForArguments(assembler, expectedNumArgs, currentSp);
        TempRegister2Scope scope2(assembler);
        Register undefinedValue = __ TempRegister2();
        CommonCall::PushUndefinedWithArgc(assembler, glue, expectedNumArgs, undefinedValue, currentSp, nullptr, nullptr);
        __ Jal(zero, __ LinkAndGetInstOffsetToLabel(&call));
    }

    __ Bind(&call);
    TempRegister1Scope scope1(assembler);
    Register method = __ TempRegister1();
    // __ Ld(method, MemoryOperand(A1, JSFunction::METHOD_OFFSET));
    __ Ld(method, A1, JSFunction::METHOD_OFFSET);
    // __ Ld(T1, MemoryOperand(method, Method::CODE_ENTRY_OFFSET));
    __ Ld(T1, method, Method::CODE_ENTRY_OFFSET);
    __ Jalr(X0, T1, Immediate(0));

    __ Add(Register(SP), Register(FP), zero);
    __ RestoreFpAndRa();
    __ Ret();
}


#undef __

}  //end panda::ecmascript::riscv64