#include "ecmascript/compiler/assembler/assembler.h"
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
#include "ecmascript/mem/dyn_chunk.h"
namespace panda::ecmascript::riscv64 
{
using Label = panda::ecmascript::Label;

void OptimizedCall::CallRuntime(ExtendedAssembler* assembler)
{	
    Register glue(T0);
    Register fp(FP);
    Register tmp(S1);
    Register sp(SP);
    Register argC(A1);
    Register argV(A2);

    assembler-> BindAssemblerStub(RTSTUB_ID(CallRuntime));
    // equals to PushFpAndLr in aarch64
    assembler-> Addi(sp, sp, Immediate(-4 * FRAME_SLOT_SIZE));
    assembler-> Sd(S0, sp, Immediate(-FRAME_SLOT_SIZE));
    assembler-> Sd(RA, sp, Immediate(-FRAME_SLOT_SIZE * 2));

    Register frameType(A2);
    assembler-> Addi(frameType, ZERO, Immediate(static_cast<int64_t>(FrameType::LEAVE_FRAME)));
    assembler-> Sd(tmp, sp, Immediate(-FRAME_SLOT_SIZE));
    assembler-> Sd(frameType, sp, Immediate(-FRAME_SLOT_SIZE * 2));
    assembler-> Addi(fp, sp, Immediate(2 * FRAME_SLOT_SIZE));
    assembler-> Sd(fp, glue, Immediate(JSThread::GlueData::GetLeaveFrameOffset(false)));

    Register rtfunc(S1);
    assembler-> Ld(tmp, fp, Immediate(GetStackArgOffSetToFp(0)));
    assembler-> Xor(tmp, tmp, tmp);
    assembler-> Addi(tmp, tmp, Immediate(3));
    assembler-> Sll(tmp, tmp, tmp);
    assembler-> Xor(tmp, tmp, tmp);
    assembler-> Add(tmp, glue, tmp);
    assembler-> Ld(rtfunc, tmp, Immediate(JSThread::GlueData::GetRTStubEntriesOffset(false)));
    assembler-> Ld(argC, fp, Immediate(GetStackArgOffSetToFp(1)));  // 1: the second arg id
    assembler-> Add(argV, fp, Immediate(GetStackArgOffSetToFp(2)));  // 2: the third arg id
    assembler-> Jalr(ZERO, rtfunc, Immediate(0));

    // callee restore
    // 0 : 0 restore size
    assembler-> Ld(tmp, sp, Immediate(0));

    // descontruct frame
    // 2 2 means stack frame slot size
    assembler-> Addi(sp, sp, Immediate(2 * FRAME_SLOT_SIZE));
    // equals to RestoreFpAndLr in aarch64
    assembler-> Ld(RA, sp, Immediate(-FRAME_SLOT_SIZE));
    assembler-> Ld(S0, sp, Immediate(-FRAME_SLOT_SIZE * 2));
    assembler-> Addi(sp, sp, Immediate(2 * FRAME_SLOT_SIZE));

    assembler-> Ret(); // Pseudo Instruction
}
void OptimizedCall::IncreaseStackForArguments(ExtendedAssembler *assembler, Register argc, Register currentSp)
{
    Register sp(SP);
    assembler-> Ld(currentSp, sp, Immediate(0));
    // currentSp = sp
    // add extra aguments, numArgs
    assembler-> Addi(argc, argc, Immediate(static_cast<int64_t>(CommonArgIdx::ACTUAL_ARGC)));
    //argc = argc + 1
    assembler-> Sub(currentSp, currentSp, Operand(argc, UXTW, FRAME_SLOT_SIZE_LOG2)); 
    //currentSp = currentSp - argc * 8

    Label aligned;
    assembler-> Andi(T1, currentSp, LogicalImmediate::Create(0xf, RegDSize));
    assembler-> Beq(T1, ZERO, assembler->LinkAndGetInstOffsetToLabel(&aligned));
    assembler-> Addi(currentSp, currentSp, Immediate(-FRAME_SLOT_SIZE));
    assembler-> Bind(&aligned);

    assembler-> Ld(sp, currentSp, Immediate(0));
    // sp = currentSp;
    assembler-> Add(currentSp, currentSp, Operand(argc, UXTW, FRAME_SLOT_SIZE_LOG2));
    // currentSp = currentSp + argc * 8;
}
void OptimizedCall::CallRuntimeWithArgv(ExtendedAssembler* assembler)
{
    assembler-> BindAssemblerStub(RTSTUB_ID(CallRuntimeWithArgv));
    Register glue(A0);
    Register runtimeId(A1);
    
    Register argv(A3);
    Register sp(SP);
    // 2 : 2 means pair
    assembler-> Addi(sp, sp, Immediate(-2*FRAME_SLOT_SIZE));
    assembler-> Sd(sp, argv, Immediate(FRAME_SLOT_SIZE));
    assembler-> Sd(sp, argv, Immediate(0));

    assembler-> Addi(sp, sp, Immediate(-2*FRAME_SLOT_SIZE));
    assembler-> Sd(sp, RA, Immediate(FRAME_SLOT_SIZE));
    assembler-> Sd(sp, runtimeId, Immediate(0));
    
    Register fp(FP);
    // construct leave frame
    Register frameType(T0);
    assembler-> Addi(frameType, ZERO, Immediate(static_cast<int64_t>(FrameType::LEAVE_FRAME_WITH_ARGV)));
    
    assembler-> Addi(sp, sp, Immediate(-2*FRAME_SLOT_SIZE));
    assembler-> Sd(sp, frameType, Immediate(FRAME_SLOT_SIZE));
    assembler-> Sd(sp, Register(FP), Immediate(0));
    
    assembler-> Addi(Register(FP), sp, Immediate(FRAME_SLOT_SIZE));
    
    // here the offset generation method is reserved
    assembler-> Sd(glue, fp, Immediate(JSThread::GlueData::GetLeaveFrameOffset(false)));

     // load runtime trampoline address
    Register tmp(T0);
    Register rtfunc(T0);

    // 3 : 3 means 2 << 3 = 8
    assembler-> Xor(tmp,tmp,tmp);
    assembler-> Addi(tmp,tmp,Immediate(3));
    assembler-> Sll(runtimeId,runtimeId,tmp);
    assembler-> Xor(tmp,tmp,tmp);
    assembler-> Add(tmp, glue,runtimeId);
    
    assembler-> Ld(tmp, rtfunc, Immediate(JSThread::GlueData::GetRTStubEntriesOffset(false)));
    
    assembler-> Addi(A1, argv, Immediate(0));
    assembler-> Addi(A2, argv, Immediate(0));
    
    assembler-> Jalr(RA, rtfunc, Immediate(0));

    assembler-> Ld(sp, ZERO, Immediate(FRAME_SLOT_SIZE));
    assembler-> Ld(sp, FP, Immediate(0));
    assembler-> Addi(sp, sp, Immediate(2*FRAME_SLOT_SIZE));

    assembler-> Ld(sp, RA, Immediate(FRAME_SLOT_SIZE));
    assembler-> Ld(sp, ZERO, Immediate(0));
    assembler-> Addi(sp, sp, Immediate(2*FRAME_SLOT_SIZE));
        
    assembler-> Addi(sp, sp, Immediate(2 * FRAME_SLOT_SIZE)); // 2 : 2 means pair
    
    assembler-> Jalr(ZERO, RA, Immediate(0));
}

void OptimizedCall::PushMandatoryJSArgs(ExtendedAssembler *assembler, Register jsfunc,
                                        Register thisObj, Register newTarget, Register currentSp)
{
    assembler-> Addi(currentSp, currentSp, Immediate(-FRAME_SLOT_SIZE));
    assembler-> Sd(currentSp, thisObj, Immediate(0));

    assembler-> Addi(currentSp, currentSp, Immediate(-FRAME_SLOT_SIZE));
    assembler-> Sd(currentSp, newTarget, Immediate(0));

    assembler-> Addi(currentSp, currentSp, Immediate(-FRAME_SLOT_SIZE));
    assembler-> Sd(currentSp, jsfunc, Immediate(0));
}

void OptimizedCall::GenJSCallWithArgV(ExtendedAssembler *assembler, bool needAddExpectedArgs)
{
    Register sp(SP);
    Register glue(A0);
    Register actualNumArgs(A1);
    Register jsfunc(A2);
    Register newTarget(A3);
    Register thisObj(A4);
    Register argV(A5);
    Register currentSp = assembler-> AvailableRegister1();
    Register callsiteSp = assembler-> AvailableRegister2();
    Label pushCallThis;

    assembler-> Addi(callsiteSp, sp, Immediate(0));
    PushOptimizedUnfoldArgVFrame(assembler, callsiteSp);
    Register argC(T2);
    assembler-> Addi(actualNumArgs, actualNumArgs, Immediate(NUM_MANDATORY_JSFUNC_ARGS));
    assembler-> Addi(argC, actualNumArgs, Immediate(0));
    IncreaseStackForArguments(assembler, argC, currentSp);
    {
        TempRegister1Scope scope1(assembler);
        TempRegister2Scope scope2(assembler);
        Register tmp = assembler-> TempRegister1();
        Register op = assembler-> TempRegister2();
        assembler-> Addi(tmp, actualNumArgs, Immediate(-NUM_MANDATORY_JSFUNC_ARGS));
        PushArgsWithArgv(assembler, glue, tmp, argV, op, currentSp, &pushCallThis, nullptr);
    }
    assembler-> Bind(&pushCallThis);
    PushMandatoryJSArgs(assembler, jsfunc, thisObj, newTarget, currentSp);

    assembler-> Addi(currentSp, currentSp, Immediate(-FRAME_SLOT_SIZE));
    assembler-> Sd(currentSp, actualNumArgs, Immediate(0));
    
    if (needAddExpectedArgs) {
        assembler-> CallAssemblerStub(RTSTUB_ID(OptimizedCallAndPushUndefined), false);
    } else {
        assembler-> CallAssemblerStub(RTSTUB_ID(CallOptimized), false);
    }

    assembler-> Ld(sp, actualNumArgs, Immediate(0));
    
    PopJSFunctionArgs(assembler, actualNumArgs, actualNumArgs);
    PopOptimizedUnfoldArgVFrame(assembler);
    
    assembler-> Jalr(ZERO, RA, Immediate(0));
}


void OptimizedCall::JSFunctionEntry(ExtendedAssembler* assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(JSFunctionEntry));
    Register glueReg(A0);
    Register argV(A2);
    Register prevFpReg(A3);
    Register needPushUndefined(A4);
    Register sp(SP);
    Register tmpArgV(A7);
    Register tempReg(T3);

