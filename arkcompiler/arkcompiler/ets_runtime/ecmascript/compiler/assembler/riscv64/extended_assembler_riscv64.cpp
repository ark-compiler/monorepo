
#include "ecmascript/compiler/assembler/riscv64/extended_assembler_riscv64.h"

#include "ecmascript/frames.h"

namespace panda::ecmascript::riscv64 
{
Register ExtendedAssembler::ghcJSCallDispacherArgs_[JS_CALL_DISPATCHER_ARGS_COUNT] =
    { S0, FP, S3, S4, S5, S6, S7, S8, S9 };
Register ExtendedAssembler::cppJSCallDispacherArgs_[JS_CALL_DISPATCHER_ARGS_COUNT] =
    { ZERO, FP, RA, SP, GP, TP, T0, T1, T2 };

void ExtendedAssembler::BindAssemblerStub(int id)
{
    Label *target = module_->GetFunctionLabel(id);
    Bind(target);
    auto callSigns = module_->GetCSigns();
    auto cs = callSigns[id];
    printf("in BindAssemblerStub GetName:%s\n", callSigns[id]->GetName().c_str());
    isGhcCallingConv_ = cs->GetCallConv() == CallSignature::CallConv::GHCCallConv;
}

void ExtendedAssembler::CalleeSave()
{
    Register sp(SP);
    // 13: 13 means store 13 register into the stack.
    Addi(sp, sp, Immediate(-FRAME_SLOT_SIZE*13));
    Sd(sp, Register(X27), Immediate(FRAME_SLOT_SIZE*12));
    Sd(sp, Register(X26), Immediate(FRAME_SLOT_SIZE*11));
    Sd(sp, Register(X25), Immediate(FRAME_SLOT_SIZE*10));
    Sd(sp, Register(X24), Immediate(FRAME_SLOT_SIZE*9));
    Sd(sp, Register(X23), Immediate(FRAME_SLOT_SIZE*8));
    Sd(sp, Register(X22), Immediate(FRAME_SLOT_SIZE*7));
    Sd(sp, Register(X21), Immediate(FRAME_SLOT_SIZE*6));
    Sd(sp, Register(X20), Immediate(FRAME_SLOT_SIZE*5));
    Sd(sp, Register(X19), Immediate(FRAME_SLOT_SIZE*4));
    Sd(sp, Register(X18), Immediate(FRAME_SLOT_SIZE*3));
    Sd(sp, Register(X9), Immediate(FRAME_SLOT_SIZE*2));
    Sd(sp, Register(X8), Immediate(FRAME_SLOT_SIZE));
    Sd(sp, sp, Immediate(0));
}

// void ExtendedAssembler::CalleeRestore() without VectorRegister restore
void ExtendedAssembler::CalleeRestore()
{
    Register sp(SP);
    Ld(sp, sp, Immediate(0));
    Ld(Register(X8), sp, Immediate(FRAME_SLOT_SIZE));
    Ld(Register(X9), sp, Immediate(FRAME_SLOT_SIZE*2));
    Ld(Register(X18), sp,  Immediate(FRAME_SLOT_SIZE*3));
    Ld(Register(X19), sp,  Immediate(FRAME_SLOT_SIZE*4));
    Ld(Register(X20), sp,  Immediate(FRAME_SLOT_SIZE*5));
    Ld(Register(X21), sp,  Immediate(FRAME_SLOT_SIZE*6));
    Ld(Register(X22), sp,  Immediate(FRAME_SLOT_SIZE*7));
    Ld(Register(X23), sp,  Immediate(FRAME_SLOT_SIZE*8));
    Ld(Register(X24), sp,  Immediate(FRAME_SLOT_SIZE*9));
    Ld(Register(X25), sp,  Immediate(FRAME_SLOT_SIZE*10));
    Ld(Register(X26), sp,  Immediate(FRAME_SLOT_SIZE*11));
    Ld(Register(X27), sp,  Immediate(FRAME_SLOT_SIZE*12));
    Addi(sp, sp, Immediate(FRAME_SLOT_SIZE*13));
}

void ExtendedAssembler::PushFpAndRa()
{
    Register sp(SP);
    Addi(sp, sp, Immediate(-FRAME_SLOT_SIZE*2));
    Sd(sp, Register(FP), Immediate(0)); // Sd(rs1, rs2, imm); note: M[R[rs1]+imm] = R[rs2]
    // TODO. FRAME_SLOT_SIZE: FRAME_SLOT_SIZE means 8 bytes, the size of a register. TODO: Not Sure
    Sd(sp, Register(RA), Immediate(FRAME_SLOT_SIZE));
}

//stub made by group11
void ExtendedAssembler::CallAssemblerStub([[maybe_unused]]int id, [[maybe_unused]]bool isTail)
{

}


void ExtendedAssembler::RestoreFpAndRa()
{
    Register sp(SP);
    Ld(Register(FP), sp, 0);
    Ld(Register(RA), sp, FRAME_SLOT_SIZE);
    Addi(sp, sp, PAIR_SLOT_SIZE);
}

//stub made by group21
void ExtendedAssembler::PushFpAndLr()
{
    
}

//stub made by group21
void ExtendedAssembler::RestoreFpAndLr()
{
    
}

void ExtendedAssembler::Align16(Register fp)
{
    Label aligned;
    //TODO. Two line don't fit to riscv
    //Tst(fp, LogicalImmediate::Create(0xf, RegXSize));  // 0xf: 0x1111
    //B(Condition::EQ, &aligned);
    
    // 8: frame slot size
    Addi(fp, fp, Immediate(-8));
    Bind(&aligned);
}
}  // namespace panda::ecmascript::riscv64
