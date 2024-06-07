#include "ecmascript/compiler/trampoline/riscv64/common_call.h"
#include "ecmascript/compiler/assembler/assembler.h"
#include "ecmascript/compiler/argument_accessor.h"
#include "ecmascript/compiler/common_stubs.h"
#include "ecmascript/compiler/rt_call_signature.h"
#include "ecmascript/ecma_runtime_call_info.h"
#include "ecmascript/frames.h"
#include "ecmascript/js_function.h"
#include "ecmascript/method.h"
#include "ecmascript/js_thread.h"
#include "ecmascript/js_generator_object.h"
#include "ecmascript/message_string.h"
#include "ecmascript/runtime_call_id.h"

namespace panda::ecmascript::riscv64 
{
using Label = panda::ecmascript::Label;
#define __ assembler->

void AsmInterpreterCall::DispatchCall(ExtendedAssembler *assembler, [[maybe_unused]]Register pcRegister,[[maybe_unused]]
    Register newSpRegister, [[maybe_unused]]Register accRegister)
{
    assembler->BindAssemblerStub(RTSTUB_ID(AsmInterpreterEntry));
    uint8_t* buffer =  assembler->getBuffer();
    printf("%s\n", buffer);
}

void AsmInterpreterCall::JSCallCommonEntry(ExtendedAssembler *assembler, JSCallMode mode)
{
    [[maybe_unused]]ExtendedAssembler *extass=assembler;
    [[maybe_unused]]JSCallMode m=mode;
}

void AsmInterpreterCall::AsmInterpreterEntry(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(AsmInterpreterEntry));
    uint8_t* buffer =  assembler->getBuffer();
    printf("%s\n", buffer);
}
void AsmInterpreterCall::GeneratorReEnterAsmInterp(ExtendedAssembler *assembler)
{
    __ BindAssemblerStub(RTSTUB_ID(GeneratorReEnterAsmInterp));
    Label target;
    size_t begin = __ GetCurrentPosition();
    //TODO:PushAsmInterpEntryFrame haven't been realized,use PushAsmInterpBridgeFrame zanshi ti dai
    PushAsmInterpBridgeFrame(assembler);
    // jump and link
    //__ Jal(&target);

    //TO DO not sure
    __ Jal(RA,Immediate(0));

    //TODO:same as above
    PopAsmInterpBridgeFrame(assembler);
    size_t end = __ GetCurrentPosition();
    // Not set RISCV64EntryFrameDuration value
    // In assembler.h
    if ((end - begin) != FrameCompletionPos::RISCV64EntryFrameDuration) {
        LOG_COMPILER(FATAL) << (end - begin) << " != " << FrameCompletionPos::RISCV64EntryFrameDuration
                            << "This frame has been modified, and the offset EntryFrameDuration should be updated too.";
    }
    // return
    //TODO Jr() not sure
    __ Jr();
    __ Bind(&target);
    GeneratorReEnterAsmInterpDispatch(assembler);
}
//TODO:need a lot
void AsmInterpreterCall::GeneratorReEnterAsmInterpDispatch(ExtendedAssembler *assembler)
{
    // assembler->CalleeSave();
    Label stackOverflow;
    Label pushFrameState;
    // //TO DO 
    Register glueRegister = __ GlueRegister();
    // Register glueRegister;
    // X12 is a1, a1 is the 2nd arg
    // unused
    // Register contextRegister = X12;

    //sp
    //To figure out
    //Register spRegister = X2;
    Register spRegister = SP;

    // resume asm interp frame
    // aarch64 x8 store the return value
    // riscv64 x10 store the return value
    // unused
    // Register pcRegister = X10;
    // riscv64 x8 -framePointer
    // aarch64 FP
    // x64     rbp
    // unused
    // Register prevSpRegister = X8;
    // riscv64 x15 -Function argument
    // aarch64 x4
    // x64     r9
    Register callTargetRegister = X15;
    // riscv64 x13
    // aarch64 x5
    // x64     rcx
    Register methodRegister = X13;
    // riscv64 x16 caller-save
    // aarch64 x6  caller-save
    // x64     r11
    Register tempRegister = X16;  // can not be used to store any variable
    // riscv64 x17
    // aarch64 x7
    Register currentSlotRegister = X17;
    // X5 temporary caller-save
    Register fpRegister = X5;
    // X20 is callee-register
    Register thisRegister = X20;
    // X18 is callee-save register, W is 32-bits
    //TODO:not sure XLEN_32 is 32-bits
    Register nRegsRegister(X18,XLEN_32);
    // x19 is callee-save register
    Register regsArrayRegister = X19;
    // x14 is 5th arg
    Register newSpRegister = X14;

    // load-store instruction
    // RV64I load 64-bit
    //__ Ld(Operand(X12, GeneratorContext::GENERATOR_METHOD_OFFSET), callTargetRegister);
    //TODO:not sure immediate
    __ Ld(X12,callTargetRegister,Immediate(0));
    //TODO:not sure immediate
    // __ Ld(Operand(callTargetRegister, JSFunctionBase::METHOD_OFFSET), methodRegister,Immediate(0));
    __ Ld(callTargetRegister,methodRegister,Immediate(0));
    //ra linkRegister X1
    __ PushFpAndRa();

    // __ Ld(rsp, fpRegister);
    // save fp
    // Lbu 8-bits
    //TODO:not sure immediate
    __ Lbu(fpRegister, spRegister,Immediate(0));
    __ Lbu(currentSlotRegister, spRegister,Immediate(0));
    // Reserve enough sp space to prevent stack parameters from being covered by cpu profiler.
    // __ Ld(tempRegister, MemoryOperand(glueRegister, JSThread::GlueData::GetCurrentContextOffset(false)),Immediate(0));
    __ Ld(tempRegister, glueRegister, Immediate(0));
    // __ Ld(tempRegister, MemoryOperand(tempRegister, 0),Immediate(0));
    __ Ld(tempRegister, tempRegister, Immediate(0));
    __ Lbu(spRegister,tempRegister,Immediate(0));

    // push context regs
    // __ Ld(Operand(X11, GeneratorContext::GENERATOR_NREGS_OFFSET), nRegsRegister,Immediate(0));
    __ Ld(X11, nRegsRegister,Immediate(0));
    // __ Ld(Operand(X11, GeneratorContext::GENERATOR_THIS_OFFSET), thisRegister,Immediate(0));
    __ Ld(X11, thisRegister,Immediate(0));
    // __ Ld(Operand(X11, GeneratorContext::GENERATOR_REGS_ARRAY_OFFSET), regsArrayRegister,Immediate(0));
    __ Ld(X11, regsArrayRegister,Immediate(0));
    // __ Addi(regsArrayRegister,regsArrayRegister,TaggedArray::DATA_OFFSET);
    __ Addi(regsArrayRegister,regsArrayRegister,Immediate(0));
    // PushArgsWithArgv(assembler, glueRegister, nRegsRegister, regsArrayRegister, tempRegister,
    //                  currentSlotRegister, &pushFrameState, &stackOverflow);

    __ Bind(&pushFrameState);
    __ Lbu(newSpRegister, currentSlotRegister,Immediate(0));
    
    // PushGeneratorFrameState(assembler, prevSpRegister, fpRegister, callTargetRegister, thisRegister, methodRegister,
    //     contextRegister, pcRegister, tempRegister);

    // call bc stub
    // DispatchCall(assembler, pcRegister, newSpRegister, methodRegister);
    __ Bind(&stackOverflow);
    {
        ThrowStackOverflowExceptionAndReturn(assembler, glueRegister, fpRegister, tempRegister);
    }
}
//TODO:ThrowStackOverflowExceptionAndReturn
void AsmInterpreterCall::ThrowStackOverflowExceptionAndReturn(ExtendedAssembler *assembler, Register glue,
    Register fp, Register op)
{
    assembler->CalleeSave();
    glue.GetId();
    fp.GetId();
    op.GetId();
}

