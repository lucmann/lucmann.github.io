---
title: ARB_explicit_uniform_location
date: 2021-03-18 14:14:04
tags: [GL]
categories: media
---

# [Overview](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_explicit_uniform_location.txt)

Numbers **ARB Extension #128**

<!--more-->

Requires **OpenGL 3.3 or [ARB_explicit_attrib_location](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_explicit_attrib_location.txt)**

Interacts **ARB_shader_subroutine**

Since **OpenGL 4.3 Core Profile Specification**

这个扩展相关的话题是**Uniform Variable**。它给应用程序提供一个可以预先分配默认uniform块中的uniform变量的uniform location的方法，也包括subroutine中的uniform变量。这样应用程序就可以不通过调用类似下面的命令去查询uniform location而直接修改uniform变量的值。

```
GetUniformLocation
GetSubroutineUniformLocation
GetSubroutineIndex
```