    Label lJSCallWithArgVAndPushUndefined;
    Label lPopFrame;

    PushJSFunctionEntryFrame(assembler, prevFpReg);

    assembler-> Addi(Register(A6), needPushUndefined, Immediate(0));
    assembler-> Addi(tmpArgV, argV, Immediate(0));
    assembler-> Addi(Register(X20), glueReg, Immediate(0));

    assembler-> Ld(Register(A2), tmpArgV, Immediate(0));
    assembler-> Ld(Register(A3), tmpArgV, Immediate(FRAME_SLOT_SIZE));
    assembler-> Ld(Register(A4), tmpArgV, Immediate(2 * FRAME_SLOT_SIZE));

    assembler-> Addi(tmpArgV, tmpArgV, Immediate(3 * FRAME_SLOT_SIZE));
    assembler-> Addi(Register(A5), tmpArgV, Immediate(0));

    assembler-> Addi(tempReg, Register(ZERO), Immediate(1));

    assembler-> Beq(Register(A6), tempReg, assembler->LinkAndGetInstOffsetToLabel(&lJSCallWithArgVAndPushUndefined));

    JSCallWithArgV(assembler);
    assembler-> Jal(Register(X1), assembler->LinkAndGetInstOffsetToLabel(&lPopFrame));

    assembler-> Bind(&lJSCallWithArgVAndPushUndefined);
    JSCallWithArgVAndPushUndefined(assembler);

    assembler-> Bind(&lPopFrame);
    assembler-> Addi(Register(A2), Register(X20), Immediate(0));

    PopJSFunctionEntryFrame(assembler, Register(A2));
    assembler-> Ret();

}



void OptimizedCall::JSProxyCallInternalWithArgV(ExtendedAssembler* assembler)
{
   assembler->BindAssemblerStub(RTSTUB_ID(JSProxyCallInternalWithArgV));
    Register jsfunc(A1);
    Register sp(SP);
    assembler-> Sd(sp, jsfunc, Immediate(FRAME_SLOT_SIZE));
    JSCallInternal(assembler, jsfunc);
}
// * uint64_t OptimizedCallAndPushUndefined(uintptr_t glue, uint32_t argc, JSTaggedType calltarget, JSTaggedType new,
//                   JSTaggedType this, arg[0], arg[1], arg[2], ..., arg[N-1])
// * webkit_jscc calling convention call js function()
//
// * OptimizedJSFunctionFrame layout description as the following:
//               +--------------------------+
//               |        arg[N-1]          |
//               +--------------------------+
//               |       ...                |
//               +--------------------------+
//               |       arg[1]             |
//               +--------------------------+
//               |       arg[0]             |
//               +--------------------------+
//               |       this               |
//               +--------------------------+
//               |       new-target         |
//               +--------------------------+
//               |       call-target        |
//               |--------------------------|
//               |       argc               |
//               |--------------------------| ---------------
//               |       returnAddr         |               ^
//      sp ----> |--------------------------|               |
//               |       callsiteFp         |               |
//               |--------------------------|   OptimizedJSFunctionFrame
//               |       frameType          |               |
//               |--------------------------|               |
//               |       call-target        |               v
//               +--------------------------+ ---------------
void OptimizedCall::OptimizedCallAndPushUndefined(ExtendedAssembler* assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(OptimizedCallAndPushUndefined));
    Register sp(SP);
    Register jsfunc(T1); 			
    Register method(T0); 			
    Register expectedNumArgs(A1); 	
    Register actualNumArgs(A2);		
    Register codeAddr(A3);			
    Register argV(A4);				


    assembler->Ld(jsfunc, sp, Immediate(FRAME_SLOT_SIZE));
    assembler->Ld(method, jsfunc, Immediate(JSFunction::METHOD_OFFSET));
    assembler->Ld(codeAddr, method, Immediate(Method::CODE_ENTRY_OFFSET));
    assembler->Ld(expectedNumArgs, method, Immediate(Method::CALL_FIELD_OFFSET));

    assembler->Addi(T6,ZERO,MethodLiteral::NumArgsBits::START_BIT);
    assembler->Srl(expectedNumArgs, expectedNumArgs,T6);

    assembler->Andi(expectedNumArgs, expectedNumArgs,
        LogicalImmediate::Create(
            MethodLiteral::NumArgsBits::Mask() >> MethodLiteral::NumArgsBits::START_BIT, RegDSize)); // aarch64 RegXSize

    assembler->Addi(expectedNumArgs, expectedNumArgs, Immediate(NUM_MANDATORY_JSFUNC_ARGS));
    assembler->Addi(argV, sp, Immediate(kungfu::ArgumentAccessor::GetExtraArgsNum() * FRAME_SLOT_SIZE));  // skip numArgs
    assembler->Ld(actualNumArgs, sp, Immediate(0));

    Register glue(A0);			
    Register currentSp(A5);		

    Label copyArguments;
    Label invokeCompiledJSFunction;

    // construct frame
    PushOptimizedArgsConfigFrame(assembler);
    Register argC(A7);			

    // replace Cmov
    Label test,end;
    assembler->Blt(expectedNumArgs, actualNumArgs, assembler->LinkAndGetInstOffsetToLabel(&test)); // if expectedNumArgs > actualNumArgs  argc = expectedNumArgs
    assembler->Addi(argC, actualNumArgs,Immediate(0));
    assembler->Jal(ZERO,assembler->LinkAndGetInstOffsetToLabel(&end)); 
    assembler->Bind(&test);
    assembler->Addi(argC, expectedNumArgs,Immediate(0));
    assembler->Bind(&end);


    IncreaseStackForArguments(assembler, argC, currentSp);
    {
        TempRegister1Scope scope1(assembler);
        TempRegister2Scope scope2(assembler);
        Register tmp = assembler->TempRegister1();
        Register undefinedValue = assembler->TempRegister2();
        assembler->Blt(expectedNumArgs, actualNumArgs, assembler->LinkAndGetInstOffsetToLabel(&copyArguments));
        PushUndefinedWithArgc(assembler, glue, tmp, undefinedValue, currentSp, nullptr, nullptr);
    }
    assembler->Bind(&copyArguments);
    assembler->Beq(actualNumArgs,ZERO, assembler->LinkAndGetInstOffsetToLabel(&invokeCompiledJSFunction));
    {
        TempRegister1Scope scope1(assembler);
        TempRegister2Scope scope2(assembler);
        Register argc = assembler->TempRegister1();
        Register argValue = assembler->TempRegister2();
        assembler->Addi(argc, actualNumArgs,Immediate(0));
        PushArgsWithArgv(assembler, glue, argc, argV, argValue, currentSp, &invokeCompiledJSFunction, nullptr);
    }
    assembler->Bind(&invokeCompiledJSFunction);
    {
        assembler->Addi(Register(T3), expectedNumArgs,Immediate(0));
        assembler->Sd(actualNumArgs, sp,Immediate(0));
        assembler->Jalr(Register(X0),codeAddr,Immediate(0));
    }

    // pop argV argC
    // 3 : 3 means argC * 8
    assembler->Ld(actualNumArgs, sp, Immediate(0));
    PopJSFunctionArgs(assembler, Register(T3), actualNumArgs);
    // pop prevLeaveFrameFp to restore thread->currentFrame_
    PopOptimizedArgsConfigFrame(assembler);
    assembler->Ret();
}
void OptimizedCall::OptimizedCallAsmInterpreter(ExtendedAssembler *assembler)
{
    Label target;
    PushAsmInterpBridgeFrame(assembler);
    assembler->Jalr(Register(X1),Register(ZERO), assembler->LinkAndGetInstOffsetToLabel(&target)); // jalr x1, rs, 0  jump and link
    PopAsmInterpBridgeFrame(assembler);
    assembler->Ret();
    assembler->Bind(&target);
    {
        AsmInterpreterCall::JSCallCommonEntry(assembler, JSCallMode::CALL_FROM_AOT);
    }
    }



