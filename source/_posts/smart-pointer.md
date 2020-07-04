---
title: Smart Pointer in C++
date: 2020-07-04 10:56:25
tags: [C++]
---

Smart Pointer是C++11引入的，被包含在C++标准库中。常见的smart pointer有: 

- `std::shared_ptr`
- `std::unique_ptr`
- `std::weak_ptr`

# `shared_ptr`

shared_ptr主要功能是可以在指针指向的对象在任何地方都不再被使用的时候自动销毁所指对象。C++引入它的目的是消除内存泄漏(memory leak)和野指针(dangling pointer). 从实现的角度看，shared_ptr是通过消耗更多的内存来换取程序的健壮性(没有内存泄漏).

每个shared_ptr对象的内部都指向两块内存区域:

- Pointer to object
- Pointer to control data that is used for reference counting

<div align=center>{% asset_img "shared_ptr.png" "shared_ptr memory layout" %}</div>

## creation

- binding a `shared_ptr` object with raw pointer

  `std::shared_ptr<int> p1(new int());`

- using `std::make_shared`

  `std::shared_ptr<int> p1 = std::make_shared<int>();`

## reference counting

`p1.use_count();`

## detachment

- calling `reset()` with no parameter

  `p1.reset();`

  这个调用将`p1`的reference count减1， 如果reference count变成0，则自动删除`p1`关联的raw pointer.

- calling `reset()` with parameter

  `p1.reset(new int(42));`

  这个调用将`p1`关联到一个新的raw pointer, 因此`p1`的reference count还是1.

- using `nullptr`

  `p1 = nullptr;`

## psuedo pointer

  

  