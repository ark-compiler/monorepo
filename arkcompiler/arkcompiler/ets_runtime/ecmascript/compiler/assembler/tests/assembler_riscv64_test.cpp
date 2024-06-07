/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "ecmascript/compiler/assembler/riscv64/assembler_riscv64.h"

#include <ostream>
#include <sstream>

#include "ecmascript/compiler/assembler/assembler.h"
#include "ecmascript/ecma_vm.h"
#include "ecmascript/mem/dyn_chunk.h"
#include "ecmascript/tests/test_helper.h"

#include "llvm-c/Analysis.h"
#include "llvm-c/Core.h"
#include "llvm-c/Disassembler.h"
#include "llvm-c/ExecutionEngine.h"
#include "llvm-c/Target.h"

namespace panda::test {
using namespace panda::ecmascript;
using namespace panda::ecmascript::riscv64;
class AssemblerRiscv64Test : public testing::Test {
public:
    static void SetUpTestCase()
    {
        GTEST_LOG_(INFO) << "SetUpTestCase";
    }

    static void TearDownTestCase()
    {
        GTEST_LOG_(INFO) << "TearDownCase";
    }

    void SetUp() override
    {
        InitializeLLVM(TARGET_RISCV64);
        TestHelper::CreateEcmaVMWithScope(instance, thread, scope);
        chunk_ = thread->GetEcmaVM()->GetChunk();
    }

    void TearDown() override
    {
        TestHelper::DestroyEcmaVMWithScope(instance, scope);
    }

    static const char *SymbolLookupCallback([[maybe_unused]] void *disInfo, [[maybe_unused]] uint64_t referenceValue,
                                            uint64_t *referenceType, [[maybe_unused]] uint64_t referencePC,
                                            [[maybe_unused]] const char **referenceName)
    {
        *referenceType = LLVMDisassembler_ReferenceType_InOut_None;
        return nullptr;
    }

    void InitializeLLVM(std::string triple)
    {
        if (triple.compare(TARGET_RISCV64) == 0) {
            LLVMInitializeRISCVTargetInfo();
            LLVMInitializeRISCVTargetMC();
            LLVMInitializeRISCVDisassembler();
            LLVMInitializeRISCVAsmPrinter();
            LLVMInitializeRISCVAsmParser();
            LLVMInitializeRISCVTarget();
        } else {
            LOG_ECMA(FATAL) << "this branch is unreachable";
            UNREACHABLE();
        }
    }