void OptimizedCall::PushJSFunctionEntryFrame(ExtendedAssembler *assembler, Register prevFp)
{
    Register fp(FP);
    Register sp(SP);
    TempRegister2Scope temp2Scope(assembler);
    assembler->PushFpAndRa();
    Register frameType = assembler->TempRegister2();
    // construct frame
    assembler->Addi(frameType, Register(ZERO), Immediate(static_cast<int64_t>(FrameType::OPTIMIZED_ENTRY_FRAME)));
    // 2 : 2 means pairs
    assembler->Addi(sp, sp, Immediate(-FRAME_SLOT_SIZE * 2));
    assembler->Sd(sp, prevFp, Immediate(0));
    // 8: 8 means 8 bytes, the size of a register. TODO: Not Sure
    assembler->Sd(sp, frameType, Immediate(8));

    assembler->Addi(fp, sp, Immediate(DOUBLE_SLOT_SIZE));
    assembler->CalleeSave();
}

void OptimizedCall::PopJSFunctionEntryFrame(ExtendedAssembler *assembler, Register glue)
{
    Register fp(FP);
    Register sp(SP);
    //prevfp cannot use X1, because RA = X1. 
    //here use X11.
    Register prevFp(X11);
    assembler->CalleeRestore();

    // 2: prevFp and frameType
    assembler->Ld(prevFp, sp, Immediate(0));
    // TODO: aarch64 set Zero = SP, but in riscv, we use Zero register.
    assembler->Ld(Register(ZERO), sp, Immediate(8));
    assembler->Addi(sp, sp, Immediate(FRAME_SLOT_SIZE * 2));
    // restore return address
    assembler->RestoreFpAndRa();
    // TODO: not sure the following offset is right, 
    assembler->Sd(glue, prevFp, Immediate(JSThread::GlueData::GetLeaveFrameOffset(false)));
}
void OptimizedCall::PushOptimizedArgsConfigFrame(ExtendedAssembler *assembler)
{
    Register sp(SP);
    TempRegister2Scope temp2Scope(assembler);
    Register frameType = assembler->TempRegister2();
    assembler->PushFpAndRa();
    // construct frame
    assembler->Addi(frameType, Register(ZERO), Immediate(static_cast<int64_t>(FrameType::OPTIMIZED_JS_FUNCTION_ARGS_CONFIG_FRAME)));
    // 2 : 2 means pairs. X19 means calleesave and 16bytes align
    assembler->Addi(sp, sp, Immediate(-FRAME_SLOT_SIZE * 2));
    // TODO: S1 means calleesave's register, replace the aarch64's register X19
    assembler->Sd(sp, Register(S1), Immediate(0));
    // 8: 8 means 8 bytes, the size of a register. TODO: Not Sure
    assembler->Sd(sp, frameType, Immediate(8));

    assembler->Addi(Register(FP), sp, Immediate(DOUBLE_SLOT_SIZE));
}
void OptimizedCall::PopOptimizedArgsConfigFrame(ExtendedAssembler *assembler)
{
    TempRegister2Scope temp2Scope(assembler);
    Register sp(SP);
    Register frameType = assembler->TempRegister2();
    // 2 : 2 means pop call site sp and type
    assembler->Ld(Register(S1), sp, Immediate(0));
    assembler->Ld(frameType, sp, Immediate(8));
    assembler->Addi(sp, sp, Immediate(FRAME_SLOT_SIZE * 2));
    assembler->RestoreFpAndRa();
}
void OptimizedCall::PushAsmBridgeFrame(ExtendedAssembler *assembler)
{
    Register sp(SP);
    TempRegister2Scope temp2Scope(assembler);
    Register frameType = assembler->TempRegister2();
    assembler->PushFpAndRa();
    // construct frame
    assembler->Addi(frameType, Register(ZERO), Immediate(static_cast<int64_t>(FrameType::ASM_BRIDGE_FRAME)));

    // 2 : 2 means pairs. means calleesave and 16bytes align
    assembler->Addi(sp, sp, Immediate(-FRAME_SLOT_SIZE * 2));
    // TODO: S1 means calleesave's register, replace the aarch64's register X19
    assembler->Sd(sp, Register(S1), Immediate(0));
    // 8: 8 means 8 bytes, the size of a register. TODO: Not Sure
    assembler->Sd(sp, frameType, Immediate(8));

    assembler->Addi(Register(FP), sp, Immediate(DOUBLE_SLOT_SIZE));
}
void OptimizedCall::PopOptimizedFrame(ExtendedAssembler *assembler)
{
    TempRegister2Scope temp2Scope(assembler);
    Register sp(SP);
    Register frameType = assembler->TempRegister2();
    // 2 : 2 means pop call site sp and type
    assembler->Ld(Register(S1), sp, Immediate(0));
    assembler->Ld(frameType, sp, Immediate(8));
    assembler->Addi(sp, sp, Immediate(FRAME_SLOT_SIZE * 2));
    assembler->RestoreFpAndRa();
}

// * uint64_t PushOptimizedUnfoldArgVFrame(uintptr_t glue, uint32_t argc, JSTaggedType calltarget,
//                                         JSTaggedType new, JSTaggedType this, JSTaggedType argV[])
// * cc calling convention call js function()
// * arguments:
//              %x0 - glue
//              %x1 - argc
//              %x2 - call-target
//              %x3 - new-target
//              %x4 - this
//              %x5 - argv
//
// * OptimizedUnfoldArgVFrame layout description as the following:
//      sp ----> |--------------------------| ---------------
//               |       returnAddr         |               ^
//  currentFp--> |--------------------------|               |
//               |       prevFp             |               |
//               |--------------------------|   OptimizedUnfoldArgVFrame
//               |       frameType          |               |
//               |--------------------------|               |
//               |       currentFp          |               v
//               +--------------------------+ ---------------

void OptimizedCall::PushOptimizedUnfoldArgVFrame(ExtendedAssembler *assembler, Register callSiteSp)
{
    Register sp(SP);
    TempRegister2Scope temp2Scope(assembler);
    Register frameType = assembler->TempRegister2();
    assembler->PushFpAndRa();
    // construct frame
    assembler->Addi(frameType, Register(ZERO), Immediate(static_cast<int64_t>(FrameType::OPTIMIZED_JS_FUNCTION_UNFOLD_ARGV_FRAME)));
    
    // 2 : 2 means pairs
    assembler->Addi(sp, sp, Immediate(-FRAME_SLOT_SIZE * 2));
    // TODO: S1 means calleesave's register, replace the aarch64's register X19
    assembler->Sd(sp, callSiteSp, Immediate(0));
    // 8: 8 means 8 bytes, the size of a register. TODO: Not Sure
    assembler->Sd(sp, frameType, Immediate(8));

    assembler->Addi(Register(FP), sp, Immediate(DOUBLE_SLOT_SIZE));
}

void OptimizedCall::PopOptimizedUnfoldArgVFrame(ExtendedAssembler *assembler)
{
    Register sp(SP);
    // 2 : 2 means pop call site sp and type
    assembler->Addi(sp, sp, Immediate(2 * FRAME_SLOT_SIZE));
    assembler->RestoreFpAndRa();
}