void AsmInterpreterCall::PushCallArgsAndDispatchNative(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(PushCallArgsAndDispatchNative));

    Register nativeCode(A0);
    Register nativeCodeTemp(A2);
    Register glue(A1);
    Register argv(A5);
    Register temp(A6);
    Register sp(SP);
    
    assembler->Addi(nativeCodeTemp, nativeCode,Immediate(0));
    assembler->Ld(glue,sp,Immediate(0));
    assembler->Addi(Register(A0), sp, Immediate(0));
    PushBuiltinFrame(assembler, glue, FrameType::BUILTIN_FRAME, temp, argv);

    CallNativeInternal(assembler, nativeCodeTemp);
    assembler->Ret();
}

void AsmInterpreterCall::PushCallNewAndDispatchNative(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(PushCallNewAndDispatchNative));
    uint8_t* buffer =  assembler->getBuffer();
    printf("%s\n", buffer);
}

void AsmInterpreterCall::PushCallRangeAndDispatchNative(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(PushCallRangeAndDispatchNative));
    uint8_t* buffer =  assembler->getBuffer();
    printf("%s\n", buffer);
}

void AsmInterpreterCall::PushCallThisRangeAndDispatch(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(PushCallThisRangeAndDispatch));
    JSCallCommonEntry(assembler, JSCallMode::CALL_THIS_WITH_ARGV);
}

