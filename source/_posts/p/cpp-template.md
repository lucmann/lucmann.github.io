---
title: C++ Template
date: 2021-10-14 16:07:50
tags: gp
categories: language
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

# 模板特化(template specialisation)

模板特化是C++泛型编程的一个重要技术，它的一个典型应用就是type trait的实现

## 全特化(full specialisation)

```cpp
template <typename T>
struct is_boolean {
    static const bool value = false;
}; // primary template

template <>
struct is_boolean<bool> {
    static const bool value = true;
}; // full specialization
```

## 偏特化(partial specialisation)

虽然模板类的全特化是一个重要的泛型编程技术，但有时候我们可能需要一个介于完全泛化和全特化之间的一个特化版本，这就是偏特化。下面的例子`is_pointer<T>`，这里我们需要一个完全泛化的模板版本来处理所有`T`不是一个指针类型的情形，而需要一个偏特化版本来处理所有`T`是一个指针类型的情形。

```cpp
template <typename T>
struct is_pointer {
    static const bool value = false;
}; // primary version that handles all the cases where T is not a pointer

template <typename T>
struct is_pointer<T*> {
    static const bool value = true;
}; // partial specialisation to handle all the cases where T is a pointer
```

从上面两个例子`is_boolean<T>`和`is_pointer<T>`可以看出，全特化和偏特化的语法不是有明显区别的，偏特化在类名后面有一个额外的包含模板参数的`<>`, 而全特化版本的类名后面的`<>`里不是模板参数，而是已经特化的具体类型名。

下面的例子是一个更复杂的偏特化的例子`remove_bounds<T>`, 这个模板类仅仅定义一个与`T`类型相同的typedef成员类型，但是高层的数组关联被移除了，这样的模板类可以完成一个类型的转化

```cpp
template <typename T>
struct remove_bounds {
    typedef T type;
}; // primary version

template <typename T, std::size_t N>
struct remove_bounds<T[N]> {
    typedef T type;
}; // partial specialisation
```

从这个例子可以看出:

- 偏特化版本的模板参数的个数不一定和默认模板的相等
- 偏特化版本的类名后面的模板参数的个数和类型必须匹配默认模板的参数的个数和类型

# `typename` vs `class`

大多数情况下，`typename`和`class`可以互换使用，但它们也有不能互换的时候，下面的情况只能使用`typename`关键字:

```cpp
template<class Option>
struct OptTraits {
    typedef typename Option::ValueType ValueType;
};  // Indicates Option::ValueType is a type
```

# template parameters vs template arguments

template parameters之于template arguments相当于函数的形参之于实参。C++模板的通用语法是:

```
template <parameter-list> declaration
```

parameter-list中的每个形参都可以是下列3种中的任何一种:

- non-type template parameter
- type template parameter
- template template parameter

## 模板类的模板构造函数

模板类的模板构造函数，意思是一个模板类的一个构造函数也是一个模板函数。这种情况有两点需要注意:

- 模板类的模板参数名不能和构造函数的模板参数名相同 (*template parameter* cannot share the same name)
- 不能显式地指定模板构造函数的模板实参 (cannot explicitly specify *template argument* for constructor)

这种情况可以很好的体现template parameter和template argument的区别，请看下面的例子:

```cpp

template<typename OptName>
struct Option {
    typedef typename OptName::ValueType ValueType;

    ...

    // Constructor
    template<size_t NumNamedValues>
    Option(const char* shortName_
         , const char* longName_
         , const char* description_
         , const NamedValue<ValueType> (&namedValues_)[NumNamedValues]
         , const char* defaultValue_ = DE_NULL)
         : shortName(shortName_)
         , longName(longName_)
         , description(description_)
         , defaultValue(defaultValue_)
         , parse((ParseFunc)DE_NULL)
         , namedValues(DE_ARRAY_BEGIN(namedValues_))
         , namedValuesEnd(DE_ARRAY_END(namedValues_))
    {
    }
};

static const NamedValue<bool> s_enableNames[] = {
    { "enable", true },
    { "disable", false }
};

// Instantiation to call constructor above
Option<TestOOM> opt = Option<TestOOM>(DE_NULL, "deqp-test-oom", "Run tests that exhaust memory", s_enableNames, "disable");
```

从这个例子看到模板构造函数被调用时本该指定的non-type template argument(size_t) `NumNamedValues`并没有被指定, 原因是模板函数的template argument list应该紧跟在函数名后面，但是构建函数模板(类型转换函数模板conversion member function template也是同样情况)被调用的时候**不使用函数名**，所以没有一种方法可以显式地指定

- constructor member function template
- conversion member function template

的template argument list.

# Constraints & Rules

- Explicit instantiation时，如果可以从function parameter推导出类型，可以省去template args.
- Function template或class template的member function的Explicit实例化不能使用`inline`和`constexpr`关键字。
- Different template instantiations are distinct types.