void OptimizedCall::JSBoundFunctionCallInternal(ExtendedAssembler *assembler, Register glue,
                                            Register actualArgC, Register jsfunc, int stubId)
{
    // Save the original parameter configuration
    PushOptimizedArgsConfigFrame(assembler);
    // basefp is the X29 register on x86, used to save the base address pointer
    Register basefp(X29);
    // fp is an available temporary register used to save the stack frame pointer
    Register fp(FP);

    // argV is the X5 register on x86, used to save the address of the first parameter
    Register argV(T0);
    // Add the offset to the base address pointer to get the address of the first parameter
    assembler->Add(argV, basefp, fp); // 0: first index id
    // Load the actual number of parameters from the address of the first parameter
    assembler->Ld(actualArgC, argV, 0);

    // boundLength is the X2 register on x86, used to save the number of bound parameters
    Register boundLength(SP);
    // realArgC is the X7 register on x86, used to save the total number of actual parameters and bound parameters
    Register realArgC(T2);
    // Define some labels for jumping
    Label copyBoundArgument;
    Label pushCallTarget;
    Label popArgs;
    Label slowCall;
    Label aotCall;
    Label notClass;
    // Get binding parameters
    assembler->Ld(boundLength, jsfunc, JSBoundFunction::BOUND_ARGUMENTS_OFFSET);
    // Get the number of bound parameters
    assembler->Ld(boundLength, boundLength, TaggedArray::LENGTH_OFFSET);
    // Calculate the total number of actual parameters and bound parameters
    assembler->Add(realArgC, boundLength, actualArgC);
    // Save the total number to the X19 register
    assembler->Add(Register(X19), realArgC, ZERO);
    // Increase the stack space according to the total number
    IncreaseStackForArguments(assembler, realArgC, fp);
    // Calculate the actual number of parameters after excluding necessary JS function parameters
    assembler->Addi(actualArgC, actualArgC, Immediate(-NUM_MANDATORY_JSFUNC_ARGS));
    assembler->Beq(actualArgC, ZERO, stubId);
    {
        // Apply for a temporary register
        TempRegister1Scope scope1(assembler);
        // tmp is a temporary register used to save intermediate results
        Register tmp(T3);
        // Calculate the offset of the first parameter
        const int64_t argoffsetSlot = static_cast<int64_t>(CommonArgIdx::FUNC) - 1;
        // Get the address of the first actual parameter by adding the offset to the address of the first parameter
        assembler->Addi(argV, argV, Immediate((NUM_MANDATORY_JSFUNC_ARGS + argoffsetSlot) * FRAME_SLOT_SIZE));
        // Push the actual parameters onto the stack
        PushArgsWithArgv(assembler, glue, actualArgC, argV, tmp, fp, nullptr, nullptr);
    }
    // Bind and copy the label of the binding parameter
    assembler->Bind(&copyBoundArgument);
    {
        // boundArgs is the X4 register on x86, used to save the address of bound parameters
        Register boundArgs(X4);
        // Load the address of the binding parameter from jsfunc
        assembler->Ld(boundArgs, jsfunc, JSBoundFunction::BOUND_ARGUMENTS_OFFSET);
        // Get the address of the first bound parameter by adding the offset to the address of the bound parameter
        assembler->Addi(boundArgs, boundArgs, Immediate(TaggedArray::DATA_OFFSET));
        {
            // Apply for a temporary register
            TempRegister1Scope scope1(assembler);
            // tmp is a temporary register used to save intermediate results
            Register tmp(T3);
            // Push the bound parameters onto the stack
            PushArgsWithArgv(assembler, glue, boundLength, boundArgs, tmp, fp, nullptr, nullptr);
        }
    }
    // boundTarget is the X7 register on x86, used to save the address of the binding target
    Register boundTarget(X7);
    // newTarget is the X6 register on x86, used to save the address of the new target
    Register newTarget(T1);
    // Bind the label of the push call target
    assembler->Bind(&pushCallTarget);
    {
        // thisObj is the X4 register on x86, used to save the address of the bound this object
        Register thisObj(X4);
        // Load the address of the bound this object from jsfunc
        assembler->Ld(thisObj, jsfunc, JSBoundFunction::BOUND_THIS_OFFSET);
        // Set the address of the new target to undefined
        assembler->Addi(newTarget, ZERO, (JSTaggedValue::VALUE_UNDEFINED));
        // 2 : 2 means pair
        // Push the new target onto the stack together with this object
        assembler->Sd(newTarget, fp, -FRAME_SLOT_SIZE * 2);
        // Load the address of the binding target from jsfunc
        assembler->Ld(boundTarget, jsfunc, JSBoundFunction::BOUND_TARGET_OFFSET);
        // 2 : 2 means pair
        // Push the total number of parameters and the binding target onto the stack
        assembler->Sd(Register(X19), boundTarget, -FRAME_SLOT_SIZE * 2);
    }
    // Call the JSCallCheck function to check whether the binding target is valid. If it is invalid, jump to the slow call tag.
    JSCallCheck(assembler, boundTarget, Register(X9), &slowCall, &slowCall);
    // hclass is an available temporary register used to save the class information of the binding target
    Register hclass(T3);
    // Load the address of class information from the address of the binding target
    assembler->Ld(hclass, boundTarget, 0);
    // Load bit field information from the address of class information
    assembler->Ld(hclass, hclass, JSHClass::BIT_FIELD_OFFSET);
    // Check whether the IsClassConstructorOrPrototypeBit bit of the bit field information is zero. If it is zero, jump to the non-class label
    assembler->Beq(hclass, ZERO, JSHClass::IsClassConstructorOrPrototypeBit::START_BIT);
    // Check whether the ConstructorBit bit of the bit field information is one. If it is one, jump to the label of the slow call
    assembler->Bne(hclass, ZERO, JSHClass::ConstructorBit::START_BIT);
    // Bind non-class tags
    assembler->Bind(&notClass);
    // Check whether the IsOptimizedBit bit of the bit field information is zero. If it is zero, jump to the label of the slow call.
    assembler->Beq(hclass, ZERO, JSHClass::IsOptimizedBit::START_BIT);
    // Bind the label of the AOT call
    assembler->Bind(&aotCall);

    {
        // Move the total number of parameters from the X19 register to the a1 register
        assembler->Addi(Register(X11), Register(X19), 0);
        // Move the binding target from the X7 register to the a2 register
        assembler->Addi(Register(X12), boundTarget, 0);
        // Add the offset to the stack frame pointer to get the address of the first parameter, and save it to the a3 register
        assembler->Addi(Register(X13), fp, Immediate(4 * FRAME_SLOT_SIZE)); // 4: skip argc and func new this
        // Move the new target from the X6 register to the a5 register
        assembler->Addi(Register(X15), Register(T1), 0);
        // boundCallInternalId is the X9 register on x86, used to save the internal identification of the bound call
        Register boundCallInternalId(X9);
        // baseAddress is the X8 register on x86, used to save the base address
        Register baseAddress(FP);
        // codeAddress is the X10 register on x86, used to save the code address
        Register codeAddress(X10);
        // Set the base address to JSThread::GlueData::GetCOStubEntriesOffset(false)
        assembler->Addi(baseAddress, ZERO,Immediate(JSThread::GlueData::GetCOStubEntriesOffset(false)));
        // Set the identifier inside the binding call to CommonStubCSigns::JsBoundCallInternal
        assembler->Addi(boundCallInternalId, ZERO, Immediate(CommonStubCSigns::JsBoundCallInternal));
        // Get the code address from the a0 register plus the base address
        assembler->Add(codeAddress, Register(A0), baseAddress);
        // Load the code address from the code address plus the offset of the identifier inside the bind call
        assembler->Ld(codeAddress, codeAddress, FRAME_SLOT_SIZE_LOG2);
        // Jump to the function inside the binding call through the code address
        // Jump to the label of the popup parameter
        assembler->Bind(&popArgs);
    }
    // Bind the tag for slow calls
    assembler->Bind(&slowCall);
    {
        assembler->Bind(&popArgs);
    }

    // Bind the label of the pop-up parameter
    assembler->Bind(&popArgs);
    // Pop up the parameters of the JS function
    PopJSFunctionArgs(assembler, Register(X19), Register(X19));
    // Pop up the optimized parameter configuration
    PopOptimizedArgsConfigFrame(assembler);
    // return
    assembler->Ret();
}
void OptimizedCall::JSProxyCallInternal(ExtendedAssembler *assembler, Register sp, Register jsfunc)
{
    // input: a0(calltarget)
    // output: glue:a0 argc:a1 calltarget:a2 argv:a3
    assembler->Addi(Register(A2), jsfunc, 0);
    assembler->Ld(Register(A1), sp, 0);
    assembler->Addi(A3, sp, Immediate(FRAME_SLOT_SIZE)); // get argv

    Register proxyCallInternalId(A4); // Use a4 as a temporary register
    Register baseAddress(A5); // Use a5 as a temporary register
    Register codeAddress(A6); // Use a6 as a temporary register
    assembler->Addi(baseAddress, ZERO, Immediate(JSThread::GlueData::GetCOStubEntriesOffset(false)));
    assembler->Addi(proxyCallInternalId, ZERO, Immediate(CommonStubCSigns::JsProxyCallInternal));
    assembler->Add(codeAddress, A0, baseAddress);
    assembler->Ld(codeAddress, codeAddress, FRAME_SLOT_SIZE_LOG2);
    assembler->Jal(codeAddress, 0); // riscv uses the jalr instruction to jump and save the return address in the ra register
}
void OptimizedCall::ThrowNonCallableInternal(ExtendedAssembler *assembler, Register sp)
{
    // In riscv, the registers are named x0-x31, where x0 is the constant 0, x1 is the return address, x2 is the stack pointer, and x8 is the frame pointer.
    // Therefore, we need to replace the original X6 and X5 with x6 and x5 respectively, and sp and fp with x2 and x8
    Register frameType(T1);
    Register taggedMessageId(T0);
    // In riscv, the instruction to save the frame pointer and return address is sd, which stores double words instead of stp
    // The syntax of sd is sd rs2, offset(rs1), where rs1 is the base address register, rs2 is the source register, and offset is the offset
    // Therefore, we need to replace __PushFpAndLr(); with the following two instructions:
    assembler->Sd(FP, sp, Immediate(-8));
    assembler->Sd(RA, sp, Immediate(-16));
    // In riscv, the instruction to move the immediate value is li, which means loading the immediate value, not mov
    // The syntax of li is li rd, imm, where rd is the destination register and imm is the immediate number
    // Therefore, we need to replace __Mov(frameType, Immediate(static_cast<int64_t>(FrameType::OPTIMIZED_JS_FUNCTION_ARGS_CONFIG_FRAME))); with the following directive:
    assembler->Addi(frameType, ZERO, static_cast<unsigned long>(FrameType::OPTIMIZED_JS_FUNCTION_ARGS_CONFIG_FRAME));
    // Similarly, we need to replace __Mov(taggedMessageId, Immediate(JSTaggedValue(GET_MESSAGE_STRING_ID(NonCallable)).GetRawData())); with the following instructions:
    assembler->Addi(taggedMessageId, ZERO, static_cast<unsigned long>(JSTaggedValue(GET_MESSAGE_STRING_ID(NonCallable)).GetRawData()));
    // In riscv, the instruction to store double words is sd, not stp
    // The syntax of sd is sd rs2, offset(rs1), where rs1 is the base address register, rs2 is the source register, and offset is the offset
    // Therefore, we need to replace __ Stp(taggedMessageId, frameType, MemoryOperand(sp, -FRAME_SLOT_SIZE * 2, AddrMode::PREINDEX)); with the following two instructions:
    assembler->Sd(taggedMessageId, sp, Immediate(-24));
    assembler->Sd(frameType, sp, Immediate(-32));
    // In riscv, the instruction to update the stack pointer is addi, which means adding an immediate value, not add
    // The syntax of addi is addi rd, rs1, imm, where rd is the destination register, rs1 is the source register, and imm is the immediate value
    // Therefore, we need to replace __Add(Register(FP), sp, Immediate(DOUBLE_SLOT_SIZE)); with the following instructions:
    assembler->Addi(FP, sp, -32);
    // In riscv, the instruction to move the immediate value is li, which means loading the immediate value, not mov
    // The syntax of li is li rd, imm, where rd is the destination register and imm is the immediate number
    // Therefore, we need to replace __Mov(argC, Immediate(1)); with the following instruction:
    assembler->Addi(T0, ZERO, Immediate(1));
    // Similarly, we need to replace __Mov(runtimeId, Immediate(RTSTUB_ID(ThrowTypeError))); with the following instructions:
    assembler->Addi(T1, ZERO, static_cast<unsigned long>(RTSTUB_ID(ThrowTypeError)));
    // In riscv, the instruction to store double words is sd, not stp
    // The syntax of sd is sd rs2, offset(rs1), where rs1 is the base address register, rs2 is the source register, and offset is the offset
    // Therefore, we need to replace __ Stp(runtimeId, argC, MemoryOperand(sp, -FRAME_SLOT_SIZE * 2, AddrMode::PREINDEX)); with the following two instructions:
    assembler->Sd(T1, (sp), Immediate(-40));
    assembler->Sd(T0, (sp), -48);
    // In riscv, the instruction to update the stack pointer is addi, which means adding an immediate value, not add
    // The syntax of addi is addi rd, rs1, imm, where rd is the destination register, rs1 is the source register, and imm is the immediate value
    // Therefore, we need to replace __Add(sp, sp, Immediate(4 * FRAME_SLOT_SIZE)); with the following instructions:
    assembler->Addi(sp, sp, -48);
    // In riscv, the instruction to call the function is jalr, that is, jump and link the register, not call
    // The syntax of jalr is jalr rd, offset(rs1), where rd is the return address register, rs1 is the target address register, and offset is the offset.
    // Therefore, we need to replace __ CallAssemblerStub(RTSTUB_ID(CallRuntime), false); with the following instruction:
    // assembler->Jalr(RA, RTSTUB_ID(CallRuntime), 0); // Call the CallRuntime function
    // In riscv, the instruction to move the immediate value is li, which means loading the immediate value, not mov
    // The syntax of li is li rd, imm, where rd is the destination register and imm is the immediate number
    // Therefore, we need to replace __Mov(Register(X0), Immediate(JSTaggedValue::VALUE_EXCEPTION)); with the following instructions:
    assembler->Addi(ZERO, ZERO, static_cast<unsigned long>(JSTaggedValue::VALUE_EXCEPTION));
    // In riscv, the instruction to restore the frame pointer and return address is ld, that is, load a double word, not ldp
    // The syntax of ld is ld rd, offset(rs1), where rd is the destination register, rs1 is the base address register, and offset is the offset.
    // Therefore, we need to replace __RestoreFpAndLr(); with the following two instructions:
    assembler->Ld(FP, sp, -8);
    assembler->Ld(RA, sp, -16);
    // In riscv, the instruction to return the function is ret, the same as x86
    // The syntax of ret is ret, no need to specify a register
    // Therefore, we can leave __Ret(); unchanged
    assembler->Ret();
}

