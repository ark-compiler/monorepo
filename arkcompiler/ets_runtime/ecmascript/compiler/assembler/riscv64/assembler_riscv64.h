/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ECMASCRIPT_COMPILER_ASSEMBLER_RISCV64_H
#define ECMASCRIPT_COMPILER_ASSEMBLER_RISCV64_H

#include "ecmascript/compiler/assembler/assembler.h"
#include "ecmascript/compiler/assembler/riscv64/assembler_riscv64_constants.h"

namespace panda::ecmascript::riscv64 {

enum RegisterWidth {
    XLEN_32 = 32,  // 32bits
    XLEN_64 = 64   // 64
};

class Register {
public:
    //TODO NOT SURE
    Register(RegisterId reg, RegisterWidth width) : reg_(reg), width_(width) {}
    Register(RegisterId reg) : reg_(reg) {};

    RegisterWidth getWidth() const {
        return width_;
    }

    inline RegisterId GetId() const
    {
        return reg_;
    }
    inline bool IsValid() const
    {
        return reg_ != RegisterId::INVALID_REG;
    }
    inline bool operator !=(const Register &other)
    {
        return reg_ != other.GetId();
    }
    inline bool operator ==(const Register &other)
    {
        return reg_ == other.GetId();
    }
private:
    RegisterId reg_;
    RegisterWidth width_;
};

class Immediate {
public:
    Immediate(int32_t value) : value_(value) {}
    ~Immediate() = default;

    int32_t Value() const
    {
        return value_;
    }
private:
    int32_t value_;
};

// add by group 13
class LogicalImmediate {
public:
    static LogicalImmediate Create(uint64_t imm, int width);
    int Value() const
    {
        ASSERT(IsValid());
        return imm_;
    }

    bool IsValid() const
    {
        return imm_ != InvalidLogicalImmediate;
    }
    // TODO NOT SURE
private:
    explicit LogicalImmediate(int value)
        : imm_(value)
    {
    }
    static const int InvalidLogicalImmediate = -1;
    int imm_;
};

class Operand {
public:
    Operand(Immediate imm)
        : reg_(RegisterId::INVALID_REG), extend_(Extend::NO_EXTEND), shift_(Shift::NO_SHIFT),
          shiftAmount_(0), immediate_(imm)
    {
    }
    Operand(Register reg, Shift shift = Shift::LSL, uint8_t shift_amount = 0)
        : reg_(reg), extend_(Extend::NO_EXTEND), shift_(shift), shiftAmount_(shift_amount), immediate_(0)
    {
    }
    Operand(Register reg, Extend extend, uint8_t shiftAmount = 0)
        : reg_(reg), extend_(extend), shift_(Shift::NO_SHIFT), shiftAmount_(shiftAmount), immediate_(0)
    {
    }
    ~Operand() = default;

    inline bool IsImmediate() const
    {
        return !reg_.IsValid();
    }

    inline bool IsShifted() const
    {
        return reg_.IsValid() && shift_ != Shift::NO_SHIFT;
    }

    inline bool IsExtended() const
    {
        return reg_.IsValid() && extend_ != Extend::NO_EXTEND;
    }

    inline Register Reg() const
    {
        return reg_;
    }

    inline Shift GetShiftOption() const
    {
        return shift_;
    }

    inline Extend GetExtendOption() const
    {
        return extend_;
    }

    inline uint8_t GetShiftAmount() const
    {
        return shiftAmount_;
    }

    inline int64_t ImmediateValue() const
    {
        return immediate_.Value();
    }

    inline Immediate GetImmediate() const
    {
        return immediate_;
    }
private:
    Register reg_;
    Extend  extend_;
    Shift  shift_;
    uint8_t  shiftAmount_;
    Immediate immediate_;
};

class AssemblerRiscv64 : public Assembler {
public:
    explicit AssemblerRiscv64(Chunk *chunk)
        : Assembler(chunk)
    {
    }
    // Integer Register-Register Operations (R-types)
    void Add(const Register &rd, const Register &rs1, const Register &rs2);
    void Addw(const Register &rd, const Register &rs1, const Register &rs2);
    void Sub(const Register &rd, const Register &rs1, const Register &rs2);
    void Subw(const Register &rd, const Register &rs1, const Register &rs2);
    void Slt(const Register &rd, const Register &rs1, const Register &rs2);
    void Sltu(const Register &rd, const Register &rs1, const Register &rs2);
    void Sll(const Register &rd, const Register &rs1, const Register &rs2);
    void Sllw(const Register &rd, const Register &rs1, const Register &rs2);
    void Srl(const Register &rd, const Register &rs1, const Register &rs2);
    void Srlw(const Register &rd, const Register &rs1, const Register &rs2);
    void Sra(const Register &rd, const Register &rs1, const Register &rs2);
    void Sraw(const Register &rd, const Register &rs1, const Register &rs2);
    void Xor(const Register &rd, const Register &rs1, const Register &rs2);
    void Or(const Register &rd, const Register &rs1, const Register &rs2);
    void And(const Register &rd, const Register &rs1, const Register &rs2);

