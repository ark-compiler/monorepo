# lldb-mi

这是来自 llvm-8.0.1 的 LLDB 的机器接口驱动程序。

# Build

lldb-mi使用CMake构建。lldb-mi所需的唯一依赖项是C++14编译器和lldb本身（包括其依赖项：Clang和LLVM）。这些依赖项的安装方式应确保可以通过CMake的[find_package](https://cmake.org/cmake/help/latest/command/find_package.html)找到它们的功能。您需要LLDB/Clang/LLVM头及其编译的库才能使构建工作，但不需要相应的源文件。


# 基于系统LLDB的构建

在OpenHarmony中，该仓库被LLVM工具链构建流程依赖，用于提供lldb的mi接口能力，详见
https://gitee.com/openharmony-sig/third_party_llvm-project/blob/master/llvm-build/README.md  

如果您想基于社区标准流程构建，并且发行版或操作系统已经提供了正确配置的LLDB/Clang/LLVM安装，则只需运行以下命令即可构建LLDB-mi：

```sh
cmake . ; cmake --build .
```

# 根据自定义LLDB构建

您还可以根据自己编译的lldb构建lldb-mi。为此，请按[此处](https://lldb.llvm.org/resources/build.html)所述编译LLDB。但将`CMAKE_INSTALL_PREFIX`设置为本地目录，并通过将`-DLLVM_BUILD_LLVM_DYLIB=On`传递给CMAKE来构建LLVM共享库。然后，在通过设置`CMAKE_PREFIX_PATH`（例如`cmake -DCMAKE_PREFIX_PATH=/home/yourname/lldb-mi/install`）构建lldb-mi时，指向该前缀目录。

此示例脚本应在~/buildspace子文件夹中的普通UNIX系统上构建LLVM和lldb-mi：

```
cd
mkdir buildspace

# Download LLVM/Clang/LLDB and build them.
git clone https://github.com/llvm/llvm-project.git
mkdir llvm-inst
mkdir llvm-build
cd llvm-build

cmake -DLLVM_ENABLE_PROJECTS="clang;lldb;libcxx;libcxxabi" -DCMAKE_INSTALL_PREFIX=~/buildspace/llvm-inst/ -GNinja ../llvm-project/llvm
ninja
ninja install

# Download lldb-mi and build it against our custom installation.
cd ~/buildspace
git clone https://github.com/lldb-tools/lldb-mi
cd lldb-mi

# Create a separate build directory for building lldb-mi.
mkdir build
cd build
cmake -DCMAKE_PREFIX_PATH=~/buildspace/llvm-inst/ -GNinja ..
ninja
```

# 根据自定义LLDB构建（仅限达尔文）框架

您还可以针对您自己编译的框架构建lldb-mi。指令与上述相同，但通过将`-DLLDB_BUILD_FRAMEWORK=1`传递给CMake而不是使用共享库来构建LLVM框架。然后用`-DUSE_LLDB_FRAMEWORK=1`构建LLDB-MI。

下面的代码片段更改了上面脚本中的cmake调用。



```
// Building llvm-project
cmake -DLLVM_ENABLE_PROJECTS="clang;lldb;libcxx;libcxxabi" -DCMAKE_INSTALL_PREFIX=~/buildspace/llvm-inst/ -DLLDB_BUILD_FRAMEWORK=1 -GNinja ../llvm-project/llvm
```

```
// Building lldb-mi
cmake -DCMAKE_PREFIX_PATH=~/buildspace/llvm-inst/ -DUSE_LLDB_FRAMEWORK=1 -GNinja ..
```

# Notes

在某些架构上(如Linux, x86-64)， LLDB动态库无法确定其位置。这导致无法定位gdbserver存根:

```bash
process launch failed: unable to locate lldb-server
```

解决方法是在运行LLDB-MI之前设置环境变量LLDB_DEBUGSERVER_PATH。

