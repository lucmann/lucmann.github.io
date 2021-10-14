---
title: C++ Template
date: 2021-10-14 16:07:50
tags: gp
categories: cpp
---

# C++中的模板

C++中的模板可分为class template和function template. 它们之间存在不同，例如，function template不能partially specialized(偏特化)

<!--more-->

```cpp
template <class T, class U> T f(U obj); // Primary template
template <class U> void f<void, U>(U obj); // Illegal
```

# 模板的实例化(Instantiation)

模板的实例化又分为Explicit Instantiation和Implicit Instantiation.

## Explicit Instantiation

```cpp
template<typename T>
void f(T s)
{
    std::out << s << '\n';
}  // Primary template definition

template void f<double>(double); // explicit instantiation
template void f<>(char);  // explicit instantiation, template args deduced
template void f(int);  // explicit instantiation, template args deduced
```

## Implicit Instantiation

# 模板的特化(Specialization)

```cpp
template<typename T>
inline bool isBoolean(void) { return false; } // Primary template definition

template<> inline bool isBoolean<bool>(void) { return true; } // template specialization
```

# `typename` vs `class`

大多数情况下，`typename`和`class`可以互换使用，但它们也有不能互换的时候，下面的情况只能使用`typename`关键字:

```cpp
template<class Option>
struct OptTraits {
    typedef typename Option::ValueType ValueType;
};  // Indicates Option::ValueType is a type
```

# Constraints & Rules

[1]Explicit instantiation时，如果可以从function parameter推导出类型，可以省去template args.
[2]Function template或class template的member function的Explicit实例化不能使用`inline`和`constexpr`关键字。
[3]Different template instantiations are distinct types.