void AsmInterpreterCall::PushCallRangeAndDispatch(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(PushCallRangeAndDispatch));
    JSCallCommonEntry(assembler, JSCallMode::CALL_WITH_ARGV);
}

void AsmInterpreterCall::PushCallNewAndDispatch(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(PushCallNewAndDispatch));
    JSCallCommonEntry(assembler, JSCallMode::CALL_CONSTRUCTOR_WITH_ARGV);
}

void AsmInterpreterCall::PushCallArgs3AndDispatch(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(PushCallArgs3AndDispatch));
    JSCallCommonEntry(assembler, JSCallMode::CALL_ARG3);
}

void AsmInterpreterCall::PushCallArgs2AndDispatch(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(PushCallArgs2AndDispatch));
    JSCallCommonEntry(assembler, JSCallMode::CALL_ARG2);
}

void AsmInterpreterCall::PushCallArg1AndDispatch(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(PushCallArg1AndDispatch));
    JSCallCommonEntry(assembler, JSCallMode::CALL_ARG1);
}

void AsmInterpreterCall::PushCallArg0AndDispatch(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(PushCallArg0AndDispatch));
    JSCallCommonEntry(assembler, JSCallMode::CALL_ARG0);
}

void AsmInterpreterCall::PushCallThisArg0AndDispatch(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(PushCallThisArg0AndDispatch));
    JSCallCommonEntry(assembler, JSCallMode::CALL_THIS_ARG0);
}

void AsmInterpreterCall::PushCallThisArg1AndDispatch(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(PushCallThisArg1AndDispatch));
    JSCallCommonEntry(assembler, JSCallMode::CALL_THIS_ARG1);
}

void AsmInterpreterCall::PushCallThisArgs2AndDispatch(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(PushCallThisArgs2AndDispatch));
    JSCallCommonEntry(assembler, JSCallMode::CALL_THIS_ARG2);
}

void AsmInterpreterCall::PushCallThisArgs3AndDispatch(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(PushCallThisArgs3AndDispatch));
    JSCallCommonEntry(assembler, JSCallMode::CALL_THIS_ARG3);
}

