
#ifndef ECMASCRIPT_COMPILER_RISCV64_EXTENDED_ASSEMBLER_RISCV64_H
#define ECMASCRIPT_COMPILER_RISCV64_EXTENDED_ASSEMBLER_RISCV64_H

#include "ecmascript/compiler/assembler/riscv64/assembler_riscv64.h"
#include "ecmascript/compiler/assembler_module.h"
#include "ecmascript/compiler/bc_call_signature.h"

namespace panda::ecmascript::riscv64 
{
using namespace panda::ecmascript::kungfu;
    class ExtendedAssembler : public AssemblerRiscv64 
    {
        public:
            static constexpr int FRAME_SLOT_SIZE = 8; // TODO: Not sure 8
            static constexpr int PAIR_SLOT_SIZE = 16; // TODO: Not sure 16
            ExtendedAssembler(Chunk *chunk, AssemblerModule *module)
            : AssemblerRiscv64(chunk), module_(module)
            {
                  uint8_t* buffer = module_->GetBuffer();
                  printf("%s\n", buffer);
            }
            uint8_t* getBuffer()
            {
                  return module_->GetBuffer();
            }
            void BindAssemblerStub(int id);
            void CalleeSave();
            void CalleeRestore();
            void PushFpAndRa();
            void PushFpAndLr();
            void RestoreFpAndLr();
            void RestoreFpAndRa();
            void CallAssemblerStub(int id, bool isTail = false);
            void Align16(Register FP);
            void PushArgsWithArgv(Register argc, Register argv, Register operatorRegister);
            void PushArgc(int32_t argc, Register tempArgcRegister);
            void PushArgc(Register argcRegister, Register tempArgcRegister);
            Register CallDispatcherArgument(kungfu::CallDispatchInputs index)
            {
                size_t i = static_cast<size_t>(index);
                Register ret = isGhcCallingConv_ ? ghcJSCallDispacherArgs_[i] : cppJSCallDispacherArgs_[i];
                if (ret.GetId() == INVALID_REG) {
                    LOG_COMPILER(FATAL) << "riscv invalid call argument:" << i;
                }
                return ret;
            }
            Register TempRegister1()
            {
                if (temp1InUse_) {
                    LOG_COMPILER(FATAL) << "temp register1 inuse.";
                    UNREACHABLE();
                }
                temp1InUse_ = true;
                return T0;
            }
            Register TempRegister2()
            {
                if (temp2InUse_) {
                    LOG_COMPILER(FATAL) << "temp register2 inuse.";
                    UNREACHABLE();
                }
                temp2InUse_ = true;
                return T1;
            }
            
            Register AvailableRegister1() const
            {
                // X28 is neither callee saved reegister nor argument register
                return X28;
            }
            Register AvailableRegister2() const
            {
                // X29 is neither callee saved reegister nor argument register
                return X29;
            }
            Register AvailableRegister3() const
            {
                // X30 is neither callee saved reegister nor argument register
                return X30;
            }
            Register AvailableRegister4() const
            {
                // X31 is neither callee saved reegister nor argument register
                return X31;
            }

            Register GlueRegister()
            {
                return isGhcCallingConv_ ? X19 : X0;
            }

            bool FromInterpreterHandler() const
            {
                return isGhcCallingConv_;
            }
        private:
            AssemblerModule *module_ {nullptr};
            bool isGhcCallingConv_ {false};
            bool tempInUse_ {false};
            bool temp1InUse_ {false};
            bool temp2InUse_ {false};
            friend class TempRegister1Scope;
            friend class TempRegister2Scope;
            friend class TempRegisterScope;

            static constexpr size_t JS_CALL_DISPATCHER_ARGS_COUNT =
            static_cast<size_t>(kungfu::CallDispatchInputs::NUM_OF_INPUTS);
            static Register ghcJSCallDispacherArgs_[JS_CALL_DISPATCHER_ARGS_COUNT];
            static Register cppJSCallDispacherArgs_[JS_CALL_DISPATCHER_ARGS_COUNT];
    };

    class TempRegister1Scope {
    public:
        explicit TempRegister1Scope(ExtendedAssembler *assembler) : assembler_(assembler) {}
        ~TempRegister1Scope()
        {
            assembler_->temp1InUse_ = false;
        }

        NO_COPY_SEMANTIC(TempRegister1Scope);
        NO_MOVE_SEMANTIC(TempRegister1Scope);
    private:
        ExtendedAssembler *assembler_;
    };


    class TempRegister2Scope {
    public:
        explicit TempRegister2Scope(ExtendedAssembler *assembler) : assembler_(assembler) {}
        ~TempRegister2Scope()
        {
            assembler_->temp2InUse_ = false;
        }

        NO_COPY_SEMANTIC(TempRegister2Scope);
        NO_MOVE_SEMANTIC(TempRegister2Scope);
    private:
        ExtendedAssembler *assembler_;
    };
}       // panda::ecmascript::riscv64
#endif  // ECMASCRIPT_COMPILER_RISCV64_EXTENDED_ASSEMBLER_RISCV64_H
