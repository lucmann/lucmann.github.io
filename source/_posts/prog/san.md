---
title: Using Sanitizers
date: 2022-11-08 15:58:26
tags: [Compiler]
categories: programming
---


# Sanitizer 简介

Sanitizer 是一个动态 Bug 检测的工具，它可以检测的 Bug 有以下几类：

- AddressSanitizer (ASan):
    * use-after-free
    * double-free
    * buffer (heap, stack, and global buffer) overflows
- MemorySanitizer (MSan): uninitialized memory reads
- UndefinedBehaviorSanitizer (UBSan)
    * signed integer overflow
    * use-of-null-pointer
    * division by zero
- ThreadSanitizer: data races, deadlocks
- LeakSanitizer: memory leaks

# 参考

[1][Be wise, sanitize: Keeping your C++ code free from bugs](https://microblink.com/be-wise-sanitize-keeping-your-c-code-free-from-bugs/)
[2][An LLVM developer setup: Modern C++ development tools](https://llvm.org/devmtg/2016-01/slides/ModernCplusplusDevelopment.pdf)

