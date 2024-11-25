---
title: CMake 不是一个构建系统
date: 2021-01-10 07:42:04
tags: [tools]
categories: utilities
---

最近看了一些关于Modern CMake(since 3.0.0)的教程，这里记录一下.

<!--more-->

# Modern CMake
作为一个*Build System Generator*, 现代CMake可以帮助我们做哪些事情呢？

- Build
- Install
- Test
- Package

# Command Options
- cmake -B build
  - Build的工作主要包括解决依赖关系，构建目标(库,应用,测试用例)。`cmake`命令有很多选项可以帮助我们灵活有效地构建目标， 例如：Out-of-source构建，意思是不污染源代码目录，在指定的单独的目录下生成构建系统, 如果build不存在，cmake会创建它

- cmake -B build --graphviz=file.dot
  - 将构建目标间的依赖关系输出Graphivz dot, 生成的`file.dot`可以用dot命令生成图片`dot -Tpng -o file.png file.dot`

- cmake --build build
  - 开始构建, 通过`-B`, `--build`选项可以省去`mkdir build && cd build && cmake .. && make`繁琐的操作

- cmake --build build --clean-first
  - 全量构建, 如果是 `-G "Ninja"`, 则 `ninja -C build clean`, 然后再重新 `ninja -C build`

- cmake --build build --target lib1
  - 只构建指定的目标`lib1`, 如果`lib1`依赖其它目标，被依赖的目标也会被构建

- cmake --install build
  - 安装

- cmake -L build
  - 查询CMAKE变量配置值，包括内置变量和自定义变量

- cmake --build build --target help
  - 列出生成的Makefile里有效的`target`

# Built-in Variables

- `-DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=gold"`
  - 将 linker 由默认的 bfd 换成 gold, CMake 3.29+ 使用 `-DCMAKE_LINKER=gold`

- `-DCMAKE_EXPORT_COMPILE_COMMANDS=on`
  - 生成 compile_commands.json

- `-DCMAKE_CXX_FLAGS="-Wno-error=missing-field-initializers"`
  - 将 `missing-field-initializers` 由 error 转为 warning

- `-DCMAKE_CXX_FLAGS="-stdlib=libc++ -fexperimental-library"`
  - 当你使用 LLVM 的 c++ runtimes 时， 需要额外指定这两个编译选项，否则编译器仍然会使用 `libstdc++.so.1`

- `-DCMAKE_EXE_LINKER_FLAGS="-nostartfiles"`
  - 解决 "Scrt1.o: undefined reference to `main`"

- CMAKE_SOURCE_DIR
  - 指所在工程顶层目录绝对路径，一般源码git-clone下来后就确定了

- CMAKE_CURRENT_SOURCE_DIR
  - 指CMakeLists.txt所在目录的绝对路径，随着CMakeLists.txt位置变化

- `-DCMAKE_VERBOSE_MAKEFILE=ON`
  - 产生非常详细的编译过程日志，包括目录改变，编译器选项和链接器选项

- CMAKE_INSTALL_PREFIX
  - 自定义安装路径, 默认 /usr/local