void OptimizedCall::JSCall(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(JSCall));
    GenJSCall(assembler, true);
}

void OptimizedCall::JSCallNew(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(JSCallNew));
    GenJSCall(assembler, true);
}

void OptimizedCall::GenJSCall(ExtendedAssembler *assembler, bool isNew)
{
  Register jsfunc(A1);
  Register sp(SP);
  assembler->Ld(jsfunc, sp, Immediate(CommonCall::FRAME_SLOT_SIZE));
  JSCallInternal(assembler, jsfunc, isNew);
}

void OptimizedCall::JSCallCheck(ExtendedAssembler *assembler, Register jsfunc, Register taggedValue, Label *nonCallable, Label *notJSFunction)
{
    TempRegister2Scope scope1(assembler);
    Register x5 = assembler->TempRegister2();
    Register x6 = assembler->TempRegister2();
    Register x7 = assembler->TempRegister2();
    Register x28 = assembler->TempRegister2();
  
    assembler->Lui(taggedValue, Immediate(0));
    assembler->Addi(taggedValue, taggedValue, Immediate(static_cast<int>(JSTaggedValue::TAG_MARK)));
    // LinkAndGetInstOffsetToLabel undefine
    assembler->Bge(jsfunc, taggedValue, assembler->LinkAndGetInstOffsetToLabel(nonCallable));
    assembler->Lui(x28, Immediate(0));
    assembler->Addi(x28, x28, Immediate(0));

    // LinkAndGetInstOffsetToLabel undefine
    assembler->Beq(jsfunc, x28, assembler->LinkAndGetInstOffsetToLabel(nonCallable));
    assembler->Lui(taggedValue, Immediate(0));
    assembler->Addi(taggedValue, taggedValue, Immediate(JSTaggedValue::TAG_SPECIAL));
    assembler->And(taggedValue, jsfunc, taggedValue);
    assembler->Lui(x28, Immediate(0));
    assembler->Addi(x28, x28, Immediate(0));

    // LinkAndGetInstOffsetToLabel undefine
    assembler->Bne(taggedValue, x28, assembler->LinkAndGetInstOffsetToLabel(nonCallable));
    Register jshclass(A2);

    assembler->Ld(jshclass, jsfunc, Immediate(JSFunction::HCLASS_OFFSET));

    Register bitfield(A2);

    assembler->Ld(bitfield, jshclass, Immediate(JSHClass::BIT_FIELD_OFFSET));
    assembler->Ld(x5, bitfield, Immediate(0));

    assembler->Lui(x6, Immediate(0));
    assembler->Addi(X6, x6, Immediate(1));
    assembler->Lui(x7, Immediate(JSHClass::CallableBit::START_BIT));
    assembler->Addi(x7, x7, Immediate(JSHClass::CallableBit::START_BIT));
    assembler->Lui(x28, Immediate(0));
    assembler->Sll(x6, x6, x7);
    assembler->And(x6, x6, x5);
    assembler->Beq(x6, x28, assembler->LinkAndGetInstOffsetToLabel(nonCallable));

    Register jstype(A3);

    assembler->Andi(jstype, bitfield, Immediate(0xff));
    assembler->Lui(x7, Immediate(0));
    assembler->Addi(x7, x7, Immediate(static_cast<int>(JSType::JS_FUNCTION_FIRST)));
    assembler->Sub(jstype, jstype, x7);
    assembler->Lui(x28, Immediate(static_cast<int>(JSType::JS_FUNCTION_LAST) - static_cast<int>(JSType::JS_FUNCTION_FIRST) + 1));
    assembler->Bge(jstype, x28, assembler->LinkAndGetInstOffsetToLabel(notJSFunction));
}

