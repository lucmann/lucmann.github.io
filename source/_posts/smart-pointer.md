---
title: Smart Pointer in C++
date: 2020-07-04 10:56:25
tags: [C++]
---

Smart Pointer是C++11引入的，被包含在C++标准库中。

# `shared_ptr`

shared_ptr主要功能是可以在指针指向的对象在任何地方都不再被使用的时候自动销毁所指对象。C++引入它的目的是消除内存泄漏(memory leak)和野指针(dangling pointer). 从实现的角度看，shared_ptr是通过消耗更多的内存来换取程序的健壮性(没有内存泄漏).

每个shared_ptr对象的内部都指向两块内存区域:

- Pointer to object
- Pointer to control data that is used for reference counting

<div align=center>{% asset_img "shared_ptr.png" "shared_ptr memory layout" %}</div>