---
title: OpenGL Extensions
date: 2021-04-22 10:47:45
tags: [OpenGL]
categories: graphics
---

# Talk About
- OpenGL Extensions有哪些分类？
- Conformant OpenGL implementation是什么?
- OpenGL Extensions和OpenGL Versions有什么关系？
- OpenGL Extensions和conformant OpenGL implementation有什么关系？

<!--more-->

# OpenGL Extensions Category
严格来说，OpenGL Extensions可以分为三类，但也可以分为两类。

## 如果按照Khronos的[Extensions Name Convention](https://www.khronos.org/registry/OpenGL/docs/rules.html#spec_naming)可以分为以下三类：

- vendor-specific ("NV", "ATI", etc)
- multivendor (EXT)
- Khronos-approved ("ARB", "OES", "OML")

具体说， Khronos的Extension命名规则是`api_category_name`, 对于**OpenGL** API来说`api`就是`GL`, `category`就是上面的三类，对于第三类**Khronos-approved** Extensions, `category`还会细分

- "ARB"或"KHR"专门指**OpenGL** Extensions
- "OES"专门指**OpenGL ES** Extensions
- "OML"专门指**OpenML** Extensions

## 如果按照Khronos的[OpenGL Registry Extension Specifications](https://www.khronos.org/registry/OpenGL/index_gl.php)可以分为以下两类:

- [OpenGL ARB Extensions Specifications](https://www.khronos.org/registry/OpenGL/index_gl.php#arbextspecs)
- [OpenGL Vendor and EXT Extension Specifications](https://www.khronos.org/registry/OpenGL/index_gl.php#arbextspecs)

## Khronos-approved Extensions
这里重点理一下**Khronos-approved** Extensions(或者**"GL\_ARB\_*"**和**"GL\_KHR\_*"**), 因为Khronos-approved Extensions与下面两个问题密切相关：

- OpenGL Extensions和OpenGL Versions有什么关系？
- OpenGL Extensions和conformant OpenGL implementation有什么关系？

先看一下[OpenGL 4.6 Core Profile Specification](https://www.khronos.org/registry/OpenGL/specs/gl/glspec46.core.pdf)
### K.3 ARB and Khronos Extensions

{% blockquote %}
OpenGL extensions that have been approved by the Khronos OpenGL Architectural Review Board Working Group(ARB), or jointly approved by the ARB and the Khronos OpenGL ES Working Group(KHR), are summarized in this section. ARB and KHR extensions are NOT required to be supported by a CONFORMANT OpenGL implementation, but are expected to be widely available; they define functionality that is likely to move into the REQUIRED feature set in a future revision of the specification.
{% endblockquote %}

所有被ARB批准的，或者被ARB和KHR联合批准的OpenGL Extensions被罗列在本章节。这些Extensions不是一个**Conformant** OpenGL实现所必须支持的，但它们应该被大多数OpenGL实现都支持；它们所定义的功能很有可能在未来的OpenGL Specification里加到**必须支持的特性集**中。

### K.3.1 Naming Conventions
为了区分**ARB**和**KHR** extensions和core OpenGL extensions(required core features), 以及vendor-specific extensions, 有以下extension命名规范：
- 每个Khronos-approved extension都有一个唯一的名字字符串"GL_ARB_name"或"GL_KHR_name", 如果某个OpenGL实现支持某个extension,那么这个extension的名字字符串就要出现在调用`glGetStringi(EXTENSIONS);`的返回值里。
- 所有在这个extension里定义的函数，函数名都以`FunctionARB`或`FunctionKHR`命名。
- 所有在这个extension里定义的枚举，枚举名都以`NAME_ARB`或`NAME_KHR`命名。


### K.3.2 Promoting Extensions to Core Features
Khronos-approved, multivendor, 以及vendor-specific extensions都可以被_晋升_(_promoted_)加入到OpenGL核心特性集(required core features). 一旦被加入，相应的extension specification将被合入到core specification. 在这个extension里定义的函数和枚举也将_删除_它们的**ARB**, **KHR**, **EXT**, 或者**vendor后缀**。而且支持这些被_晋升_的extension的OpenGL实现应该继续支持带后缀的函数名和枚举名。

# What Is The Relationship Between OpenGL Extensions And OpenGL Versions?
下面这张图展示各种OpenGL extensions集合的关系。只有那些**Required Core Features(Extensions)**才会被纳入到各个**OpenGL Core Profile Specification**， 也即OpenGL versions是通过不断加入新的Required Core Features来升级的。

<div align=center>{% asset_img extensions.png "venn diagram of extensions" %}</div>

# What Is Conformant OpenGL Implementation?
