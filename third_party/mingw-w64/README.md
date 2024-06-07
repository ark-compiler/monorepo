# Mingw-w64

 Mingw-w64 is an advancement of the original mingw.org project, created to support the GCC compiler on Windows systems, and can support compiling and generating executable programs under 64 bit or 32-bit windows environment.

Official Homepage:https://www.mingw-w64.org/ 

To enable [LLVM compiler](https://gitee.com/openharmony/third_party_llvm-project ) support compilation in mingw64 environment introduce Mingw-w64.

## Directory Structure 

```
README.md               #this file
README_zh.md            #description in Chinese
LICENSE                 #license
mingw-w64-crt/          #compile run File
mingw-w64-doc/          #compile documents
mingw-w64-headers/      #mingw-w64 headers file
mingw-w64-libraries/    #mingw-w64 libraries file
```

## LLVM MinGW

Benefits of a LLVM based MinGW toolchain are:

- Support for targeting ARM/ARM64 (while GCC obviously does support these architectures, it doesn't support Windows on ARM)
- A single toolchain targeting all four architectures (i686, x86_64, armv7 and arm64) instead of separate compiler binaries for each architecture
- Support for generating debug info in PDB format
- Support for Address Sanitizer and Undefined Behaviour Sanitizer

Clang on its own can also be used as compiler in the normal GNU binutils based environments though, so the main difference lies in replacing binutils with LLVM based tools. 

## Build

 Mingw-w64 is currently mainly used as a dependency library of llvm. Please refer to [how to build](https://gitee.com/openharmony/third_party_mingw-w64/blob/master/mingw-w64-doc/howto-build/mingw-w64-howto-build.txt).

wiki:https://sourceforge.net/p/mingw-w64/wiki2/Home/

## License

see [License](https://gitee.com/openharmony/third_party_mingw-w64/blob/master/LICENSE).