void OptimizedCall::CallBuiltinTrampoline(ExtendedAssembler *assembler)
{
    Register glue(A0);
    Register sp(SP);
    Register nativeFuncAddr(A4);
    Register taggedValue(X12);
    Register temp(A1);

    assembler->Sd(Register(X30), sp, Immediate(-CommonCall::FRAME_SLOT_SIZE * 2));
    assembler->Sd(glue, sp, Immediate(-FRAME_SLOT_SIZE * 2 + 8));

    assembler->Lui(taggedValue, Immediate(0));
    assembler->Addi(taggedValue, taggedValue, Immediate(static_cast<int32_t>(FrameType::BUILTIN_CALL_LEAVE_FRAME)));

    assembler->Sd(temp, sp, Immediate(-CommonCall::FRAME_SLOT_SIZE * 2));
    assembler->Sd(Register(X29), sp, Immediate(-CommonCall::FRAME_SLOT_SIZE * 2 + 8));

    assembler->Addi(temp, sp, Immediate(CommonCall::FRAME_SLOT_SIZE));

    assembler->Sd(temp, sp, Immediate(-CommonCall::FRAME_SLOT_SIZE * 2));

    assembler->Addi(Register(X29), temp, Immediate(0));
    assembler->Addi(Register(A0), sp, Immediate(CommonCall::TRIPLE_SLOT_SIZE));
    assembler->Blr(nativeFuncAddr);
    assembler->Addi(temp, Register(FP), Immediate(0));

    assembler->Ld(Register(X29), temp, Immediate(CommonCall::DOUBLE_SLOT_SIZE));
    assembler->Ld(Register(X30), temp, Immediate(CommonCall::DOUBLE_SLOT_SIZE + 8));

    assembler->Addi(temp, temp, Immediate(CommonCall::FRAME_SLOT_SIZE));

    assembler->Addi(sp, temp, Immediate(0));
    assembler->Ret();
}