- `-DCMAKE_CXX_STANDARD=14
  - C++ 版本号, 如 11, 14

- `-DCMAKE_CXX_COMPILER_ID=GNU`
  - 如 GNU, Clang, Intel, MSVC, 作用和 `CC=gcc CXX=g++` 一样

- `-DCMAKE_CXX_FLAGS="-std=c++11"`
  - C++ 编译器选项， 如 -std=c++11

## Guideline

- Declare your module with `ADD_LIBRARY` or `ADD_EXECUTABLE`.
- Declare your build flags with `TARGET_xxx()`.
- Declare your dependencies with `TARGET_LINK_LIBRARIES`
- Specify what is `PUBLIC` and what is `PRIVATE`
- Don't make any assumption about the platform and compiler.
- Make sure that all your projects can be built both standalone and as a subproject of another project.
- Always add namespaced aliases for libraries.
  ```
  add_library(foo STATIC
    foo1.cpp
    foo2.cpp
    )
  add_library(my::foo ALIAS foo)
  ```
- Don't make libraries STATIC/SHARED unless they cannot be built otherwise.
- Leave the control of BUILD_SHARED_LIBS to your clients.
- Create macros to wrap commands that have output parameters. Otherwise, creat a function.
- Modern CMake is about Targets and Properties!
- Avoid custom variables in the arguments of project commands.
- CMake is not a build system, but a build system generator.
- Forget those commands:
    * add_compile_options()
    * include_directories()
    * link_directories()
    * link_libraries()
- Non-INTERFACE_ properties define the build specification of a targt
- INTERFACE_ properties define the usage requirements of a target
- Use target_link_libraries() to express direct dependencies
- Don't use `TARGET_LINK_LIBRARIES()` without specifying `PUBLIC`, `PRIVATE` or `INTERFACE`.

## Targets and Properties

Modern CMake更像一个面向对象编程语言， Targets是Objects, 它们有Properties(Member Variables)和Commands(Methods), 
Targets的Properties包括编译这个Target的源文件，编译选项，以及最后链接的库这些都是一个Target的Properties. 只要是Properties，就有这个Property的作用域(Scope). Properties也有作用域的概念(scope), 对应`INTERFACE`和`PRIVATE`. 
INTERFACE properties是外部用的，也就是给导入或使用本Targets的其它Targets用的。PRIVATE properties是Targets内部用的。

## CMake Inheritance

### Include Inheritance

在 CMake 中，在预处理阶段搜索头文件是从 `INCLUDE_DIRECTORIES` 和 `INTERFACE_INCLUDE_DIRECTORIES` 这两个变量里包含的路径中搜索。`target_include_directories` 会将指定的路径都加入 `INCLUDE_DIRECTORIES`, 但是会依据 `<PRIVATE|PUBLIC|INTERFACE>` 有选择地将指定路径加入 `INTERFACE_INCLUDE_DIRECTORIES`. `INCLUDE_DIRECTORIES` 包含的路径只会被**当前 target** 作为搜索路径, 而 `INTERFACE_INCLUDE_DIRECTORIES` 包含的路径会被加到任何依赖当前 target 的 target 的 `INCLUDE_DIRECTORIES`.

## Generator Expressions
Generator Expressions（生成表达式）是指在生成构建系统的过程中（如果是Make构建系统，就是在生成Makefile的过程中）针对每个构建配置生成特定的值. 生成表达式有3类:
- Logical Expressions
- Informational Expressions
- Output Expressions

```
target_compile_definitions(foo PRIVATE
    "VERBOSITY=$<IF:$<CONFIG:Debug>,30,10>"
)    
```

上例中使用了嵌套的generator expressions, `$<CONFIG:cfg>`嵌套在`$<IF:?,true-value...,false-value...>`, 两者都是logical expressions, 注意后者是CMake 3.8才有的。`$<CONFIG:Debug>`的意思是如果`CONFIG`是`Debug`,那么这个表达式的值是`1`,否则是`0`, 注意这个比较是不区分大小写的字符串比较。`$<IF:?,true-value...,false-value...>`就像三元表达式`a ? b : c`一样。

注意`0`和`1`是两个basic logical expressions,所有其它logical expressions的最终值都是`0`或`1`,所以下面的表达式是有效的


$<$<CONFIG:Debug>:DEBUG_MODE>


它展开后是`$<0:DEBUG_MODE>`或`$<1:DEBUG_MODE>`,所以整个表达式最终值是`DEBUG_MODE`或空。


## `find_package()`

如果cmake可以成功执行 `find_package(PACKAGE)`，那么说明你的系统上一定存在一个 `FindPACKAGE.cmake`的文件

- `find_package(Threads REQUIRED)`: FindThreads.cmake 文件确定一个系统上的 thread 库，如果在Linux 下，如果找到的话就是 `libpthread.so` (C++ std::thread 依赖 `libpthread.so`)

## `pkg_check_modules()`

使用 `pkg_check_modules()` 前需要先(因为pkg_check_modules() 底层是依靠 pkg-config)

- `find_package(PkgConfig)`

例如找到系统上的 libdrm.so (因为cmake和libdrm本身没有提供 FindLibDRM.cmake), 就需要

- `pkg_check_modules(LIBDRM REQUIRED libdrm)`

这里 `LIBDRM` 是自定义的，但要与后面使用的变量名对齐，如果libdrm.so 被找到，则以下变量会被置成相应的路径或值(基本上就是pkg-config 能查到的所有信息)

- LIBDRM_FOUND
- LIBDRM_LIBRARIES
- LIBDRM_LIBRARY_DIRS
- LIBDRM_LDFLAGS
- LIBDRM_LDFLAGS_OTHER
- LIBDRM_INCLUDE_DIRS
- LIBDRM_CFLAGS
- LIBDRM_CFLAGS_OTHER

## CTest

- 打开CTest, 后续可以直接通过 `make [-C builddir] test` 运行测试用例
  - `enable_testing()`

- 增加测试用例
  - `add_test(NAME PALBench.test_cpu_read_vram COMMAND test_cpu_access_vram -size 1048576 -access read)`

# NOTES
## -fPIC

- -fPIC 是一个编译器选项，而不是链接器选项。
- 当你正在构建的一个动态库(如libPAL.so)依赖另一个静态库(如libADT.a), 这时必须在 `add_library(ADT STATIC ...)` 之后增加编译选项 `-fPIC`, 这时需要使用
    - `target_compile_options(ADT PRIVATE "-fPIC")`
  而不是
    - `target_link_options(ADT PUBLIC "-fPIC")`

## _GNU_SOURCE

- 编译器莫名其妙地报undeclared错误，如 `O_CLOEXEC` 未声明，即使已经包含了它的头文件 `fcntl.h` 也还报，这可能需要定义一下`_GNU_SOURCE`:
  - `target_compile_definitions(target PUBLIC _GNU_SOURCE)`

# 小贴士

- `time ninja -C build | while read line; do echo $(date +%s.%N) ${line}; done`
  - 输出编译时间