    void DisassembleChunk(const char *triple, Assembler *assemlber, std::ostream &os)
    {
        LLVMDisasmContextRef dcr = LLVMCreateDisasm(triple, nullptr, 0, nullptr, SymbolLookupCallback);
        uint8_t *byteSp = assemlber->GetBegin();
        size_t numBytes = assemlber->GetCurrentPosition();
        unsigned pc = 0;
        const char outStringSize = 100;
        char outString[outStringSize];
        while (numBytes > 0) {
            size_t InstSize = LLVMDisasmInstruction(dcr, byteSp, numBytes, pc, outString, outStringSize);
            if (InstSize == 0) {
                // 8 : 8 means width of the pc offset and instruction code
                os << std::setw(8) << std::setfill('0') << std::hex << pc << ":" << std::setw(8)
                   << *reinterpret_cast<uint32_t *>(byteSp) << "maybe constant" << std::endl;
                pc += 4; // 4 pc length
                byteSp += 4; // 4 sp offset
                numBytes -= 4; // 4 num bytes
            }
            // 8 : 8 means width of the pc offset and instruction code
            os << std::setw(8) << std::setfill('0') << std::hex << pc << ":" << std::setw(8)
               << *reinterpret_cast<uint32_t *>(byteSp) << " " << outString << std::endl;
            pc += InstSize;
            byteSp += InstSize;
            numBytes -= InstSize;
        }
        LLVMDisasmDispose(dcr);
    }
    EcmaVM *instance {nullptr};
    JSThread *thread {nullptr};
    EcmaHandleScope *scope {nullptr};
    Chunk *chunk_ {nullptr};
};

#define __ masm.
HWTEST_F_L0(AssemblerRiscv64Test, AddSub)
{
    std::string expectResult(
            "00000000:002080b3 \tadd\tra, ra, sp\n"
            "00000004:003100b3 \tadd\tra, sp, gp\n"
            "00000008:41f887b3 \tsub\ta5, a7, t6\n"
            "0000000c:416409b3 \tsub\ts3, s0, s6\n" 
            "00000010:01f8a7b3 \tslt\ta5, a7, t6\n"
            "00000014:016429b3 \tslt\ts3, s0, s6\n"
            "00000018:01f8b7b3 \tsltu\ta5, a7, t6\n"
            "0000001c:016439b3 \tsltu\ts3, s0, s6\n"
            );
    AssemblerRiscv64 masm(chunk_);
    __ Add(Register(RA), Register(RA), Register(SP));
    __ Add(Register(RA), Register(SP), Register(GP));
    __ Sub(Register(A5), Register(A7), Register(T6));
    __ Sub(Register(S3), Register(FP), Register(S6));
    __ Slt(Register(A5), Register(A7), Register(T6));
    __ Slt(Register(S3), Register(FP), Register(S6));
    __ Sltu(Register(A5), Register(A7), Register(T6));
    __ Sltu(Register(S3), Register(FP), Register(S6));
    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, AddSub_32Width)
{
    std::string expectResult(
            "00000000:000f00bb \taddw\tra, t5, zero\n"
            "00000004:009c8dbb \taddw\ts11, s9, s1\n"
            "00000008:41d382bb \tsubw\tt0, t2, t4\n"
            "0000000c:40b501bb \tsubw\tgp, a0, a1\n");
    AssemblerRiscv64 masm(chunk_);
    __ Addw(Register(RA), Register(T5), Register(ZERO));
    __ Addw(Register(S11), Register(S9), Register(S1));
    __ Subw(Register(T0), Register(T2), Register(T4));
    __ Subw(Register(GP), Register(A0), Register(A1));
    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Shift)
{
    std::string expectResult(
            "00000000:01f897b3 \tsll\ta5, a7, t6\n"
            "00000004:016419b3 \tsll\ts3, s0, s6\n"
            "00000008:01f8d7b3 \tsrl\ta5, a7, t6\n"
            "0000000c:016459b3 \tsrl\ts3, s0, s6\n"
            "00000010:41f8d7b3 \tsra\ta5, a7, t6\n"
            "00000014:416459b3 \tsra\ts3, s0, s6\n");
    AssemblerRiscv64 masm(chunk_);
    __ Sll(Register(A5), Register(A7), Register(T6));
    __ Sll(Register(S3), Register(FP), Register(S6));
    __ Srl(Register(A5), Register(A7), Register(T6));
    __ Srl(Register(S3), Register(FP), Register(S6));
    __ Sra(Register(A5), Register(A7), Register(T6));
    __ Sra(Register(S3), Register(FP), Register(S6));
    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Shift_32Width)
{
    std::string expectResult(
            "00000000:01f897bb \tsllw\ta5, a7, t6\n"
            "00000004:016419bb \tsllw\ts3, s0, s6\n"
            "00000008:01f8d7bb \tsrlw\ta5, a7, t6\n"
            "0000000c:016459bb \tsrlw\ts3, s0, s6\n"
            "00000010:41f8d7bb \tsraw\ta5, a7, t6\n"
            "00000014:416459bb \tsraw\ts3, s0, s6\n");
    AssemblerRiscv64 masm(chunk_);
    __ Sllw(Register(A5), Register(A7), Register(T6));
    __ Sllw(Register(S3), Register(FP), Register(S6));
    __ Srlw(Register(A5), Register(A7), Register(T6));
    __ Srlw(Register(S3), Register(FP), Register(S6));
    __ Sraw(Register(A5), Register(A7), Register(T6));
    __ Sraw(Register(S3), Register(FP), Register(S6));
    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, BitwiseOp)
{
    std::string expectResult(
            "00000000:01f8c7b3 \txor\ta5, a7, t6\n"
            "00000004:016449b3 \txor\ts3, s0, s6\n"
            "00000008:01f8e7b3 \tor\ta5, a7, t6\n"
            "0000000c:016469b3 \tor\ts3, s0, s6\n"
            "00000010:01f8f7b3 \tand\ta5, a7, t6\n"
            "00000014:016479b3 \tand\ts3, s0, s6\n");
    AssemblerRiscv64 masm(chunk_);
    __ Xor(Register(A5), Register(A7), Register(T6));
    __ Xor(Register(S3), Register(FP), Register(S6));
    __ Or(Register(A5), Register(A7), Register(T6));
    __ Or(Register(S3), Register(FP), Register(S6));
    __ And(Register(A5), Register(A7), Register(T6));
    __ And(Register(S3), Register(FP), Register(S6));
    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Beq)
{
	std::string expectResult(
        	"00000000:00b50463 \tbeq\ta0, a1, 8\n"
        	"00000004:00f68863 \tbeq\ta3, a5, 16\n");
    	AssemblerRiscv64 masm(chunk_);
	__ Beq(Register(A0), Register(A1),8);
	__ Beq(Register(A3), Register(A5),16);
    	std::ostringstream oss;
    	DisassembleChunk(TARGET_RISCV64, &masm, oss);
    	ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Bne)
{
	std::string expectResult(
        	"00000000:00c09463 \tbne\tra, a2, 8\n"
           	"00000004:00259863 \tbne\ta1, sp, 16\n");
    	AssemblerRiscv64 masm(chunk_);
	__ Bne(Register(RA), Register(A2),8);
	__ Bne(Register(A1), Register(SP),16);
    	std::ostringstream oss;
    	DisassembleChunk(TARGET_RISCV64, &masm, oss);
    	ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Blt)
{
	std::string expectResult(
        	"00000000:00d54463 \tblt\ta0, a3, 8\n"
            	"00000004:00184263 \tblt\ta6, ra, 4\n");
    	AssemblerRiscv64 masm(chunk_);
	__ Blt(Register(A0), Register(A3),8);
	__ Blt(Register(A6), Register(RA),4);
    	std::ostringstream oss;
    	DisassembleChunk(TARGET_RISCV64, &masm, oss);
    	ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Bge)
{
    std::string expectResult(
        "00000000:0107d263 \tbge\ta5, a6, 4\n"
        "00000004:00945463 \tbge\ts0, s1, 8\n");

    AssemblerRiscv64 masm(chunk_);
    __ Bge(Register(A5), Register(A6), 4);
    __ Bge(Register(S0), Register(S1), 8);
    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Bltu) {
    std::string expectResult(
        "00000000:0062e663 \tbltu\tt0, t1, 12\n"
        "00000004:00a1e463 \tbltu\tgp, a0, 8\n");

    AssemblerRiscv64 masm(chunk_);
    __ Bltu(Register(T0), Register(T1), 0xc);
    __ Bltu(Register(GP), Register(A0), 8);

    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Bgeu)
{
    std::string expectResult(
        "00000000:01f472e3 \tbgeu\ts0, t6, 2052\n"
        "00000004:00a7f963 \tbgeu\ta5, a0, 18\n");

    AssemblerRiscv64 masm(chunk_);
    __ Bgeu(Register(S0), Register(T6), 0x804);
    __ Bgeu(Register(A5), Register(A0), 0x12);
    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, StoreOp)
{
    std::string expectResult(
            "00000000:00758423 \tsb\tt2, 8(a1)\n"
            "00000004:01c69823 \tsh\tt3, 16(a3)\n"
            "00000008:01d7a623 \tsw\tt4, 12(a5)\n");
    AssemblerRiscv64 masm(chunk_);
    __ Sb(Register(A1), Register(T2), 8); // Stores the least significant byte from T2 to the address (A1 + 8).
    __ Sh(Register(A3), Register(T3), 16); // Stores the least significant 2 bytes from T3 to the address (A3 + 16).
    __ Sw(Register(A5), Register(T4), 12); // Stores the least significant 4 bytes from T4 to the address (A5 + 12).
    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, RV64StoreOp)
{
    std::string expectResult(
            "00000000:0096bc23 \tsd\ts1, 24(a3)\n"
            "00000004:01d93823 \tsd\tt4, 16(s2)\n"
            "00000008:00f4b423 \tsd\ta5, 8(s1)\n");
    AssemblerRiscv64 masm(chunk_);
    __ Sd(Register(A3), Register(S1), 24);
    __ Sd(Register(S2), Register(T4), 16);
    __ Sd(Register(S1), Register(A5), 8);
    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Lui)
{
    std::string expectResult(
            "00000000:00008537 \tlui\ta0, 8\n"
            "00000004:0000c337 \tlui\tt1, 12\n"
            "00000008:0007c9b7 \tlui\ts3, 124\n");

    AssemblerRiscv64 masm(chunk_);

    __ Lui(Register(A0), 8);
    __ Lui(Register(T1), 12);
    __ Lui(Register(S3), 124);

    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Auipc)
{
    std::string expectResult(
            "00000000:00008517 \tauipc\ta0, 8\n"
            "00000004:0000c317 \tauipc\tt1, 12\n"
            "00000008:0007c997 \tauipc\ts3, 124\n");

    AssemblerRiscv64 masm(chunk_);

    __ Auipc(Register(A0), 8);
    __ Auipc(Register(T1), 12);
    __ Auipc(Register(S3), 124);

    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Jal)
{
    std::string expectResult(
            "00000000:0080056f \tjal\ta0, 8\n"
            "00000004:00c0036f \tjal\tt1, 12\n"
            "00000008:07c009ef \tjal\ts3, 124\n");
    AssemblerRiscv64 masm(chunk_);

    __ Jal(Register(A0), Immediate(8));
    __ Jal(Register(T1), Immediate(12));
    __ Jal(Register(S3), Immediate(124));

    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Lb)
{
    std::string expectResult(
        "00000000:00450583 \tlb\ta1, 4(a0)\n");

    AssemblerRiscv64 masm(chunk_);

    // LB: Load Byte
    __ Lb(Register(A1), Register(A0), 4);

    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Jalr)
{
    std::string expectResult(
        "00000000:004505e7 \tjalr\ta1, 4(a0)\n");

    AssemblerRiscv64 masm(chunk_);

    // LB: Load Byte
    __ Jalr(Register(A1), Register(A0), 4);

    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Addi)
{
    std::string expectResult(
        "00000000:00450593 \taddi\ta1, a0, 4\n");

    AssemblerRiscv64 masm(chunk_);

    // LB: Load Byte
    __ Addi(Register(A1), Register(A0), 4);

    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Lwu)
{
    std::string expectResult(
        "00000000:00456583 \tlwu\ta1, 4(a0)\n");

    AssemblerRiscv64 masm(chunk_);

    // LB: Load Byte
    __ Lwu(Register(A1), Register(A0), 4);

    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

HWTEST_F_L0(AssemblerRiscv64Test, Addiw)
{
    std::string expectResult(
        "00000000:0045059b \taddiw\ta1, a0, 4\n");

    AssemblerRiscv64 masm(chunk_);

    // LB: Load Byte
    __ Addiw(Register(A1), Register(A0), 4);

    std::ostringstream oss;
    DisassembleChunk(TARGET_RISCV64, &masm, oss);
    ASSERT_EQ(oss.str(), expectResult);
}

#undef __
}  // namespace panda::test