void OptimizedCall::JSCallInternal(ExtendedAssembler *assembler, Register jsfunc, bool isNew)
{
    Register sp(SP);
    Register glue(X10);
    Register taggedValue(X12);
    Label nonCallable;
    Label notJSFunction;
    JSCallCheck(assembler, jsfunc, taggedValue, &nonCallable, &notJSFunction);

    Register method(X12);
    Register callField(X13);
    Register actualArgC(X14);
    Label callNativeMethod;
    Label lCallConstructor;
    Label lCallBuiltinStub;
    Label lCallNativeCpp;
    Label lNotClass;

    assembler->Ld(Register(X15), jsfunc, Immediate(0));
    assembler->Ld(Register(X15), Register(X15), Immediate(JSHClass::BIT_FIELD_OFFSET));
    assembler->Ld(method, jsfunc, Immediate(JSFunction::METHOD_OFFSET));
    assembler->Ld(actualArgC, sp, Immediate(0));
    assembler->Ld(callField, method, Immediate(Method::CALL_FIELD_OFFSET));
    
    TempRegister2Scope scope1(assembler);
    Register x5 = assembler->TempRegister2();
    Register x6 = assembler->TempRegister2();
    Register x7 = assembler->TempRegister2();
    Register x28 = assembler->TempRegister2();

    assembler->Ld(x5, callField, Immediate(0));
    assembler->Lui(x6, Immediate(0));
    assembler->Addi(x6, x6, Immediate(1));
    assembler->Lui(x28, Immediate(0));
    assembler->Lui(x7, Immediate(0));
    assembler->Addi(x7, x7, Immediate(MethodLiteral::IsNativeBit::START_BIT));
    assembler->Sll(x6, x6, x7);
    assembler->And(x6, x6, X5);
    assembler->Bne(x6, x28, assembler->LinkAndGetInstOffsetToLabel(&callNativeMethod));//LinkAndGetInstOffsetToLabel is stub in assembler/riscv64/assembler_riscv64.cpp

    if(!isNew){
        assembler->Ld(x5, Register(X15), Immediate(0));
        assembler->Lui(x6, Immediate(0));
        assembler->Addi(x6, x6, Immediate(1));
        assembler->Lui(x7, Immediate(0));
        assembler->Addi(x7, x7, Immediate(JSHClass::IsClassConstructorOrPrototypeBit::START_BIT));
        assembler->Lui(x28, Immediate(0));
        assembler->Sll(x6, x6, x7);
        assembler->And(x6, x6, x5);
        assembler->Beq(x6, x28, assembler->LinkAndGetInstOffsetToLabel(&lNotClass));//LinkAndGetInstOffsetToLabel is stub in assembler/riscv64/assembler_riscv64.cpp

        assembler->Ld(x5, Register(X15), Immediate(0));
        assembler->Lui(x6, Immediate(0));
        assembler->Addi(x6, x6, Immediate(1));
        assembler->Lui(x7, Immediate(0));
        assembler->Addi(x7, x7, Immediate(JSHClass::ConstructorBit::START_BIT));
        assembler->Lui(x28, Immediate(0));
        assembler->Sll(x6, x6, x7);
        assembler->And(x6, x6, x5);
        assembler->Bne(x6, x28, assembler->LinkAndGetInstOffsetToLabel(&lCallConstructor));//LinkAndGetInstOffsetToLabel is stub in assembler/riscv64/assembler_riscv64.cpp
    }

    assembler->Bind(&lNotClass);
    {
        Register argV(X15);
        assembler->Addi(argV, sp, Immediate(kungfu::ArgumentAccessor::GetExtraArgsNum() * CommonCall::FRAME_SLOT_SIZE));
        assembler->Addi(argV, argV, Immediate(kungfu::ArgumentAccessor::GetFixArgsNum() * CommonCall::FRAME_SLOT_SIZE));
        assembler->Lui(x28, Immediate(0));
        assembler->Addi(x28, x28, Immediate(kungfu::ArgumentAccessor::GetFixArgsNum()));
        assembler->Sub(actualArgC, actualArgC, x28);
        OptimizedCallAsmInterpreter(assembler);
    }

    assembler->Bind(&callNativeMethod);
    {
        Register nativeFuncAddr(X14);
        if(!isNew){
            assembler->Ld(x5, callField, Immediate(0));
            assembler->Lui(x6, Immediate(0));
            assembler->Addi(x6, x6, Immediate(1));
            assembler->Lui(x7, Immediate(MethodLiteral::IsFastBuiltinBit::START_BIT));
            assembler->Addi(x7, x7, Immediate(MethodLiteral::IsFastBuiltinBit::START_BIT));
            assembler->Lui(x28, Immediate(0));
            assembler->Sll(x6, x6, x7);
            assembler->And(x6, x6, x5);
            assembler->Beq(x6, x28, assembler->LinkAndGetInstOffsetToLabel(&lCallNativeCpp));//LinkAndGetInstOffsetToLabel is stub in assembler/riscv64/assembler_riscv64.cpp

            assembler->Lui(x28, Immediate(kungfu::ArgumentAccessor::GetFixArgsNum() + 3));
            assembler->Blt(actualArgC, x28, assembler->LinkAndGetInstOffsetToLabel(&lCallBuiltinStub));//LinkAndGetInstOffsetToLabel is stub in assembler/riscv64/assembler_riscv64.cpp
        } else {
            assembler->Ld(x5, callField, Immediate(0));
            assembler->Lui(x6, Immediate(0));
            assembler->Addi(x6, x6, Immediate(1));
            assembler->Lui(x7, Immediate(0));
            assembler->Addi(x7, x7, Immediate(MethodLiteral::IsFastBuiltinBit::START_BIT));
            assembler->Lui(x28, Immediate(0));
            assembler->Sll(x6, x6, x7);
            assembler->And(x6, x6, x5);
            assembler->Bne(x6, x28, assembler->LinkAndGetInstOffsetToLabel(&lCallBuiltinStub));//LinkAndGetInstOffsetToLabel is stub in assembler/riscv64/assembler_riscv64.cpp
        }
        assembler->Bind(&lCallNativeCpp);
        assembler->Ld(nativeFuncAddr, method, Immediate(Method::NATIVE_POINTER_OR_BYTECODE_ARRAY_OFFSET));
        CallBuiltinTrampoline(assembler);
    }

    assembler->Bind(&lCallBuiltinStub);
    {
        Register builtinStub = assembler->TempRegister2();
        assembler->Ld(Register(X15), method, Immediate(Method::EXTRA_LITERAL_INFO_OFFSET));
        assembler->Andi(Register(X15), Register(X15), Immediate(0xff));
        
        if(!isNew){
            assembler->Lui(x28, Immediate(0));
            assembler->Addi(x28, x28, Immediate(kungfu::BuiltinsStubCSigns::BUILTINS_CONSTRUCTOR_STUB_FIRST));
            assembler->Bge(Register(X15), x28, assembler->LinkAndGetInstOffsetToLabel(&lCallNativeCpp));//LinkAndGetInstOffsetToLabel is stub in assembler/riscv64/assembler_riscv64.cpp
        }

        assembler->Add(builtinStub, glue, Register(X15));//UXTW=2, FRAME_SLOT_SIZE_LOG2=3
        assembler->Ld(builtinStub, builtinStub, Immediate(JSThread::GlueData::GetBuiltinsStubEntriesOffset(false)));
        assembler->Ld(Register(X11), method, Immediate(Method::NATIVE_POINTER_OR_BYTECODE_ARRAY_OFFSET));

        assembler->Ld(Register(X12), sp, Immediate(CommonCall::FRAME_SLOT_SIZE)); 

        assembler->Ld(Register(X13), sp, Immediate(32)); //QUADRUPLE_SLOT_SIZE=32

        assembler->Ld(Register(X14), sp, Immediate(24)); //TRIPLE_SLOT_SIZE=24

        assembler->Ld(Register(X15), sp, Immediate(0));

        assembler->Lui(x28, Immediate(0));
        assembler->Addi(x28, x28, Immediate(NUM_MANDATORY_JSFUNC_ARGS));
        assembler->Sub(Register(X15), Register(X15), x28);

        if(!isNew){
            Label lTailCall;
            Register fp(X8);
            assembler->Ld(Register(X16), sp, Immediate(32));//QUADRUPLE_SLOT_SIZE=32
            assembler->Ld(Register(X16), sp, Immediate(32 + 8));//QUADRUPLE_SLOT_SIZE=32

            assembler->Lui(x5, Immediate(0));
            assembler->Addi(x5, x5, Immediate(3));
            assembler->Bne(Register(X15), x5, assembler->LinkAndGetInstOffsetToLabel(&lTailCall));//LinkAndGetInstOffsetToLabel is stub in assembler/riscv64/assembler_riscv64.cpp
            PushAsmBridgeFrame(assembler);
            {
                TempRegister2Scope scope2(assembler);
                Register arg3 = assembler->TempRegister2();
                assembler->Ld(arg3, fp, Immediate(64));//OCTUPLE_SLOT_SIZE=64
                assembler->Sd(arg3, sp, Immediate(-32));//QUADRUPLE_SLOT_SIZE=32, PREINDEX=1
                assembler->Sd(Register(X17), sp, Immediate(-32 + 8));//QUADRUPLE_SLOT_SIZE=32, PREINDEX=1
                assembler->Blr(builtinStub);
                assembler->Addi(sp, sp, Immediate(CommonCall::DOUBLE_SLOT_SIZE));
            }
            PopOptimizedFrame(assembler);
            assembler->Ret();
            assembler->Bind(&lTailCall);
            {
                assembler->Br(builtinStub);//Br is stub in assembler/riscv64/assembler_riscv64.cpp
            }
        } else {
            assembler->Addi(Register(X16), sp, Immediate(32));//QUADRUPLE_SLOT_SIZE=32
            assembler->Br(builtinStub);
        }
    }

    Label jsBoundFunction;
    Label jsProxy;

    assembler->Bind(&notJSFunction);
    {
        Register bitfield(X12);
        Register jstype2(X15);
        assembler->Andi(jstype2, bitfield, Immediate(0xff));
        assembler->Lui(x5, Immediate(0));
        assembler->Addi(x5, x5, Immediate(static_cast<int64_t>(JSType::JS_BOUND_FUNCTION)));
        assembler->Beq(jstype2, x5, assembler->LinkAndGetInstOffsetToLabel(&jsBoundFunction));//LinkAndGetInstOffsetToLabel is stub in assembler/riscv64/assembler_riscv64.cpp

        assembler->Lui(x5, Immediate(0));
        assembler->Addi(x5, x5, Immediate(static_cast<int64_t>(JSType::JS_PROXY)));
        assembler->Beq(jstype2, x5, assembler->LinkAndGetInstOffsetToLabel(&jsProxy));//LinkAndGetInstOffsetToLabel is stub in assembler/riscv64/assembler_riscv64.cpp

        assembler->Ret();
    }

    assembler->Bind(&jsBoundFunction);
    {
        JSBoundFunctionCallInternal(assembler, glue, actualArgC, jsfunc, RTSTUB_ID(JSCall));
    }
    assembler->Bind(&jsProxy);
    {
        JSProxyCallInternal(assembler, sp, jsfunc);
    }
    assembler->Bind(&nonCallable);
    {
        ThrowNonCallableInternal(assembler, sp);
    }
    assembler->Bind(&lCallConstructor);
    {
        Register frameType(X16);

        assembler->PushFpAndLr();//PushFpAndLr is stub in assembler/riscv64/extended_assembler_riscv64.cpp

        assembler->Lui(frameType, Immediate(0));
        assembler->Addi(frameType, frameType, Immediate(static_cast<int64_t>(FrameType::OPTIMIZED_JS_FUNCTION_ARGS_CONFIG_FRAME)));

        assembler->Sd(ZERO, sp, Immediate(-ExtendedAssembler::FRAME_SLOT_SIZE * 2));//AddrMode::PREINDEX
        assembler->Sd(frameType, sp, Immediate(-ExtendedAssembler::FRAME_SLOT_SIZE * 2 + 8));//AddrMode::PREINDEX

        assembler->Addi(Register(FP), sp, Immediate(CommonCall::DOUBLE_SLOT_SIZE));

        Register argC(X15);
        Register runtimeId(X16);

        assembler->Lui(argC, Immediate(0));
        assembler->Lui(runtimeId, Immediate(0));
        assembler->Addi(runtimeId, runtimeId, Immediate(RTSTUB_ID(ThrowCallConstructorException)));

        assembler->Sd(runtimeId, sp, Immediate(-ExtendedAssembler::FRAME_SLOT_SIZE * 2));//AddrMode::PREINDEX
        assembler->Sd(argC, sp, Immediate(-ExtendedAssembler::FRAME_SLOT_SIZE * 2 + 8));//AddrMode::PREINDEX

        assembler->CallAssemblerStub(RTSTUB_ID(CallRuntime), false);//CallAssemblerStub is stub in assembler/riscv64/extended_assembler_riscv64.cpp
        assembler->Addi(sp, sp, Immediate(4 * ExtendedAssembler::FRAME_SLOT_SIZE));
        assembler->RestoreFpAndLr();//RestoreFpAndLr is stub in assembler/riscv64/extended_assembler_riscv64.cpp
        assembler->Ret();
    }
}

// PopJSFunctionArgs/JSCallWithArgVAndPushUndefined/JSCallWithArgV/CallOptimized/DeoptEnterAsmInterp/DeoptHandlerAsm are implemented by group 32(chencan gaoxingzheng)
void OptimizedCall::PopJSFunctionArgs(ExtendedAssembler *assembler, Register expectedNumArgs, Register actualNumArgs) {
    Register sp(SP);
    Register fp(A6);
    Label aligned;
    const int64_t argoffsetSlot = static_cast<int64_t>(CommonArgIdx::FUNC) - 1;
    TempRegister1Scope scop1(assembler);
    Register tmp = assembler->TempRegister1();
    if (expectedNumArgs != actualNumArgs) {
        assembler->Slt(tmp, expectedNumArgs, actualNumArgs);
        assembler->Bne(tmp, ZERO, assembler->LinkAndGetInstOffsetToLabel(&aligned));
        assembler->Add(sp, sp, expectedNumArgs);
    } else {
        assembler->Add(sp, sp, expectedNumArgs);
    }

    assembler->Addi(sp, sp, Immediate(argoffsetSlot * FRAME_SLOT_SIZE));
    assembler->Add(fp, ZERO, sp);

    assembler->Andi(tmp, fp, Immediate(0xf));
    assembler->Beq(tmp, ZERO, assembler->LinkAndGetInstOffsetToLabel(&aligned));
    assembler->Addi(sp, sp, Immediate(FRAME_SLOT_SIZE));

    assembler->Bind(&aligned);
}


