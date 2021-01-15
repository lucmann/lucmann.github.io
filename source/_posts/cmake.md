---
title: CMake
date: 2021-01-10 07:42:04
tags:
categories: /bin
---

最近看了一些关于Modern CMake(since 3.0.0)的教程，这里记录一下.

<!--more-->

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
