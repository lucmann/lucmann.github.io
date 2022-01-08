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

# Frequently Asked Questions

- 在 C++11 之前，编译器可能会为一个类创建的函数有哪些?

- 如果要阻止一个类的对象被复制(copyable), 有哪些做法?

    1. 将 copy 构造函数和 copy assignment 操作符声明为 private, 并不予实现。
    2. 实现一个 base class, 这个 base class 的 copy 构造函数和 copy assignment 操作符都为 private。

- C++ Template parameters 有哪几种?

    1. non-type template parameter

        - lvalue reference type
        - integral type
        - pointer type
        - pointer member type
        - enumeration type
        - std::nullptr_t (自从C++11)
        - floating type (自从C++20)

    2. type template parameter
    3. template template parameter

- C++ Template 定义为什么通常都在头文件中?

    因为 Template 定义必须在模板隐式实例化([implicit instantiation](https://lucmann.github.io/p/cpp-template/))之前，通常我们在`.cpp` 源文件里直接使用模板都属于隐式实例化。所以将模板定义在头文件中，可以有效防止模板实例化之前没有定义。

- `std::endl` 和 `\n` 有何区别？

    对 `std::endl` 的每次调用都会 flush output buffer