void AsmInterpreterCall::ResumeRspAndDispatch(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(ResumeRspAndDispatch));

    Register glueRegister = assembler->GlueRegister();
    Register sp(FP);
    Register rsp(SP);
    Register pc(S2);
    Register jumpSizeRegister(S7);

    Register ret(S5);
    Register opcode(A6);//Should be a W register 
    Register temp(A7);
    Register bcStub(A7);
    Register fp(S0);
    
    int64_t fpOffset = static_cast<int64_t>(AsmInterpretedFrame::GetFpOffset(false))
        - static_cast<int64_t>(AsmInterpretedFrame::GetSize(false));
    int64_t spOffset = static_cast<int64_t>(AsmInterpretedFrame::GetBaseOffset(false))
        - static_cast<int64_t>(AsmInterpretedFrame::GetSize(false));
    int64_t thisOffset = static_cast<int64_t>(AsmInterpretedFrame::GetThisOffset(false))
        - static_cast<int64_t>(AsmInterpretedFrame::GetSize(false));
    ASSERT(fpOffset < 0);
    ASSERT(spOffset < 0);
    
    Label newObjectRangeReturn;
    Label dispatch;
    assembler->Lw(fp, sp, Immediate(fpOffset));  // store fp for temporary
    
    assembler->Bge(Register(ZERO), jumpSizeRegister, assembler->LinkAndGetInstOffsetToLabel(&newObjectRangeReturn));
    assembler->Lw(sp, sp, Immediate(spOffset));
    

    assembler->Sll(Register(T3),jumpSizeRegister,Register(ZERO));
    assembler->Add(pc, pc, Register(T3));
    
    assembler->Lb(opcode, pc, Immediate(0));
    assembler->Bind(&dispatch);
    {
        assembler->Addi(rsp, fp, Immediate(0));
        
        assembler->Addi(Register(T5),Register(ZERO),FRAME_SLOT_SIZE_LOG2);
        assembler->Sll(Register(T3), opcode, Register(T5));
        assembler->Add(bcStub, glueRegister, Register(T3));
        assembler->Ld(bcStub, bcStub, Immediate(JSThread::GlueData::GetBCStubEntriesOffset(false)));
        assembler->Jal(bcStub,Immediate(0));
    }

    Label getThis;
    Label notUndefined;
    // newObjectRangeReturn label
    assembler->Bind(&newObjectRangeReturn);
    {
        assembler->Addi(Register(T3),Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
        assembler->Bne(ret, Register(T3), assembler->LinkAndGetInstOffsetToLabel(&notUndefined));
        ASSERT(thisOffset < 0);
        assembler->Bind(&getThis);
        assembler->Lw(ret, sp, Immediate(thisOffset));  // update acc
        assembler->Lw(sp, sp, Immediate(spOffset));  // update sp
        assembler->Addi(rsp, fp, Immediate(0));
        assembler->Sub(pc, pc, jumpSizeRegister);
        assembler->Lb(opcode, pc, Immediate(0));

        assembler->Addi(Register(T5),Register(ZERO),FRAME_SLOT_SIZE_LOG2);
        assembler->Sll(Register(T3), opcode, Register(T5));
        assembler->Add(bcStub, glueRegister, Register(T3));
        
        assembler->Ld(bcStub, bcStub, Immediate(JSThread::GlueData::GetBCStubEntriesOffset(false)));
        assembler->Jal(bcStub,Immediate(0));
    }
    
    //process notUndefined label
    assembler->Bind(&notUndefined);
    {
        Label notEcmaObject;
        assembler->Addi(temp, Register(ZERO), Immediate(static_cast<int32_t>(JSTaggedValue::TAG_HEAPOBJECT_MASK)));
        assembler->And(temp, temp, ret);
        
        assembler->Bne(temp, Register(ZERO), assembler->LinkAndGetInstOffsetToLabel(&notEcmaObject));
        // acc is heap object
        assembler->Ld(temp, ret, Immediate(TaggedObject::HCLASS_OFFSET));
        assembler->Ld(temp, temp, Immediate(JSHClass::BIT_FIELD_OFFSET));
        assembler->Andi(temp, temp, Immediate(0xFF));
        
        assembler->Addi(Register(T3),Register(ZERO), Immediate(static_cast<int64_t>(JSType::ECMA_OBJECT_LAST)));
        assembler->Bgeu(temp, Register(T3), assembler->LinkAndGetInstOffsetToLabel(&notEcmaObject));
        
        assembler->Addi(Register(T3),Register(ZERO), Immediate(static_cast<int64_t>(JSType::ECMA_OBJECT_FIRST)));
        assembler->Bltu(temp, Register(T3), assembler->LinkAndGetInstOffsetToLabel(&notEcmaObject));
        // acc is ecma object
        assembler->Lw(sp, sp, Immediate(spOffset));  // update sp
        assembler->Sub(pc, pc, jumpSizeRegister);
        assembler->Lb(opcode, pc, Immediate(0));
        assembler->Jal(Register(ZERO), assembler->LinkAndGetInstOffsetToLabel(&dispatch));

        // not ECMAScript
        assembler->Bind(&notEcmaObject);
        {
            int64_t constructorOffset = static_cast<int64_t>(AsmInterpretedFrame::GetFunctionOffset(false))
                - static_cast<int64_t>(AsmInterpretedFrame::GetSize(false));
            ASSERT(constructorOffset < 0);
            assembler->Lw(temp, sp, Immediate(constructorOffset));
            assembler->Ld(temp, temp, Immediate(JSFunctionBase::METHOD_OFFSET));
            assembler->Ld(temp, temp, Immediate(Method::EXTRA_LITERAL_INFO_OFFSET));
            
            assembler->Addi(Register(S10),Register(ZERO),MethodLiteral::FunctionKindBits::START_BIT);
            assembler->Srl(temp, temp,Register(S10));
            assembler->Andi(temp, temp, Immediate((1LU << MethodLiteral::FunctionKindBits::SIZE) - 1));
            
            assembler->Addi(Register(T3),Register(ZERO), Immediate(static_cast<int64_t>(FunctionKind::CLASS_CONSTRUCTOR)));
            assembler->Bltu(temp, Register(T3), assembler->LinkAndGetInstOffsetToLabel(&getThis));
            // exception branch
            {
                assembler->Addi(opcode, Register(ZERO), Immediate(kungfu::BytecodeStubCSigns::ID_NewObjectRangeThrowException));
                assembler->Lw(sp, sp, Immediate(spOffset));  // update sp
                assembler->Jal(Register(ZERO), assembler->LinkAndGetInstOffsetToLabel(&dispatch));
            }
        }
    }
}

