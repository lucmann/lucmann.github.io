---
title: Sanitizers
date: 2022-11-08 15:58:26
tags: [tools]
categories: utilities
---

![shadowed process memory](/images/sanitizer/shadowed-process-memory.png)

<!--more-->

# Sanitizer 简介

Sanitizer 是一个动态 Bug 检测的工具，它可以检测的 Bug 有以下几类：

- 内存越界
- 内存未初始化
- 内存泄漏
- 死锁
- 未定义行为 (UB)

# 内存 Bug

Sanitizer 检测内存越界，未初始化，泄漏的原理是让编译器在程序真正使用的内存周围加上额外的内存 **(Red Zone)**， 并在这些 Red Zone 里填上不同的值。

![various red zones](/images/sanitizer/redzones.png)

要完成这些需要让程序链接一个叫 **`libasan.so`** 的运行时动态库，以及在编译和链接时指定 `-fsanitize=address`。

- AddressSanitizer (ASan `-fsanitizer=address`):
    - use-after-free
    - double-free
    - buffer (heap, stack, and global buffer) overflows
    - memory leak

```cpp
struct Foo {
    int a[100];
};

int main() {
    Foo *foo = new Foo;

    return 0;
}
```

![Leak Sanitizer](/images/sanitizer/leak-sanitizer.png)

- MemorySanitizer (MSan `-fsanitizer=memory -fsanitizer-memory-track-origins`):
    - uninitialized memory reads

# 死锁

- ThreadSanitizer: data races, deadlocks

# 未定义行为

- UndefinedBehaviorSanitizer (UBSan `-fsanitizer=undefined`)
    - signed integer overflow
    - use-of-null-pointer
    - division by zero

# 参考

- [Be wise, sanitize: Keeping your C++ code free from bugs](https://microblink.com/be-wise-sanitize-keeping-your-c-code-free-from-bugs/)
- [AddressSanitizer flags](https://github.com/google/sanitizers/wiki/AddressSanitizerFlags)
- [An LLVM developer setup: Modern C++ development tools](https://llvm.org/devmtg/2016-01/slides/ModernCplusplusDevelopment.pdf)
