---
title: 关于C++应该知道的那些事
date: 2020-07-24 17:02:49
tags: c++
categories: programming
---

# 缩略语 (Abbreviation)

- RAII

Resource Acquisition Is Initialization

[知乎-RAII原理](https://zhuanlan.zhihu.com/p/34660259)

<!--more-->

- RTTI

Runtime Type Identification

[CSDN-RTTI原理](https://blog.csdn.net/ljianhui/article/details/46487951)

- SFINAE

Substitution Failure Is Not An Error

意思是在函数模板或类模板（包括类模板特化）类型推导时，由于类型或值替换导致的Ill-formed code(Subsititution Failure)不被作为一个编译时错误。

# 

- Don't declare objects `const` if you want to move from them.
- Using `std::move` doesn't guarantee anything will be moved.
