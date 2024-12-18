---
title: GNU Compiler Collection - gcc/g++
date: 2021-04-15 15:58:26
tags: [Compiler]
categories: utilities
---

# `__attribute__`

`__attribute__` is a keyword introduced by GCC. It is regarded as an extension of a language. It helps the compiler optimize calls, check code more carefully for correctness, control memory placement and code generation options.

<!--more-->

## Syntax
```
__attribute__ ((attribute-list))
```

where an attribute-list is a possibly empty comma-separated sequence of attributes. Say:

```
static void _init( void ) __attribute__((constructor));
```

## Categories
- [Function Attributes](https://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html#Function-Attributes)
- [Variable Attribute](https://gcc.gnu.org/onlinedocs/gcc/Variable-Attributes.html#Variable-Attributes)
- [Type Attributes](https://gcc.gnu.org/onlinedocs/gcc/Type-Attributes.html#Type-Attributes)
- [Label Attributes](https://gcc.gnu.org/onlinedocs/gcc/Label-Attributes.html#Label-Attributes)
- [Enumerator Attributes](https://gcc.gnu.org/onlinedocs/gcc/Enumerator-Attributes.html#Enumerator-Attributes)
- [Statement Attributes](https://gcc.gnu.org/onlinedocs/gcc/Statement-Attributes.html#Statement-Attributes)

Let's say to specify an attribute of variables.

```
extern __thread struct _glapi_table * _glapi_tls_Dispatch
    __attribute__((tls_model("initial-exec")));
```

where the `tls_model` attribute sets thread-local storage model of a particular thread variable, overriding `-ftls-model=` command-line switch on a per-variable basis. The [`tls_model`](https://docs.oracle.com/cd/E53394_01/html/E54813/man-tlsam.html#scrolltoc) argument should be one of `global-dynamic`, `local-dynamic`, `initial-exec`, or `local-exec`.

### [Visibility](http://anadoxin.org/blog/control-over-symbol-exports-in-gcc.html)

```
__attribute__((visibility("default")));
```

this attribute involves with the visibility of exported symbols in a shared object, overriding `-fvisibility=hidden` command-line option during the compilation. The visibility argument should be one of `default`, `hidden`, `internal`, or `protected`.

# 内置宏
编译器会预定义许多宏，尤其在交叉编译时，不同的工具链编译器会定义不同的宏。例如:

- __aarch64__
- __ARM_ARCH_7A
- __ARM_ARCH_8A

如果使用**armv7**的工具链，那么只有`__ARM_ARCH_7A`会被定义，而如果使用**armv8**的工具链，那么只有`__ARM_ARCH_8A`会被定义。但以上两个编译器都会定义`__aarch64__`.

## How to Check Macros Predefined by Compiler
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

## [`__builtin_offsetof`](https://gcc.gnu.org/onlinedocs/gcc/Offsetof.html)

- C/C++ 宏 `offsetof` 包含在头文件 `stddef.h`

# 内置函数

## [`__builtin_clz`](https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html)

`int __builtin_clz(unsigned int x);`

返回一个数的高位端的 `0` 的个数

## [原子操作函数族](https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html)

- `type __atomic_add_fetch(type *ptr, type val, int memorder)`

以原子方式给 `ptr` 指向的内存值加上 `val`, 并返回 `ptr` 指向内存加完后的值。它与 GCC 的 `__sync_add_and_fetch()` 有何区别呢？ 唯一的区别是 `__atomic_add_fetch` 更新。

- `bool __atomic_compare_exchange_n(type *ptr, type *expected, type desired, bool weak, int success_memorder, int failure_memorder)`

原子 CAS 操作：
```
if (*ptr == *expected); then
   *ptr = desired  // read-modify-write
else
   *expected = *ptr // read
```

`type __sync_val_compare_and_swap (type *ptr, type oldval, type newval, ...)`

内置的原子比较和交换操作，也就是说，如果 `*ptr` 的当前值是 `oldval`, 则把 `newval` 写入 `*ptr`, 并且返回写入前的 `*ptr` 值。

# 与编译器相关的 keywords

## volatile vs. register

| volatile                               | register                                |
|:---------------------------------------|:----------------------------------------|
| type qualifier                         | storage class                           |
| force (compiler)                       | hint (compiler)                         |
| latency                                | fast                                    |
| OK for global variables                | NOT for global variables                |
