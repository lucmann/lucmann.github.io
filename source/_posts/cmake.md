---
title: CMake
date: 2021-01-10 07:42:04
tags:
categories: /bin
---

最近看了一些关于Modern CMake(since 3.0.0)的教程，这里记录一下.

<!--more-->

# Guideline
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

# Targets and Properties
Modern CMake更像一个面向对象编程语言， Targets是Objects, 它们有Properties(Member Variables)和Commands(Methods), 
Targets的Properties包括编译这个Target的源文件，编译选项，以及最后链接的库这些都是一个Target的Properties. 只要是Properties，就有这个Property的作用域(Scope). Properties也有作用域的概念(scope), 对应`INTERFACE`和`PRIVATE`. 
INTERFACE properties是外部用的，也就是给导入或使用本Targets的其它Targets用的。PRIVATE properties是Targets内部用的。

