# Ark Compiler Monorepo (for RISC-V)

## Getting Started

Build clang:

``` shell
# Inhibit arm, aarch64 and windows, for more information see
#   python3 ./toolchain/llvm-project/llvm-build/build.py --help
python3 ./toolchain/llvm-project/llvm-build/build.py \
	--no-build-arm \
	--no-build-aarch64 \
	--no-build-mipsel \
	--no-bulid windows
```

Copy clang to _prebuilts_ directory:

``` shell
mv prebuilts/clang/ohos/linux-x86_64/llvm prebuilts/clang/ohos/linux-x86_64/llvm.origin 
cp -r out/install/linux-x86_64/clang-dev/ prebuilts/clang/ohos/linux-x86_64/llvm
```

Build LLVM Lib:

``` shell
python3 toolchain/llvm-project/llvm-build/build-ohos-riscv64.py
```

Copy LLVM Lib to _prebuilts_ directory:

``` shell
mkdir -p prebuilts/ark_tools/ark_js_prebuilts/llvm_prebuilts_riscv64/{build,llvm}
cp -r out/ohos-riscv64-install/include prebuilts/ark_tools/ark_js_prebuilts/llvm_prebuilts_riscv64/llvm/
cp -r out/ohos-riscv64/{include,lib} prebuilts/ark_tools/ark_js_prebuilts/llvm_prebuilts_riscv64/build/
```

Build `x86_64` Ark eTS Runtime:

``` shell
python3 ./arkcompiler/toolchain/build/compile_script/ark.py x64.release --verbose
```

And also `riscv64`：

``` shell
python3 ./arkcompiler/toolchain/build/compile_script/ark.py riscv64.release --verbose
```

Run `test262`:

``` shell
python3 ./arkcompiler/toolchain/build/compile_script/ark.py riscv64.release test262
```


## License

All subtrees follow its upstream LICENSE.
