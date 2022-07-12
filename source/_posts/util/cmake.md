---
title: Modern CMake
date: 2021-01-10 07:42:04
tags: [cmake]
categories: utilities
---

最近看了一些关于Modern CMake(since 3.0.0)的教程，这里记录一下.

<!--more-->

# What Modern CMake Provides?
作为一个*Build System Generator*, 现代CMake可以帮助我们做哪些事情呢？

- Build
- Install
- Test
- Package

## Build
Build的工作主要包括解决依赖关系，构建目标(库,应用,测试用例)。`cmake`命令有很多选项可以帮助我们灵活有效地构建目标， 例如：

Out-of-source构建，意思是不污染源代码目录，在指定的单独的目录下生成构建系统, 如果build不存在，cmake会创建它

```
cmake -B build
```

将构建目标间的依赖关系输出Graphivz dot, 生成的`file.dot`可以用dot命令生成图片`dot -Tpng -o file.png file.dot`

```
cmake -B build --graphviz=file.dot
```

开始构建, 通过`-B`, `--build`选项可以省去`mkdir build && cd build && cmake .. && make`繁琐的操作

```
cmake --build build
```

全量构建

```
cmake --build build --clean-first
```

只构建指定的目标`lib1`, 如果`lib1`依赖其它目标，被依赖的目标也会被构建

```
cmake --build build --target lib1
```

安装, 一般需要root权限

```
cmake --install build
```

查询CMAKE变量配置值，包括内置变量和自定义变量

```
cmake -L build
```

列出生成的Makefile里有效的`target`

```
cmake --build build --target help
```

# CMake's `-D`

- `-DCMAKE_EXPORT_COMPILE_COMMANDS=True`    : 生成 `compile_commands.json`, 用来在像 CLion 这样的 IDE 里创建 index
- `-DCMAKE_VERBOSE_MAKEFILE=True`           : 产生非常详细的编译过程日志，包括目录改变，编译和链接
- `-DCMAKE_INSTALL_PREFIX=/path/to/install` : 自定义安装路径

# Guideline

- Declare your module with `ADD_LIBRARY` or `ADD_EXECUTABLE`.
- Declare your build flags with `TARGET_xxx()`.
- Declare your dependencies with `TARGET_LINK_LIBRARIES`
- Specify what is `PUBLIC` and what is `PRIVATE`
- Don't make any assumption about the platform and compiler.
- Make sure that all your projects can be built both standalone and as a subproject of another project.
- Always add namespaced aliases for libraries.
    ```cmake
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

# Targets and Properties

Modern CMake更像一个面向对象编程语言， Targets是Objects, 它们有Properties(Member Variables)和Commands(Methods), 
Targets的Properties包括编译这个Target的源文件，编译选项，以及最后链接的库这些都是一个Target的Properties. 只要是Properties，就有这个Property的作用域(Scope). Properties也有作用域的概念(scope), 对应`INTERFACE`和`PRIVATE`. 
INTERFACE properties是外部用的，也就是给导入或使用本Targets的其它Targets用的。PRIVATE properties是Targets内部用的。

## CMake Inheritance

### Include Inheritance

在 CMake 中，在预处理阶段搜索头文件是从 `INCLUDE_DIRECTORIES` 和 `INTERFACE_INCLUDE_DIRECTORIES` 这两个变量里包含的路径中搜索。`target_include_directories` 会将指定的路径都加入 `INCLUDE_DIRECTORIES`, 但是会依据 `<PRIVATE|PUBLIC|INTERFACE>` 有选择地将指定路径加入 `INTERFACE_INCLUDE_DIRECTORIES`. `INCLUDE_DIRECTORIES` 包含的路径只会被**当前 target** 作为搜索路径, 而 `INTERFACE_INCLUDE_DIRECTORIES` 包含的路径会被加到任何依赖当前 target 的 target 的 `INCLUDE_DIRECTORIES`.

# Generator Expressions
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

```
$<$<CONFIG:Debug>:DEBUG_MODE>
```

它展开后是`$<0:DEBUG_MODE>`或`$<1:DEBUG_MODE>`,所以整个表达式最终值是`DEBUG_MODE`或空。

# Appendix: **CMake Built-in Variables**
- CMAKE_CURRENT_SOURCE_DIR

指`CMakeLists.txt`所在当前目录的绝对路径，一般随着`CMakeLists.txt`所在目录的变化而变化。

- CMAKE_SOURCE_DIR

指所在工程顶层目录绝对路径，一般源码`clone`下来后就确定了。

# `find_package()` vs `pkg_check_modules()`

## `find_package()`

```
find_package(Gradle REQUIRED 4.10)
find_package(Vulkan QUIET)
find_package(PkgConfig REQUIRED)
find_package(${ARGN})
find_package (PkgConfig)
find_package(Doxygen)
find_package(X11 REQUIRED)
find_package(EpollShim)
find_package(OSMesa REQUIRED)
find_package(Threads)
find_package (Threads)
find_package(Boost COMPONENTS regex)
find_package(fmt REQUIRED CONFIG)
find_package(PythonInterp REQUIRED)
find_package(Python COMPONENTS Interpreter REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(Vulkan)
find_package(SDL2 REQUIRED)
find_package(assimp REQUIRED)
find_package( ZLIB REQUIRED )
find_package(Git QUIET)
find_package(Threads)
find_package(Git QUIET)
find_package(Sanitizers)
find_package(Boost 1.61 REQUIRED)
```

## `pkg_check_modules()`

```
pkg_check_modules(XCB xcb REQUIRED)
pkg_check_modules(X11 x11 REQUIRED)
pkg_check_modules(WAYLAND wayland-client REQUIRED)
pkg_check_modules (PKG_OSMESA QUIET osmesa)
pkg_check_modules(Wayland REQUIRED
pkg_check_modules(WAYLAND_PROTOCOLS REQUIRED wayland-protocols>=1.15)
```