void OptimizedCall::JSCallWithArgVAndPushUndefined(ExtendedAssembler *assembler) {
    assembler->BindAssemblerStub(RTSTUB_ID(JSCallWithArgVAndPushUndefined));
    GenJSCallWithArgV(assembler, true);
}

void OptimizedCall::JSCallWithArgV(ExtendedAssembler *assembler) {
    assembler->BindAssemblerStub(RTSTUB_ID(JSCallWithArgV));
    GenJSCallWithArgV(assembler, false);
}


void OptimizedCall::CallOptimized(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(CallOptimized));
    Register sp(SP);
    Register jsfunc(A7);
    Register method(A6);
    Register codeAddr(A5);

    assembler->Ld(jsfunc, sp, Immediate(FRAME_SLOT_SIZE));

    assembler->Ld(method, jsfunc, Immediate(JSFunction::METHOD_OFFSET));

    assembler->Ld(codeAddr, method, Immediate(Method::CODE_ENTRY_OFFSET));

    assembler->Jalr(ZERO, codeAddr, 0);
}

void OptimizedCall::DeoptEnterAsmInterp(ExtendedAssembler *assembler)
{

    Register glueRegister = assembler->GlueRegister();
    Register context(S1);
    Register opRegister(T0);
    Register outputCount(T1);
    Register frameStateBase(T2);
    Register currentSlotRegister(T3);
    Register sp(SP);
    Register depth(S2);
    Register tmpReg(S3);
    Label loopBegin;
    Label stackOverflow;
    Label pushArgv;

    assembler->PushFpAndRa();

    assembler->Ld(depth, context, Immediate(AsmStackContext::GetInlineDepthOffset(false)));
    assembler->Addi(context, context, Immediate(AsmStackContext::GetSize(false)));
    assembler->Addiw(T6, ZERO, Immediate(0));

    assembler->Addiw(currentSlotRegister, sp, Immediate(0));
    assembler->Bind(&loopBegin);
    assembler->Ld(outputCount, context, Immediate(0));
    assembler->Addi(frameStateBase, context, Immediate(FRAME_SLOT_SIZE));
    assembler->Beq(T6, ZERO, assembler->LinkAndGetInstOffsetToLabel(&pushArgv));
    assembler->Addiw(tmpReg, currentSlotRegister, Immediate(AsmInterpretedFrame::GetSize(false)));
    assembler->Addi(T0, frameStateBase, Immediate(AsmInterpretedFrame::GetBaseOffset(false)));
    assembler->Sd(tmpReg, T0, Immediate(InterpretedFrameBase::GetPrevOffset(false)));

    assembler->Bind(&pushArgv);
    assembler->Addiw(tmpReg, outputCount, 0);
    assembler->Sd(currentSlotRegister, frameStateBase, Immediate(AsmInterpretedFrame::GetFpOffset(false)));
    PushArgsWithArgv(assembler, glueRegister, outputCount, frameStateBase, opRegister,
                     currentSlotRegister, nullptr, &stackOverflow);
    assembler->Addi(context, context, Immediate(FRAME_SLOT_SIZE));
    assembler->Add(context, context, tmpReg);
    assembler->Addiw(T6, T6, Immediate(1));
    assembler->Bgeu(depth, T6, assembler->LinkAndGetInstOffsetToLabel(&loopBegin));

    Register callTargetRegister = assembler->CallDispatcherArgument(kungfu::CallDispatchInputs::CALL_TARGET);
    Register methodRegister = assembler->CallDispatcherArgument(kungfu::CallDispatchInputs::METHOD);
    {
        assembler->Ld(callTargetRegister, frameStateBase, Immediate(AsmInterpretedFrame::GetFunctionOffset(false)));
        assembler->Ld(S2, frameStateBase, Immediate(AsmInterpretedFrame::GetPcOffset(false)));
        assembler->Ld(T6, frameStateBase, Immediate(AsmInterpretedFrame::GetAccOffset(false)));
        assembler->Ld(methodRegister, callTargetRegister, Immediate(JSFunctionBase::METHOD_OFFSET));

        assembler->Addiw(opRegister, currentSlotRegister, Immediate(AsmInterpretedFrame::GetSize(false)));

        assembler->Addiw(SP, currentSlotRegister, Immediate(0));
        AsmInterpreterCall::DispatchCall(assembler, S2, opRegister, T6);
    }

    assembler->Bind(&stackOverflow);
    {
        Register temp(A0);
        AsmInterpreterCall::ThrowStackOverflowExceptionAndReturn(
                assembler, glueRegister, sp, temp);
    }
}

void OptimizedCall::DeoptHandlerAsm(ExtendedAssembler *assembler)
{

    assembler->BindAssemblerStub(RTSTUB_ID(DeoptHandlerAsm));

    assembler->PushFpAndRa();
    Register sp(SP);
    Register fp(FP);
    Register frameType(S3);
    Register glueReg(A0);

	assembler->Addi(frameType, ZERO, Immediate(static_cast<int64_t>(FrameType::ASM_BRIDGE_FRAME)));
    assembler->Addi(sp, sp, Immediate(-DOUBLE_SLOT_SIZE));
    assembler->Sd(sp, glueReg, Immediate(FRAME_SLOT_SIZE));
    assembler->Sd(sp, frameType, Immediate(0));

    assembler->CalleeSave();

    Register deoptType(A1); // X1 -> A1
    Register depth(S1); // X2 -> S1
    Register argC(S3); // X3 -> S3
    Register runtimeId(S4); // X4 -> S4
    assembler->Addi(sp, sp, Immediate(-DOUBLE_SLOT_SIZE));
    assembler->Sd(sp, deoptType, Immediate(FRAME_SLOT_SIZE));
    assembler->Sd(sp, depth, Immediate(0));
	
    assembler->Addi(argC, ZERO, Immediate(2));
    assembler->Addi(runtimeId, ZERO, Immediate(RTSTUB_ID(DeoptHandler)));
	
    assembler->Addi(sp, sp, Immediate(-DOUBLE_SLOT_SIZE));
    assembler->Sd(sp, runtimeId, Immediate(FRAME_SLOT_SIZE));
    assembler->Sd(sp, argC, Immediate(0));
	
    assembler->CallAssemblerStub(RTSTUB_ID(CallRuntime), false);
    assembler->Addi(sp, sp, Immediate(2 * DOUBLE_SLOT_SIZE));

    assembler->CalleeRestore();

    Register context(S1); // X2 -> S1
    assembler->Addi(context, Register(X0), Immediate(0));
    assembler->Ld(glueReg, sp, Immediate(0));

    Register ret(A0); // X0 -> A0
    Label stackOverflow;
	Register tempReg(T0);
	assembler->Lui(tempReg, Immediate(JSTaggedValue::VALUE_EXCEPTION));
    assembler->Beq(ret, tempReg, assembler->LinkAndGetInstOffsetToLabel(&stackOverflow));

    Label target;
    Register temp(A1); // X1 -> A1
    assembler->Ld(fp, context, Immediate(AsmStackContext::GetCallerFpOffset(false)));
    assembler->Ld(temp, context, Immediate(AsmStackContext::GetCallFrameTopOffset(false)));
    assembler->Addi(sp, temp, Immediate(0));
    assembler->Ld(T6, context, Immediate(AsmStackContext::GetReturnAddressOffset(false))); // X30 -> T6

    PushAsmInterpBridgeFrame(assembler);
	assembler->Jal(ZERO, assembler->LinkAndGetInstOffsetToLabel(&target));
    PopAsmInterpBridgeFrame(assembler);
    assembler->Ret();
    assembler->Bind(&target);
    DeoptEnterAsmInterp(assembler);

    assembler->Bind(&stackOverflow);
    {
        assembler->Addi(runtimeId, ZERO, Immediate(RTSTUB_ID(ThrowStackOverflowException)));
		// 2 : 2 means pair
        assembler->Addi(sp, sp, Immediate(-DOUBLE_SLOT_SIZE));
        assembler->Sd(sp, runtimeId, Immediate(FRAME_SLOT_SIZE));
        assembler->Sd(sp, Register( ZERO), Immediate(0));
        assembler->CallAssemblerStub(RTSTUB_ID(CallRuntime), false);
        assembler->Addi(sp, sp, Immediate(2 * DOUBLE_SLOT_SIZE));
        assembler->RestoreFpAndRa();
        assembler->Ret();
    }
}

}