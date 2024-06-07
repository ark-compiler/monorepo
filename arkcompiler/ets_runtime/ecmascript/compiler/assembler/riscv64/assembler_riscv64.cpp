#include <cstdint>
#include <map>

#include "ecmascript/compiler/assembler/riscv64/assembler_riscv64.h"

#include "ecmascript/base/bit_helper.h"
#include "ecmascript/ecma_macros.h"

namespace panda::ecmascript::riscv64 {
using namespace panda::ecmascript::base;

//stub made by group 13
LogicalImmediate LogicalImmediate::Create([[maybe_unused]]uint64_t imm, [[maybe_unused]]int width){
	return LogicalImmediate(0);
}
void AssemblerRiscv64::Add([[maybe_unused]]const Register &rd, [[maybe_unused]]const Register &rn, [[maybe_unused]]const Operand &operand)
{
}
void AssemblerRiscv64::Sub([[maybe_unused]]const Register &rd, [[maybe_unused]]const Register &rn, [[maybe_unused]]const Operand &operand)
{
}
void AssemblerRiscv64::Andi([[maybe_unused]]const Register &rd, [[maybe_unused]]const Register &rn, [[maybe_unused]]const LogicalImmediate &imm)
{
}
void AssemblerRiscv64::Bind(Label *target)
{
    size_t pos = GetCurrentPosition();
    ASSERT(!target->IsBound());

    if (target->IsLinked()) {
        uint32_t linkPos = target->GetLinkedPos();
        while (linkPos != 0) {
            uint32_t preLinkPos = GetU32(linkPos);
            int32_t disp = static_cast<int32_t>(pos - linkPos - sizeof(int32_t));
            PutI32(linkPos, disp);
            linkPos = preLinkPos;
        }
    }

    if (target->IsLinkedNear()) {
        uint32_t linkPos = target->GetLinkedNearPos();
        while (linkPos != 0) {
            int8_t offsetToNext = GetI8(static_cast<size_t>(linkPos));
            int32_t disp = static_cast<int32_t>(pos - linkPos - sizeof(int8_t));
            ASSERT(InRange8(disp));
            PutI8(linkPos, static_cast<int8_t>(disp));
            if (offsetToNext == 0) {
                break;
            }
            linkPos += static_cast<uint32_t>(offsetToNext);
        }
        target->UnlinkNearPos();
    }

    target->BindTo(pos);
}
void AssemblerRiscv64::Jalr(const Register &rd, const Register &rs1,
                            const Immediate &imm12) {
    uint32_t code = 0;
    code = (imm12.Value() << I_TYPE_imm12_LOWBITS) | Rd(rd.GetId()) |
           funct3Jalr | Rs1(rs1.GetId()) | opCodeJalr;
    EmitU32(code);
}

void AssemblerRiscv64::Jr()
{

}

void AssemblerRiscv64::Lb(const Register &rd, const Register &rs1,
                          const Immediate &imm12) {
    uint32_t code = 0;
    code = (imm12.Value() << I_TYPE_imm12_LOWBITS) | Rd(rd.GetId()) | funct3Lb |
           Rs1(rs1.GetId()) | opCodeILoad;
    EmitU32(code);
}

void AssemblerRiscv64::Lh(const Register &rd, const Register &rs1,
                          const Immediate &imm12) {
    uint32_t code = 0;
    code = (imm12.Value() << I_TYPE_imm12_LOWBITS) | Rd(rd.GetId()) | funct3Lh |
           Rs1(rs1.GetId()) | opCodeILoad;
    EmitU32(code);
}

void AssemblerRiscv64::Lw(const Register &rd, const Register &rs1,
                          const Immediate &imm12) {
    uint32_t code = 0;
    code = (imm12.Value() << I_TYPE_imm12_LOWBITS) | Rd(rd.GetId()) | funct3Lw |
           Rs1(rs1.GetId()) | opCodeILoad;
    EmitU32(code);
}

void AssemblerRiscv64::Lbu(const Register &rd, const Register &rs1,
                           const Immediate &imm12) {
    uint32_t code = 0;
    code = (imm12.Value() << I_TYPE_imm12_LOWBITS) | Rd(rd.GetId()) |
           funct3Lbu | Rs1(rs1.GetId()) | opCodeILoad;
    EmitU32(code);
}

void AssemblerRiscv64::Lhu(const Register &rd, const Register &rs1,
                           const Immediate &imm12) {
    uint32_t code = 0;
    code = (imm12.Value() << I_TYPE_imm12_LOWBITS) | Rd(rd.GetId()) |
           funct3Lhu | Rs1(rs1.GetId()) | opCodeILoad;
    EmitU32(code);
}

void AssemblerRiscv64::Addi(const Register &rd, const Register &rs1,
                            const Immediate &imm12) {
    uint32_t code = 0;
    code = (imm12.Value() << I_TYPE_imm12_LOWBITS) | Rd(rd.GetId()) |
           funct3Addi | Rs1(rs1.GetId()) | opCodeI;
    EmitU32(code);
}

void AssemblerRiscv64::Slti(const Register &rd, const Register &rs1,
                            const Immediate &imm12) {
    uint32_t code = 0;
    code = (imm12.Value() << I_TYPE_imm12_LOWBITS) | Rd(rd.GetId()) |
           funct3Slti | Rs1(rs1.GetId()) | opCodeI;
    EmitU32(code);
}

void AssemblerRiscv64::Sltiu(const Register &rd, const Register &rs1,
                             const Immediate &imm12) {
    uint32_t code = 0;
    code = (imm12.Value() << I_TYPE_imm12_LOWBITS) | Rd(rd.GetId()) |
           funct3Sltiu | Rs1(rs1.GetId()) | opCodeI;
    EmitU32(code);
}

void AssemblerRiscv64::Xori(const Register &rd, const Register &rs1,
                            const Immediate &imm12) {
    uint32_t code = 0;
    code = (imm12.Value() << I_TYPE_imm12_LOWBITS) | Rd(rd.GetId()) |
           funct3Xori | Rs1(rs1.GetId()) | opCodeI;
    EmitU32(code);
}

void AssemblerRiscv64::Ori(const Register &rd, const Register &rs1,
                           const Immediate &imm12) {
    uint32_t code = 0;
    code = (imm12.Value() << I_TYPE_imm12_LOWBITS) | Rd(rd.GetId()) |
           funct3Ori | Rs1(rs1.GetId()) | opCodeI;
    EmitU32(code);
}

void AssemblerRiscv64::Andi(const Register &rd, const Register &rs1,
                            const Immediate &imm12) {
    uint32_t code = 0;
    code = (imm12.Value() << I_TYPE_imm12_LOWBITS) | Rd(rd.GetId()) |
           funct3Andi | Rs1(rs1.GetId()) | opCodeI;
    EmitU32(code);
}

void AssemblerRiscv64::Lwu(const Register &rd, const Register &rs1,
                           const Immediate &imm12) {
    uint32_t code = 0;
    code = (imm12.Value() << I_TYPE_imm12_LOWBITS) | Rd(rd.GetId()) |
           funct3Lwu | Rs1(rs1.GetId()) | opCodeILoad;
    EmitU32(code);
}

void AssemblerRiscv64::Ld(const Register &rd, const Register &rs1,
                          const Immediate &imm12) {
    uint32_t code = 0;
    code = (imm12.Value() << I_TYPE_imm12_LOWBITS) | Rd(rd.GetId()) | funct3Ld |
           Rs1(rs1.GetId()) | opCodeILoad;
    EmitU32(code);
}

void AssemblerRiscv64::Addiw(const Register &rd, const Register &rs1,
                             const Immediate &imm12) {
    uint32_t code = 0;
    code = (imm12.Value() << I_TYPE_imm12_LOWBITS) | Rd(rd.GetId()) |
           funct3Addiw | Rs1(rs1.GetId()) | opCodeW;
    EmitU32(code);
}
void AssemblerRiscv64::Blr(const Register &rn)//TODO. unfinished
{
    uint32_t code = Rd(rn.GetId());;
    EmitU32(code);
}

void AssemblerRiscv64::Br([[maybe_unused]]const Register &rn)//stub made by group21
{
    
}

void AssemblerRiscv64::Ret()
{
    Ret(Register(RA));//X30 in Arrch 
}

void AssemblerRiscv64::Ret(const Register &rn)//TODO. unfinished
{
    uint32_t code = Rd(rn.GetId());;
    EmitU32(code);
}
int32_t AssemblerRiscv64::LinkAndGetInstOffsetToLabel(Label *label)
{
    int32_t offset = 0;
    if (label->IsBound()) {
        offset = static_cast<int32_t>(label->GetPos() - GetCurrentPosition());
    } else {
        if (label->IsLinked()) {
            offset = static_cast<int32_t>(label->GetLinkedPos() - GetCurrentPosition());
        } else {
            offset = 0;
        }
        label->LinkTo(GetCurrentPosition());
    }
    return offset;
}

EMIT_INSTS
} // panda::ecmascript::riscv64