    // add by group 13 overloaded function 
    void Add(const Register &rd, const Register &rn, const Operand &operand);
    void Sub(const Register &rd, const Register &rn, const Operand &operand);
    
    // Integer Register-Register Operations (B-types)
    void Beq(const Register &rs1, const Register &rs2,const Immediate& imm12);
    void Bne(const Register &rs1, const Register &rs2,const Immediate& imm12);
    void Blt(const Register &rs1, const Register &rs2,const Immediate& imm12);
    void Bge(const Register &rs1, const Register &rs2,const Immediate& imm12);
    void Bltu(const Register &rs1, const Register &rs2,const Immediate& imm12);
    void Bgeu(const Register &rs1, const Register &rs2,const Immediate& imm12);

    // Integer Store Operations (S-types)
    void Sb(const Register &rs1, const Register &rs2, const Immediate& imm12);
    void Sh(const Register &rs1, const Register &rs2, const Immediate& imm12);
    void Sw(const Register &rs1, const Register &rs2, const Immediate& imm12);
    void Sd(const Register &rs1, const Register &rs2, const Immediate& imm12);

    // Immdiately Operations (I-types)
    void Lb(const Register &rd, const Register &rs1, const Immediate &imm12);
    void Jalr(const Register &rd, const Register &rs1, const Immediate &imm12);
    void Lh(const Register &rd, const Register &rs1, const Immediate &imm12);
    void Lw(const Register &rd, const Register &rs1, const Immediate &imm12);
    void Lbu(const Register &rd, const Register &rs1, const Immediate &imm12);
    void Lhu(const Register &rd, const Register &rs1, const Immediate &imm12);
    void Addi(const Register &rd, const Register &rs1, const Immediate &imm12);
    void Slti(const Register &rd, const Register &rs1, const Immediate &imm12);
    void Sltiu(const Register &rd, const Register &rs1, const Immediate &imm12);
    void Xori(const Register &rd, const Register &rs1, const Immediate &imm12);
    void Ori(const Register &rd, const Register &rs1, const Immediate &imm12);
    void Andi(const Register &rd, const Register &rs1, const Immediate &imm12);
    void Lwu(const Register &rd, const Register &rs1, const Immediate &imm12);
    void Ld(const Register &rd, const Register &rs1, const Immediate &imm12);
    void Addiw(const Register &rd, const Register &rs1, const Immediate &imm12);

    // add by group 13 overloaded function
    void Andi(const Register &rd, const Register &rn, const LogicalImmediate &imm);

    // Integer Register-Immediate Operations (U-types)
    void Lui(const Register &rd, const Immediate& imm20);
    void Auipc(const Register &rd, const Immediate& imm20);

    // Jump Operations (J-types)
    void Jal(const Register &rd, const Immediate& imm20);
    void Bind(Label *target);
    //TO DO  Jr()
    void Jr();
    
    //TODO Pseudo Operations
    void Blr(const Register &rn);
    void Br(const Register &rn);
    void Ret();
    void Ret(const Register &rn);
    int32_t LinkAndGetInstOffsetToLabel(Label *label);
private:
    // R_TYPE field defines
    inline uint32_t Rd(uint32_t id){
        return (id << R_TYPE_rd_LOWBITS) & R_TYPE_rd_MASK;
    }
    inline uint32_t Rs1(uint32_t id){
        return (id << R_TYPE_rs1_LOWBITS) & R_TYPE_rs1_MASK;
    }
    inline uint32_t Rs2(uint32_t id){
        return (id << R_TYPE_rs2_LOWBITS) & R_TYPE_rs2_MASK;
    }
    
};
} // namespace panda::ecmascript::riscv64
#endif
