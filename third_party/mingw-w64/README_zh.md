# Mingw-w64 

Mingw-w64是对原始mingw项目的改进，旨在支持Windows系统上的GCC编译器，可支持编译生成64位或32位windows环境下可执行程序。

官方主页：https://www.mingw-w64.org/

为了让[LLVM编译器](https://gitee.com/openharmony/third_party_llvm-project) 支持在mingw64环境下的编译，引入了Mingw-w64。

## 目录结构

```
README.md               英文说明
README_zh.md            中文说明
LICENSE                 证书文件
mingw-w64-crt/          编译运行文件
mingw-w64-doc/          编译文档
mingw-w64-headers/      mingw-w64头文件
mingw-w64-libraries/    mingw-w64库文件
```

## LLVM MinGW

基于 LLVM 的 MinGW 工具链的好处：

- 支持面向 ARM/ARM64（虽然 GCC 显然支持这些架构，但它不支持 ARM 上的 Windows）
- 针对所有四种架构（i686、x86_64、armv7 和 arm64）的单个工具链，而不是针对每种架构的单独编译器二进制文件
- 支持生成 PDB 格式的调试信息
- 支持 Address Sanitizer 和 Undefined Behavior Sanitizer

 Clang 本身也可以在基于 GNU binutils 的普通环境中用作编译器，因此主要区别在于将 binutils 替换为基于 LLVM 的工具。 

## 构建

mingw-w64目前主要作为llvm的依赖库使用，单独构建请参考[how to build](https://gitee.com/openharmony/third_party_mingw-w64/blob/master/mingw-w64-doc/howto-build/mingw-w64-howto-build.txt)。

官方wiki：https://sourceforge.net/p/mingw-w64/wiki2/Home/

## License

见[License](https://gitee.com/openharmony/third_party_mingw-w64/blob/master/LICENSE)