void AsmInterpreterCall::ResumeRspAndReturn(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(ResumeRspAndReturn));
    Register rsp(SP);
    Register currentSp(S4);

    [[maybe_unused]] TempRegister1Scope scope1(assembler);
    Register fpRegister = assembler->TempRegister1();
    int64_t offset = static_cast<int64_t>(AsmInterpretedFrame::GetFpOffset(false))
        - static_cast<int64_t>(AsmInterpretedFrame::GetSize(false));
    ASSERT(offset < 0);
    assembler->Ld(fpRegister, currentSp, Immediate(offset));
    assembler->Addi(rsp, fpRegister, Immediate(0));

    // return
    {
        assembler->RestoreFpAndRa();
        
        assembler->Addi(Register(A0), Register(S1), Immediate(0));
        assembler->Ret();
    }
}

void AsmInterpreterCall::ResumeCaughtFrameAndDispatch(ExtendedAssembler *assembler)
{
    assembler->BindAssemblerStub(RTSTUB_ID(ResumeCaughtFrameAndDispatch));

    Register glue(T0);
    Register pc(S2);
    Register fp(S0);
    Register opcode(A2);
    Register bcStub(A3);

    Label dispatch;
    assembler->Ld(fp, glue, JSThread::GlueData::GetLastFpOffset(false));
    assembler->Beq(fp, Register(ZERO), assembler->LinkAndGetInstOffsetToLabel(&dispatch));
    assembler->Add(Register(SP), fp, Register(ZERO));
    assembler->Bind(&dispatch);
    {
        assembler->Lbu(opcode, pc, Immediate(0));
        assembler->Lw(opcode, opcode, Immediate(0));
        assembler->Addi(Register(T5),Register(ZERO),FRAME_SLOT_SIZE_LOG2);
        assembler->Sll(opcode, opcode, Register(T5));
        assembler->Add(bcStub, glue, opcode);
        assembler->Ld(bcStub, bcStub, Immediate(JSThread::GlueData::GetBCStubEntriesOffset(false)));
        assembler->Jalr(Register(ZERO), bcStub, Immediate(0));
    }
}

void AsmInterpreterCall::ResumeUncaughtFrameAndReturn(ExtendedAssembler *assembler)
{
    assembler-> BindAssemblerStub(RTSTUB_ID(ResumeUncaughtFrameAndReturn));


    Register glue(T0);
    Register fp(S0);
    Register acc(T1);
    Register cppRet(A0);
    Register sp(SP);

    assembler->Ld(fp, glue, JSThread::GlueData::GetLastFpOffset(false));
    assembler->Add(sp, fp, Register(ZERO));
    assembler->Add(cppRet, acc, Register(ZERO));

    assembler->RestoreFpAndRa();

    assembler->Ret();
}
void AsmInterpreterCall::ResumeRspAndRollback(ExtendedAssembler *assembler)
{
    assembler-> BindAssemblerStub(RTSTUB_ID(ResumeRspAndRollback));

    Register glue(T0);
    Register spRegister(FP);

    Register pc(S2);
    Register jumpSizeRegister(T1);

    Register ret(S3);
    Register frameStateBase(A0); 
    Register opcode(A2);
    Register bcStub(A3);
    
    int64_t size = static_cast<int64_t>(AsmInterpretedFrame::GetSize(false));
    int64_t baseOffset = static_cast<int64_t>(AsmInterpretedFrame::GetBaseOffset(false));
    int64_t funcOffset = static_cast<int64_t>(AsmInterpretedFrame::GetFunctionOffset(false));
    int64_t fpOffset = static_cast<int64_t>(AsmInterpretedFrame::GetFpOffset(false));
    assembler->Addi(frameStateBase, spRegister, Immediate(-size)); // update sp
    assembler->Ld(spRegister, frameStateBase, Immediate(baseOffset));
    assembler->Add(pc, pc, jumpSizeRegister); // new PC

    assembler->Lbu(opcode, pc, Immediate(0));
    assembler->Ld(ret, frameStateBase, Immediate(funcOffset)); // restore acc
    assembler->Ld(Register(SP), frameStateBase, Immediate(fpOffset)); // resume rsp
	
    assembler->Addi(Register(A4),Register(ZERO),Immediate(FRAME_SLOT_SIZE_LOG2));
    assembler->Sll(opcode, opcode,Register(A4));
    assembler->Add(bcStub, glue, opcode);
    assembler->Ld(bcStub, bcStub, Immediate(JSThread::GlueData::GetBCStubEntriesOffset(false)));
    assembler->Jalr(Register(ZERO), bcStub, Immediate(0));
}

