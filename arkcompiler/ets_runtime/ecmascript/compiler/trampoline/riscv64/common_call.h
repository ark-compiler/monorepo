
#ifndef ECMASCRIPT_COMPILER_RISCV64_EXTENDED_ASSEMBLER_H
#define ECMASCRIPT_COMPILER_RISCV64_EXTENDED_ASSEMBLER_H

#include "ecmascript/compiler/assembler/riscv64/assembler_riscv64.h"
#include "ecmascript/compiler/assembler/riscv64/extended_assembler_riscv64.h"
#include "ecmascript/frames.h"


namespace panda::ecmascript::riscv64 
{
using Label = panda::ecmascript::Label;
class CommonCall 
{
    public:
        static constexpr int FRAME_SLOT_SIZE_LOG2 = 3; // TODO: Not sure 3
        static constexpr int FRAME_SLOT_SIZE = 8; // TODO: Not sure 8
        static constexpr int DOUBLE_SLOT_SIZE = 16; // TODO: Not sure 16
        static constexpr int TRIPLE_SLOT_SIZE = 24; // TODO: Not sure 24
        // add by group 13
        static inline int64_t GetStackArgOffSetToFp(unsigned argId)
        {
        //   +--------------------------+
        //   |       argv0              | calltarget , newtARGET, this, ....
        //   +--------------------------+ ---
        //   |       argc               |   ^
        //   |--------------------------|   arguments
        //   |       env                |   |
        //   |--------------------------|   |
        //   |       codeAddress        |   |
        //   |--------------------------|   |
        //   |       returnAddr         |   |
        //   |--------------------------| Fixed OptimizedBuiltinLeaveFrame
        //   |       callsiteFp         |   |
        //   |--------------------------|   |
        //   |       frameType          |   v
        //   +--------------------------+ ---
        // 16 : 16 means arguments offset to fp
        return 16 + static_cast<int64_t>(argId) * static_cast<int64_t>(FRAME_SLOT_SIZE);
        };
        static void PushAsmInterpBridgeFrame(ExtendedAssembler *assembler);
        static void PopAsmInterpBridgeFrame(ExtendedAssembler *assembler);
        static void PushLeaveFrame(ExtendedAssembler *assembler, Register glue);
        static void PopLeaveFrame(ExtendedAssembler *assembler);
        static void PushArgsWithArgv(ExtendedAssembler *assembler, Register glue, Register argc, Register argv,Register op, Register fp, Label *next, Label *stackOverflow);
        static void PushUndefinedWithArgc(ExtendedAssembler *assembler, Register glue, Register argc, Register temp, Register currentSlot, Label *next, Label *stackOverflow);
};

class OptimizedCall : public CommonCall 
{
    public:
        static void CallRuntime(ExtendedAssembler *assembler);
        static void CallRuntimeWithArgv(ExtendedAssembler *assembler);
        static void JSFunctionEntry(ExtendedAssembler *assembler);
        static void JSCall(ExtendedAssembler *assembler);
        static void JSCallWithArgV(ExtendedAssembler*assembler);
        static void JSCallWithArgVAndPushUndefined(ExtendedAssembler *assembler);
        static void JSProxyCallInternalWithArgV(ExtendedAssembler *assembler);
        static void OptimizedCallAndPushUndefined(ExtendedAssembler *assembler);
        static void OptimizedCallAsmInterpreter(ExtendedAssembler *assembler);
        static void DeoptHandlerAsm(ExtendedAssembler *assembler);
        static void DeoptEnterAsmInterp(ExtendedAssembler *assembler);
        static void JSCallNew(ExtendedAssembler *assembler);
        static void GenJSCall(ExtendedAssembler *assembler, bool isNew);
        static void JSCallCheck(ExtendedAssembler *assembler, Register jsfunc, Register taggedValue, Label *nonCallable, Label *notJSFunction);
        static void CallBuiltinTrampoline(ExtendedAssembler *assembler);
        static void CallOptimized(ExtendedAssembler *assembler);
        static void PushJSFunctionEntryFrame(ExtendedAssembler *assembler, Register prevFp);
        static void PopJSFunctionEntryFrame(ExtendedAssembler *assembler, Register glue);
        static void PushOptimizedArgsConfigFrame(ExtendedAssembler *assembler);
        static void PopOptimizedArgsConfigFrame(ExtendedAssembler *assembler);
        static void PushAsmBridgeFrame(ExtendedAssembler *assembler);
        static void PopOptimizedFrame(ExtendedAssembler *assembler);
        static void PushOptimizedUnfoldArgVFrame(ExtendedAssembler *assembler, Register callSiteSp);
        static void PopOptimizedUnfoldArgVFrame(ExtendedAssembler *assembler);
        static void PushMandatoryJSArgs(ExtendedAssembler *assembler, Register jsfunc,Register thisObj, Register newTarget, Register currentSp);
        static void GenJSCallWithArgV(ExtendedAssembler *assembler, bool needAddExpectedArgs);
        static void PopJSFunctionArgs(ExtendedAssembler *assembler, Register expectedNumArgs, Register actualNumArgs);
        static void JSCallInternal(ExtendedAssembler *assembler, Register jsfunc, bool isNew = false);
        static void IncreaseStackForArguments(ExtendedAssembler *assembler, Register argc, Register currentSp);
        static void JSBoundFunctionCallInternal(ExtendedAssembler *assembler, Register glue, Register actualArgC, Register jsfunc, int stubId);
        static void JSProxyCallInternal(ExtendedAssembler *assembler, Register sp, Register jsfunc);
        static void ThrowNonCallableInternal(ExtendedAssembler *assembler, Register sp);
        
