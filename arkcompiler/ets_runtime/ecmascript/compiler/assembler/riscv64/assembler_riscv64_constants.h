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

#ifndef ECMASCRIPT_COMPILER_ASSEMBLER_RISCV64_CONSTANTS_H
#define ECMASCRIPT_COMPILER_ASSEMBLER_RISCV64_CONSTANTS_H
namespace panda::ecmascript::riscv64 {
enum RegisterId : uint8_t {
    X0, X1, SP, X3, X4, X5, X6, X7,
    X8, X9, X10, X11, X12, X13, X14, X15,
    X16, X17, X18, X19, X20, X21, X22, X23,
    X24, X25, X26, X27, X28, X29, X30, X31,
    ZERO = X0,
    RA = X1,
    GP = X3,
    TP = X4,
    T0 = X5,
    T1 = X6,
    T2 = X7,
    S0 = X8,
    FP = X8,
    S1 = X9,
    A0 = X10,
    A1 = X11,
    A2 = X12,
    A3 = X13,
    A4 = X14,
    A5 = X15,
    A6 = X16,
    A7 = X17,
    S2 = X18,
    S3 = X19,
    S4 = X20,
    S5 = X21,
    S6 = X22,
    S7 = X23,
    S8 = X24,
    S9 = X25,
    S10 = X26,
    S11 = X27,
    T3 = X28,
    T4 = X29,
    T5 = X30,
    T6 = X31,
    INVALID_REG = 0xFF,
};
enum opCode {
    opCodeI = 0x13,       // SLLI,SRLI,SRAI,ADDI,SLTI,SLTIU,XORI,ORI,ANDI,SLLI,SRLI,SRAI,
    opCodeILoad = 0x03,   // LWU,LD,LB,LH,LW,LBU,LHU
    opCodeR = 0x33,       // ADD,SUB,SLL,SLT,SLTU,XOR,SRL,SRA,OR,AND
    opCodeW = 0x1b,       // ADDW,SUBW,SLLW,SRLW,SRAW
    opCodeS = 0x23,       // SD,SB,SH,SW
    opCodeB = 0x63,       // BEQ,BNE,BLT,BGE,BLTU,BGEU
    opCodeULui = 0x37,    // LUI
    opCodeUAuipc = 0x17,  // AUIPC
    opCodeJal = 0x6f,     // JAL
    opCodeJalr = 0x67,    // JALR
    opCodeIF = 0X0f,      // FENCE,FENCE.I
    opCodeIE = 0x73,      // ECALL,EBREA,
    opCodeIC = 0x73,      // CSRRW,CSRRS,CSRRC,CSRRWI,CSRRSI,CSRRCI,ADDIW,SLLIW,SRLIW,SRAIW
    opCodeIA = 0x3b       // ADDW,SUBW,SLLW,SRLW,SRAW
};

enum funct3 {
    funct3Slli = 0x1000,
    funct3Srli = 0x5000,
    funct3Srai = 0x5000,
    funct3Add = 0x0,
    funct3Sub = 0x0,
    funct3Sll = 0x1000,
    funct3Slt = 0x2000,
    funct3Sltu = 0x3000,
    funct3Xor = 0x4000,
    funct3Srl = 0x5000,
    funct3Addi = 0x0,
    funct3Slti = 0x2000,
    funct3Sltiu = 0x3000,
    funct3Xori = 0x4000,
    funct3Andi = 0x7000,
    funct3Ori = 0x6000,
    funct3Lb = 0x0,
    funct3Lh = 0x1000,
    funct3Lw = 0x2000,
    funct3Lbu = 0x4000,
    funct3Lhu = 0x5000,
    funct3Sb = 0x0,
    funct3Sh = 0x1000,
    funct3Sw = 0x2000,
    funct3Beq = 0x0,
    funct3Bne = 0x1000,
    funct3Blt = 0x4000,
    funct3Bge = 0x5000,
    funct3Bltu = 0x6000,
    funct3Ogeu = 0x7000,
    funct3Jalr = 0x0,
    funct3Lwu = 0x6000,
    funct3Ld = 0x3000,
    funct3Addiw = 0x0,
};
enum RegisterType {
    W = 0,  /* a word for 32 bits */
    D = 1,  /* a double-word for 64 bits */
};

static const int RegDSize = 64;
static const int RegWSize = 32;

// add by group 13
enum Extend : uint8_t {
    NO_EXTEND = 0xFF,
    UXTB = 0,   /* zero extend to byte */
    UXTH = 1,   /* zero extend to half word */
    UXTW = 2,   /* zero extend to word */
    UXTX = 3,   /* zero extend to 64bit */
    SXTB = 4,   /* sign extend to byte */
    SXTH = 5,   /* sign extend to half word */
    SXTW = 6,   /* sign extend to word */
    SXTX = 7,   /* sign extend to 64bit */
};

enum Shift : uint8_t {
    NO_SHIFT = 0xFF,
    LSL = 0x0,
    LSR = 0x1,
    ASR = 0x2,
    ROR = 0x3,
    MSL = 0x4,
};

enum AddSubOpFunct {
    ADD     = 0x00000033,
    ADDW    = 0x0000003b,
    SUB     = 0x40000033,
    SUBW    = 0x4000003b,
    SLT     = 0x00002033,
    SLTU    = 0x00003033,
};

enum ShiftOpFunct {
    SLL     = 0x00001033,
    SLLW    = 0x0000103b,
    SRL     = 0x00005033,
    SRLW    = 0x0000503b,
    SRA     = 0x40005033,
    SRAW    = 0x4000503b,
};

enum BitwiseOpFunct {
    XOR = 0x00004033,
    OR  = 0x00006033,
    AND = 0x00007033,
};

enum BOpFunct{
    BEQ  = 0x00000063,
    BNE  = 0x00001063,
    BLT  = 0x00004063,  
    BGE  = 0x00005063,
    BLTU = 0x00006063,
    BGEU = 0x00007063,
};

enum StoreOpFunct {
    SB = 0x00000023,
    SH = 0x00001023,
    SW = 0x00002023,
    SD = 0x00003023,
};

enum UOpFunct {
    LUI = 0x00000037,
    AUIPC = 0x00000017,
};

enum JOpFunct {
    JAL = 0x0000006f,
};

#define R_TYPE_FIELD_LIST(V)    \
    V(R_TYPE, opcode,  6,  0)   \
    V(R_TYPE,     rd, 11,  7)   \
    V(R_TYPE, funct3, 14, 12)   \
    V(R_TYPE,    rs1, 19, 15)   \
    V(R_TYPE,    rs2, 24, 20)   \
    V(R_TYPE, funct7, 31, 25)
    
#define B_TYPE_FIELD_LIST(V)    \
    V(B_TYPE, opcode,  6,  0)   \
    V(B_TYPE,   imm1, 11,  7)   \
    V(B_TYPE, funct3, 14, 12)   \
    V(B_TYPE,    rs1, 19, 15)   \
    V(B_TYPE,    rs2, 24, 20)   \
    V(B_TYPE,   imm2, 31, 25)

#define S_TYPE_FIELD_LIST(V)    \
    V(S_TYPE, opcode,  6,  0)   \
    V(S_TYPE,   imm5, 11,  7)   \
    V(S_TYPE, funct3, 14, 12)   \
    V(S_TYPE,    rs1, 19, 15)   \
    V(S_TYPE,    rs2, 24, 20)   \
    V(S_TYPE,   imm7, 31, 25)

#define I_TYPE_FIELD_LIST(V)  \
    V(I_TYPE, opcode, 6, 0)   \
    V(I_TYPE, rd, 11, 7)      \
    V(I_TYPE, funct3, 14, 12) \
    V(I_TYPE, rs1, 19, 15)    \
    V(I_TYPE, imm12, 31, 20)

#define U_TYPE_FIELD_LIST(V) 	\
  	V(U_TYPE, imm20, 31, 12) 	\
  	V(U_TYPE,    rd, 11,  7) 	\
  	V(U_TYPE,opcode,  6,  0)  	

#define J_TYPE_FIELD_LIST(V) 	\
  	V(J_TYPE, imm20, 31, 12)	\
    V(J_TYPE,    rd, 11,  7) 	\
  	V(J_TYPE,opcode,  6,  0)  	

#define DECL_FIELDS_IN_INSTRUCTION(INSTNAME, FIELD_NAME, HIGHBITS, LOWBITS) \
static const uint32_t INSTNAME##_##FIELD_NAME##_HIGHBITS = HIGHBITS;  \
static const uint32_t INSTNAME##_##FIELD_NAME##_LOWBITS = LOWBITS;    \
static const uint32_t INSTNAME##_##FIELD_NAME##_WIDTH = ((HIGHBITS - LOWBITS) + 1); \
static const uint32_t INSTNAME##_##FIELD_NAME##_MASK = (((1 << INSTNAME##_##FIELD_NAME##_WIDTH) - 1) << LOWBITS);

#define DECL_INSTRUCTION_FIELDS(V)  \
    R_TYPE_FIELD_LIST(V) \
    B_TYPE_FIELD_LIST(V) \
    S_TYPE_FIELD_LIST(V) \
    U_TYPE_FIELD_LIST(V) \
    J_TYPE_FIELD_LIST(V) \
    I_TYPE_FIELD_LIST(V)

DECL_INSTRUCTION_FIELDS(DECL_FIELDS_IN_INSTRUCTION)
#undef DECL_INSTRUCTION_FIELDS

#define EMIT_INSTS \
    EMIT_R_TYPE_INSTS(EMIT_R_TYPE_INST) \
    EMIT_B_TYPE_INSTS(EMIT_B_TYPE_INST) \
    EMIT_S_TYPE_INSTS(EMIT_S_TYPE_INST) \
    EMIT_U_TYPE_INSTS(EMIT_U_TYPE_INST) \
    EMIT_J_TYPE_INSTS(EMIT_J_TYPE_INST)

#define EMIT_R_TYPE_INSTS(V) \
    V( Add,  ADD)            \
    V(Addw, ADDW)            \
    V( Sub,  SUB)            \
    V(Subw, SUBW)            \
    V( Slt,  SLT)            \
    V(Sltu, SLTU)            \
    V( Sll,  SLL)            \
    V(Sllw, SLLW)            \
    V( Srl,  SRL)            \
    V(Srlw, SRLW)            \
    V( Sra,  SRA)            \
    V(Sraw, SRAW)            \
    V( Xor,  XOR)            \
    V(  Or,   OR)            \
    V( And,  AND)            

#define EMIT_B_TYPE_INSTS(V) \
    V(Beq, BEQ)              \
    V(Bne, BNE)              \
    V(Blt, BLT)              \
    V(Bge, BGE)              \
    V(Bltu, BLTU)            \
    V(Bgeu, BGEU)

#define EMIT_S_TYPE_INSTS(V) \
    V(  Sb,   SB)            \
    V(  Sh,   SH)            \
    V(  Sw,   SW)            \
    V(  Sd,   SD)

#define EMIT_U_TYPE_INSTS(V) \
    V(Lui, LUI)              \
    V(Auipc, AUIPC)          

#define EMIT_J_TYPE_INSTS(V) \
    V(Jal, JAL)

#define EMIT_R_TYPE_INST(INSTNAME, INSTID) \
void AssemblerRiscv64::INSTNAME(const Register &rd, const Register &rs1, const Register &rs2) \
{ \
    uint32_t rd_id = Rd(rd.GetId()); \
    uint32_t rs1_id = Rs1(rs1.GetId()); \
    uint32_t rs2_id = Rs2(rs2.GetId()); \
    uint32_t code = rd_id | rs1_id | rs2_id | INSTID; \
    EmitU32(code); \
}

#define B_TYPE_Imm_Left_High 31
#define B_TYPE_Imm_Left_Low 25
#define B_TYPE_Imm_Right_High 11
#define B_TYPE_Imm_Right_Low 7
#define EMIT_B_TYPE_INST(INSTNAME, INSTID) \
void AssemblerRiscv64::INSTNAME(const Register &rs1, const Register &rs2, const Immediate& imm12) \
{ \
    uint32_t rs1_id = Rs1(rs1.GetId()); \
    uint32_t rs2_id = Rs2(rs2.GetId()); \
    uint32_t imm = ((imm12.Value() & 0x1000) << (B_TYPE_Imm_Left_High - 12)) /*[12] -> [31]*/\
                  | ((imm12.Value() & 0x07E0) << (B_TYPE_Imm_Left_Low - 5)) /*[10:5] -> [30:25] */ \
                  | ((imm12.Value() & 0x001E) << (B_TYPE_Imm_Right_High - 4)) /*[4:1] -> [11:8] */ \
                  | ((imm12.Value() & 0x0800) >> (11 - B_TYPE_Imm_Right_Low));/*[11] -> [7] */ \
    uint32_t code = imm | rs1_id | rs2_id | INSTID; \
    EmitU32(code); \
}

#define S_TYPE_Imm_Left_Low 25
#define S_TYPE_Imm_Right_Low 7
#define EMIT_S_TYPE_INST(INSTNAME, INSTID) \
void AssemblerRiscv64::INSTNAME(const Register &rs1, const Register &rs2, const Immediate& imm12) \
{ \
    uint32_t imm12_id = (((imm12.Value() & 0xFE0) << (S_TYPE_Imm_Left_Low - 5)) /*[11:5]*/ \
                  | (imm12.Value() & 0x1F)) << S_TYPE_Imm_Right_Low; /*[4:0]*/ \
    uint32_t rs1_id = Rs1(rs1.GetId()); \
    uint32_t rs2_id = Rs2(rs2.GetId()); \
    uint32_t code = imm12_id | rs1_id | rs2_id | INSTID; \
    EmitU32(code); \
}

#define EMIT_U_TYPE_INST(INSTNAME, INSTID) \
void AssemblerRiscv64::INSTNAME(const Register &rd, const Immediate& imm20) \
{ \
	uint32_t code = 0; \
	code = (imm20.Value() << U_TYPE_imm20_LOWBITS)  | Rd(rd.GetId()) | INSTID; \
	EmitU32(code); \
}

#define EMIT_J_TYPE_INST(INSTNAME, INSTID) \
void AssemblerRiscv64::INSTNAME(const Register &rd, const Immediate& imm20) \
{ \
    uint32_t code = 0; \
    uint32_t imm = ((imm20.Value() & 0x80000)     /*[20]*/    \
          | (imm20.Value() & 0x3FF) << 9          /*[10:1]*/  \
          | (imm20.Value() & 0x400) >> 2          /*[11]*/    \
          | (imm20.Value() & 0x7F800) >> 11)      /*[19:12]*/ \
					>> 1; \
	code = (imm << J_TYPE_imm20_LOWBITS) | Rd(rd.GetId()) | INSTID; \
	EmitU32(code); \
}

};  // namespace panda::ecmascript::riscv64
#endif
