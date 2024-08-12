---
title: C++ 标准库
date: 2024-08-12 16:07:50
tags: c++
categories: programming
---

# [std::allocator](https://en.cppreference.com/w/cpp/memory/allocator)

```cpp
template< class T > struct allocator;
```

<!--more-->

`std::allocator`本质上就是 new/delete, 但是为什么需要它呢？

- 分离 allocation 和 construction(或者分离 deallocation 和 destruction)
- 主要的使用场景就是在实现container时

# 参考
- [What's advantage of using std::allocator instead of new in C++?](https://stackoverflow.com/questions/31358804/whats-the-advantage-of-using-stdallocator-instead-of-new-in-c)
