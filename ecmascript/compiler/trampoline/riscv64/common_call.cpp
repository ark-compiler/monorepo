#include "ecmascript/compiler/trampoline/riscv64/common_call.h"

#include "ecmascript/compiler/assembler/assembler.h"
#include "ecmascript/compiler/argument_accessor.h"
#include "ecmascript/compiler/common_stubs.h"
#include "ecmascript/compiler/rt_call_signature.h"
#include "ecmascript/ecma_runtime_call_info.h"
#include "ecmascript/frames.h"
#include "ecmascript/js_function.h"
#include "ecmascript/js_thread.h"
#include "ecmascript/js_generator_object.h"
#include "ecmascript/message_string.h"
#include "ecmascript/method.h"
#include "ecmascript/runtime_call_id.h"

namespace panda::ecmascript::riscv64 
{
using Label = panda::ecmascript::Label;
#define __ assembler->

void CommonCall::PushAsmInterpBridgeFrame(ExtendedAssembler *assembler)
{
    Register fp(FP);    
    Register sp(SP);    
    
    [[maybe_unused]] TempRegister1Scope scope1(assembler);
    Register frameTypeRegister = __ TempRegister1();

    __ Addi(frameTypeRegister, Register(X0), Immediate(static_cast<int64_t>(FrameType::ASM_INTERPRETER_BRIDGE_FRAME)));

    __ Addi(sp, sp, Immediate(static_cast<int64_t>(-2 * FRAME_SLOT_SIZE))); 
    __ Sd(frameTypeRegister, sp, Immediate(static_cast<int64_t>(0 * FRAME_SLOT_SIZE))); 
    __ Sd(Register(X1), sp,Immediate(static_cast<int64_t>( 1 * FRAME_SLOT_SIZE))); 
    
    
    __ Addi(sp, sp, Immediate(static_cast<int64_t>(-2 * FRAME_SLOT_SIZE)));
    __ Sd(Register(SP),sp, Immediate(static_cast<int64_t>(0 * FRAME_SLOT_SIZE))); 
    __ Sd(fp, sp, Immediate(static_cast<int64_t>(1 * FRAME_SLOT_SIZE))); 
    

    __ Addi(fp, sp, Immediate(static_cast<int64_t>(3 * FRAME_SLOT_SIZE)));
    
    if (!__ FromInterpreterHandler()) {
         __ CalleeSave();
     }

}

void CommonCall::PopAsmInterpBridgeFrame(ExtendedAssembler *assembler)
{
    Register sp(SP);    

    if (!__ FromInterpreterHandler()) {
         __ CalleeSave();
     }


    __ Ld(Register(SP), sp, Immediate(static_cast<int64_t>(0 * FRAME_SLOT_SIZE))); 
    __ Ld(Register(FP), sp, Immediate(static_cast<int64_t>(1 * FRAME_SLOT_SIZE))); 
    __ Addi(sp, sp, 2 * FRAME_SLOT_SIZE); 

    __ Ld(Register(SP), sp, Immediate(static_cast<int64_t>(0 * FRAME_SLOT_SIZE))); 
    __ Ld(Register(X1), sp, Immediate(static_cast<int64_t>(1 * FRAME_SLOT_SIZE))); 
    __ Addi(sp, sp, 2 * FRAME_SLOT_SIZE); 
}

//stub made by group11
void CommonCall::PushArgsWithArgv([[maybe_unused]]ExtendedAssembler *assembler, [[maybe_unused]]Register glue, [[maybe_unused]]Register argc, [[maybe_unused]]Register argv,[[maybe_unused]]Register op, [[maybe_unused]]Register fp, [[maybe_unused]]Label *next, [[maybe_unused]]Label *stackOverflow)
{

}

void CommonCall::PushUndefinedWithArgc([[maybe_unused]]ExtendedAssembler *assembler, [[maybe_unused]]Register glue, [[maybe_unused]]Register argc, [[maybe_unused]]Register temp, [[maybe_unused]]Register currentSlot, [[maybe_unused]]Label *next, [[maybe_unused]]Label *stackOverflow)
{
    
}


void CommonCall::PushLeaveFrame(ExtendedAssembler *assembler, Register glue)
{
    TempRegister2Scope temp2Scope(assembler);
    Register frameType = __ TempRegister2();
    Register currentSp(X16);

    Register sp(SP);

    // construct leave frame
    __ Addi(frameType, Register(X0), Immediate(static_cast<int64_t>(FrameType::LEAVE_FRAME)));

    __ Addi(sp, sp,  Immediate(static_cast<int64_t>(-2 * FRAME_SLOT_SIZE))); 
    __ Sd(Register(FP), sp,  Immediate(static_cast<int64_t>( 0 * FRAME_SLOT_SIZE)));
    __ Sd(Register(X1), sp,  Immediate(static_cast<int64_t>( 1 * FRAME_SLOT_SIZE)));


    // 2 : 2 means pairs
    __ Addi(sp, sp,  Immediate(static_cast<int64_t>(-2 * FRAME_SLOT_SIZE))); 
    __ Sd(Register(S1), sp,  Immediate(static_cast<int64_t>(0 * FRAME_SLOT_SIZE)));
    __ Sd(frameType, sp,  Immediate(static_cast<int64_t>(1 * FRAME_SLOT_SIZE)));
    

    __ Addi(Register(FP), sp, Immediate(static_cast<int64_t>(DOUBLE_SLOT_SIZE)));

    __ Sd(Register(FP), glue,  Immediate(static_cast<int64_t>(JSThread::GlueData::GetLeaveFrameOffset(false))));
    __ Addi(sp, sp,  Immediate(static_cast<int64_t>(JSThread::GlueData::GetLeaveFrameOffset(false)))); 
}



void CommonCall::PopLeaveFrame(ExtendedAssembler *assembler)
{
    Register sp(SP);
    Register currentSp(X17);
    TempRegister2Scope temp2Scope(assembler);
    Register frameType = __ TempRegister2();
    __ Ld(sp, currentSp, Immediate(static_cast<int64_t>(0 * FRAME_SLOT_SIZE)));
    __ Ld(frameType, sp, Immediate(static_cast<int64_t>(1 * FRAME_SLOT_SIZE)));
    __ Addi(sp, sp, 2 * FRAME_SLOT_SIZE);

    __ Ld(Register(FP), sp, Immediate(static_cast<int64_t>(0 * FRAME_SLOT_SIZE)));
    __ Ld(Register(X1), sp, Immediate(static_cast<int64_t>(1 * FRAME_SLOT_SIZE)));
    __ Addi(sp, sp, 2 * FRAME_SLOT_SIZE);
}

}// end panda::ecmascript::riscv64
