---
title: Macros Predefined by Compiler
date: 2021-04-15 15:58:26
tags: [gcc]
categories: bin
---

# What Does A Compiler Do For Us Quietly
编译器会预定义许多宏，尤其在交叉编译时，不同的工具链编译器会定义不同的宏。例如:

- __aarch64__
- __ARM_ARCH_7A
- __ARM_ARCH_8A

<!--more-->

如果使用**armv7**的工具链，那么只有`__ARM_ARCH_7A`会被定义，而如果使用**armv8**的工具链，那么只有`__ARM_ARCH_8A`会被定义。但以上两个编译器都会定义`__aarch64__`.

# How to Check Macros Predefined by Compiler
```
aarch64-linux-gnu-gcc -march=armv8-a -E -dM - < /dev/null
```

- `-E` 是`gcc`的一个`common option`, 指示编译器只做预处理
- `-d<letters>` 是`gcc`的一个`common option`, 打开由字母指定的compiler pass的dump
  * `-dM` 打印预处理阶段所有的宏`#define`, 包括预定义宏。
  * `-dD` 功能和`-dM`类似，但它不包含预定义宏，而且它同时输出`#define`和预处理结果。
  * `-dN` 功能和`-dD`类似，但只输出宏名，不展开宏。
  * `-dI` 除了预处理结果外，还输出`#include`.
  * `-dU` 功能和`-dD`类似，但只输出那些被展开的宏或在预处理中被`#if`, `#ifndef`测试过的宏。
