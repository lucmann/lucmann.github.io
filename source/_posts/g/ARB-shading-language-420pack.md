---
title: ARB_shading_language_420pack
date: 2021-03-18 19:08:34
tags: [OpenGL]
categories: lib
---

# [Overview](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_shading_language_420pack.txt)
Numbers **ARB Extension #108**

<!--more-->

Interacts
- [ARB_shader_image_load_store](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_shader_image_load_store.txt)

Since **OpenGL 4.2 Core Profile Specification**

这个扩展是一个**Language feature only extension**, 即它完全只跟GLSL相关。它增加了以下改变
- 增加续行符`\`
- 把GLSL的字符集由ASCII改为UTF-8
- 允许隐式地将返回值类型转换为函数声明的类型
- **const**关键字可以在函数体内声明变量，同时允许使用一个非常量表达式初始化该变量
- 变量声明的修饰符不再必须有严格的顺序。**layout qualifier**可以被使用多次，多个**parameter qualifiers**可以被用，但并不是说变量声明可以有任意的初始化列表。只是说某一类的修饰符只可以用一个，它们之间的顺序限制被取消了。
- 增加layout qualifier identifier `binding`用来绑定一个uniform block的location. uniform block本身需要GLSL 1.4的支持。
- 增加layout qualifier identifier `binding`用来绑定units到sampler和image variable声明。
- 增加C语言风格的初始化列表，即 `a[10] = { 0 };`
- 增加vectors和matrices的`.length()`方法，返回vector的分量个数或matrix的列数。
- 允许标量的swizzle操作。
- 增加内置常量`gl_MinProgramTexelOffset`和`gl_MaxProgramTexelOffset`