void AsmInterpreterCall::CallSetter(ExtendedAssembler *assembler)
{
    __ BindAssemblerStub(RTSTUB_ID(CallSetter));
    Label target;
    PushAsmInterpBridgeFrame(assembler);
    // jump and link
    //__ Jal(&target);
    //TO DO not sure
    __ Jal(RA,Immediate(0));
    PopAsmInterpBridgeFrame(assembler);
    __ Jr();
    __ Bind(&target);
    JSCallCommonEntry(assembler, JSCallMode::CALL_SETTER);
}

void AsmInterpreterCall::CallGetter(ExtendedAssembler *assembler)
{
    __ BindAssemblerStub(RTSTUB_ID(CallGetter));
    Label target;
    PushAsmInterpBridgeFrame(assembler);
    // jump and link
    //__ Jal(&target);
    //TO DO not sure
    __ Jal(RA,Immediate(0));
    PopAsmInterpBridgeFrame(assembler);
    __ Jr();
    __ Bind(&target);
    JSCallCommonEntry(assembler, JSCallMode::CALL_GETTER);
}

void AsmInterpreterCall::CallContainersArgs3(ExtendedAssembler *assembler)
{
    __ BindAssemblerStub(RTSTUB_ID(CallContainersArgs3));
    Label target;
    PushAsmInterpBridgeFrame(assembler);
    //GetArgvAtStack(assembler);
    //__ Jal(&target);
    //TO DO not sure
    __ Jal(RA,Immediate(0));
    PopAsmInterpBridgeFrame(assembler);
    __ Jr();
    __ Bind(&target);
    {
        JSCallCommonEntry(assembler, JSCallMode::CALL_THIS_ARG3_WITH_RETURN);
    }
}