    private:
        friend class OptimizedFastCall;
};     
       
class OptimizedFastCall : public CommonCall 
{
    public:
        static void OptimizedFastCallEntry(ExtendedAssembler *assembler);
        static void OptimizedFastCallAndPushUndefined(ExtendedAssembler* assembler);
        static void JSFastCallWithArgV(ExtendedAssembler *assembler);
        static void JSFastCallWithArgVAndPushUndefined(ExtendedAssembler *assembler);
};

class AsmInterpreterCall : public CommonCall 
{
    public:
        static void AsmInterpreterEntry(ExtendedAssembler*assembler);
        static void GeneratorReEnterAsmInterp(ExtendedAssembler *assembler); 
        static void GeneratorReEnterAsmInterpDispatch(ExtendedAssembler *assembler);
        static void ThrowStackOverflowExceptionAndReturn(ExtendedAssembler *assembler, Register glue,
        Register fp, Register op);
        static void PushCallArgsAndDispatchNative(ExtendedAssembler *assembler); 
        static void PushCallArg0AndDispatch(ExtendedAssembler *assembler); 
        static void PushCallArg1AndDispatch(ExtendedAssembler *assembler); 
        static void PushCallArgs2AndDispatch(ExtendedAssembler *assembler); 
        static void PushCallArgs3AndDispatch(ExtendedAssembler *assembler); 
        static void PushCallThisArg0AndDispatch(ExtendedAssembler *assembler); 
        static void PushCallThisArg1AndDispatch(ExtendedAssembler *assembler); 
        static void PushCallThisArgs2AndDispatch(ExtendedAssembler *assembler); 
        static void PushCallThisArgs3AndDispatch(ExtendedAssembler *assembler); 
        static void PushCallRangeAndDispatch(ExtendedAssembler *assembler); 
        static void PushCallNewAndDispatch(ExtendedAssembler *assembler); 
        static void PushCallNewAndDispatchNative(ExtendedAssembler *assembler); 
        static void PushCallRangeAndDispatchNative(ExtendedAssembler *assembler); 
        static void PushCallThisRangeAndDispatch(ExtendedAssembler *assembler); 
        static void ResumeRspAndDispatch(ExtendedAssembler *assembler); 
        static void ResumeRspAndReturn(ExtendedAssembler *assembler); 
        static void ResumeCaughtFrameAndDispatch(ExtendedAssembler *assembler); 
        static void ResumeUncaughtFrameAndReturn(ExtendedAssembler *assembler); 
        static void ResumeRspAndRollback(ExtendedAssembler *assembler); 
        static void CallSetter(ExtendedAssembler *assembler); 
        static void CallGetter(ExtendedAssembler *assembler); 
        static void CallContainersArgs3(ExtendedAssembler *assembler); 
        static void CallReturnWithArgv(ExtendedAssembler *assembler); 
    private:
        static void DispatchCall(ExtendedAssembler *assembler, Register pc, Register newSp, Register acc = INVALID_REG);
        static void JSCallCommonEntry(ExtendedAssembler *assembler, JSCallMode mode);
        static void PushBuiltinFrame(ExtendedAssembler *assembler, Register glue,
        FrameType type, Register op, Register next);
        static void CallNativeWithArgv(ExtendedAssembler *assembler, bool callNew);
        static void CallNativeInternal(ExtendedAssembler *assembler, Register nativeCode);
        static Register GetThisRegsiter(ExtendedAssembler *assembler, JSCallMode mode, Register defaultRegister);
        static Register GetNewTargetRegsiter(ExtendedAssembler *assembler, JSCallMode mode, Register defaultRegister);
        friend class OptimizedCall;
};

}




#endif  // ECMASCRIPT_COMPILER_RISCV64_EXTENDED_ASSEMBLER_H