void AsmInterpreterCall::CallReturnWithArgv(ExtendedAssembler *assembler)
{
    __ BindAssemblerStub(RTSTUB_ID(CallReturnWithArgv));
    Label target;
    PushAsmInterpBridgeFrame(assembler);
    // X19 is callee-save register
    //Register X19 = __ CppJSCallAvailableRegister1();
    // rbp -x9

    //__ Ld(Operand(x9, FRAME_SLOT_SIZE), x19);

    //__ Jal(&target);
    //TO DO not sure
    __ Jal(RA,Immediate(0));
    PopAsmInterpBridgeFrame(assembler);
    __ Jr();
    __ Bind(&target);
    {
        JSCallCommonEntry(assembler, JSCallMode::CALL_THIS_ARGV_WITH_RETURN);
    }
}
void AsmInterpreterCall::CallNativeWithArgv(ExtendedAssembler *assembler, bool callNew)
{
    Register glue(A0);
    Register nativeCode(A1);
    Register callTarget(A2);
    Register thisObj(A3);
    Register argc(A4);
    Register argv(A5);
    Register opArgc(S0);
    Register opArgv(T0);
    Register temp(T1);
    Register currentSlotRegister(T2);
    Register spRegister(SP);

    Label pushThis;
    Label stackOverflow;
    PushBuiltinFrame(assembler, glue, FrameType::BUILTIN_FRAME_WITH_ARGV, temp, argc);

    assembler->Addi(currentSlotRegister, spRegister,Immediate(0));
    // Reserve enough sp space to prevent stack parameters from being covered by cpu profiler.

    assembler->Ld(temp,glue,Immediate(JSThread::GlueData::GetCurrentContextOffset(false)));
    assembler->Ld(temp,temp,Immediate(0));
    assembler->Addi(Register(SP), temp,Immediate(0));


    assembler->Addi(opArgc, argc,Immediate(0));
    assembler->Addi(opArgv, argv,Immediate(0));
    PushArgsWithArgv(assembler, glue, opArgc, opArgv, temp, currentSlotRegister, &pushThis, &stackOverflow);

    assembler->Bind(&pushThis);
    // newTarget
    if (callNew) {
        // 16: this & newTarget
        assembler->Addi(currentSlotRegister,currentSlotRegister,-16);
        assembler->Sd(callTarget,currentSlotRegister,8);
        assembler->Sd(thisObj,currentSlotRegister,0);

    } else {
        assembler->Addi(temp,Register(ZERO), Immediate(JSTaggedValue::VALUE_UNDEFINED));
        // 16: this & newTarget
        assembler->Addi(currentSlotRegister,currentSlotRegister,-16);
        assembler->Sd(temp,currentSlotRegister,8);
        assembler->Sd(thisObj,currentSlotRegister,0);

    }
    // callTarget
    assembler->Addi(currentSlotRegister,currentSlotRegister,Immediate(-FRAME_SLOT_SIZE));
    assembler->Sd(callTarget,currentSlotRegister,Immediate(0));

    assembler->Addi(temp, currentSlotRegister, Immediate(40)); 
    
    
    assembler->Add(argc,argc,argc);
    assembler->Add(argc,argc,argc);
    assembler->Add(argc,argc,argc);
    
    assembler->Add(Register(FP), temp, argc);  // 3: argc * 8

    assembler->Addi(temp, argc, Immediate(NUM_MANDATORY_JSFUNC_ARGS));
    
    // 2: thread & argc
    assembler->Addi(currentSlotRegister,currentSlotRegister,-2 * FRAME_SLOT_SIZE);
    assembler->Sd(glue,currentSlotRegister,1 * FRAME_SLOT_SIZE);
    assembler->Sd(temp,currentSlotRegister,0 * FRAME_SLOT_SIZE);

    assembler->Addi(Register(A0), currentSlotRegister, Immediate(0));

    assembler->Align16(currentSlotRegister);

    assembler->Addi(spRegister, currentSlotRegister,Immediate(0));

    CallNativeInternal(assembler, nativeCode);
    assembler->Ret();

    assembler->Bind(&stackOverflow);
    {
        // use builtin_with_argv_frame to mark gc map
        Register frameType(T2);
        assembler->Ld(temp,glue,Immediate(JSThread::GlueData::GetLeaveFrameOffset(false)));
        assembler->Addi(spRegister, temp,Immediate(0));
        assembler->Addi(frameType, Register(ZERO),Immediate(static_cast<int32_t>(FrameType::BUILTIN_FRAME_WITH_ARGV_STACK_OVER_FLOW_FRAME)));
        // 2: frame type and argc

        assembler->Addi(Register(SP),Register(SP),-2 * FRAME_SLOT_SIZE );
        assembler->Sd(Register(ZERO),Register(SP),1 * FRAME_SLOT_SIZE );

        assembler->Sd(frameType,Register(SP),0 * FRAME_SLOT_SIZE );
    

        assembler->Addi(temp, Register(ZERO),Immediate(JSTaggedValue::VALUE_UNDEFINED));
        // 2: fill this&newtgt slots
        assembler->Addi(currentSlotRegister,currentSlotRegister,-4*FRAME_SLOT_SIZE);

        assembler->Sd(temp,spRegister,3*FRAME_SLOT_SIZE);
        assembler->Sd(temp,spRegister,2*FRAME_SLOT_SIZE);

        // 2: fill func&align slots
        assembler->Sd(Register(ZERO),spRegister,1*FRAME_SLOT_SIZE);
        assembler->Sd(temp,spRegister,0*FRAME_SLOT_SIZE);



        Register runtimeId(T2);
        Register trampoline(T3);
        assembler->Addi(runtimeId,Register(ZERO), Immediate(kungfu::RuntimeStubCSigns::ID_ThrowStackOverflowException));
        // 3 : 3 means *8
        assembler->Add(runtimeId,runtimeId,runtimeId);
        assembler->Add(runtimeId,runtimeId,runtimeId);
        assembler->Add(runtimeId,runtimeId,runtimeId);
        
        assembler->Add(trampoline, glue, runtimeId);  // 3: argc * 8

        assembler->Ld(trampoline,trampoline,Immediate(JSThread::GlueData::GetRTStubEntriesOffset(false)));

        assembler->Blr(trampoline);

        // resume rsp
        assembler->Add(Register(SP), Register(FP),Register(ZERO));
        assembler->RestoreFpAndRa();
    

        assembler->Ret();
    }
}
void AsmInterpreterCall::PushBuiltinFrame(ExtendedAssembler *assembler, Register glue,FrameType type, Register op, Register next){

    Register sp(SP);
    assembler->PushFpAndRa();

    assembler->Addi(op, sp,Immediate(0));
    assembler->Sd(op, glue, JSThread::GlueData::GetLeaveFrameOffset(false));
    assembler->Addi(op,Register(ZERO), Immediate(static_cast<int32_t>(type)));
    if (type == FrameType::BUILTIN_FRAME) {
        // push stack args
        assembler->Addi(next, sp, Immediate(BuiltinFrame::GetStackArgsToFpDelta(false)));
        // 16: type & next
        assembler->Addi(sp,sp,-2 * FRAME_SLOT_SIZE);
        assembler->Sd(next,sp,1 * FRAME_SLOT_SIZE);
        assembler->Sd(op,sp, FRAME_SLOT_SIZE);
        assembler->Addi(Register(FP), sp, Immediate(2 * FRAME_SLOT_SIZE));  // 16: skip next and frame type
    } else {
        // 16: type & next
        assembler->Addi(sp,sp,-2 * FRAME_SLOT_SIZE);
        assembler->Sd(next,sp,1 * FRAME_SLOT_SIZE);
        assembler->Sd(op,sp,0 * FRAME_SLOT_SIZE);
        
        assembler->Addi(Register(FP), sp, Immediate(2 * FRAME_SLOT_SIZE));  // 16: skip next and frame type
    }
}
void AsmInterpreterCall::CallNativeInternal(ExtendedAssembler *assembler, Register nativeCode)
{
    assembler->Jalr(Register(ZERO), nativeCode, Immediate(0));
    // resume rsp
    assembler->Addi(Register(SP), Register(FP), Immediate(0));
    assembler->RestoreFpAndRa();

}
Register AsmInterpreterCall::GetThisRegsiter(ExtendedAssembler *assembler, JSCallMode mode, Register defaultRegister)
{
    switch (mode) {
    case JSCallMode::CALL_GETTER:
    case JSCallMode::CALL_THIS_ARG0:
        return __ CallDispatcherArgument(kungfu::CallDispatchInputs::ARG0);
    case JSCallMode::CALL_SETTER:
    case JSCallMode::CALL_THIS_ARG1:
        return __ CallDispatcherArgument(kungfu::CallDispatchInputs::ARG1);
    case JSCallMode::CALL_THIS_ARG2:
    case JSCallMode::CALL_CONSTRUCTOR_WITH_ARGV:
    case JSCallMode::SUPER_CALL_WITH_ARGV:
    case JSCallMode::CALL_THIS_WITH_ARGV:
    case JSCallMode::CALL_THIS_ARGV_WITH_RETURN:
        return __ CallDispatcherArgument(kungfu::CallDispatchInputs::ARG2);
    case JSCallMode::CALL_THIS_ARG3:
    case JSCallMode::CALL_THIS_ARG3_WITH_RETURN:
        return __ CallDispatcherArgument(kungfu::CallDispatchInputs::ARG3);
    case JSCallMode::CALL_FROM_AOT:
    case JSCallMode::CALL_ENTRY: {
        Register argvRegister = __ CallDispatcherArgument(kungfu::CallDispatchInputs::ARG1);
        assembler->Lw(defaultRegister,argvRegister,Immediate(-FRAME_SLOT_SIZE));
        return defaultRegister;
    }
    default:
        LOG_ECMA(FATAL) << "this branch is unreachable";
        UNREACHABLE();
    }
    return INVALID_REG;
}


Register AsmInterpreterCall::GetNewTargetRegsiter(ExtendedAssembler *assembler, JSCallMode mode,
    Register defaultRegister)
{
    switch (mode) {
    case JSCallMode::CALL_CONSTRUCTOR_WITH_ARGV:
    case JSCallMode::CALL_THIS_WITH_ARGV:
        return __ CallDispatcherArgument(kungfu::CallDispatchInputs::CALL_TARGET);
    case JSCallMode::SUPER_CALL_WITH_ARGV:
        return __ CallDispatcherArgument(kungfu::CallDispatchInputs::ARG3);
    case JSCallMode::CALL_FROM_AOT:
    case JSCallMode::CALL_ENTRY: {
    Register argvRegister = __ CallDispatcherArgument(kungfu::CallDispatchInputs::ARG1);
        assembler->Lw(defaultRegister,argvRegister,Immediate(-2*FRAME_SLOT_SIZE));
        return defaultRegister;
    }
    default:
        LOG_ECMA(FATAL) << "this branch is unreachable";
        UNREACHABLE();
    }
    return INVALID_REG;
}


}// end panda::ecmascript::riscv64
