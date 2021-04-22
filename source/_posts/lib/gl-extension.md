---
title: OpenGL Extensions
date: 2021-04-22 10:47:45
tags: [GL]
categories: lib
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

具体说， Khronos的Extension命名规则是`api_category_name`, 对于**OpenGL** API来说`api`就是`GL`, `category`就是上面的三
类，对于第三类**Khronos-approved** Extensions, `category`还会细分

- "ARB"或"KHR"专门指**OpenGL** Extensions
- "OES"专门指**OpenGL ES** Extensions
- "OML"专门指**OpenML** Extensions

## 如果按照Khronos的[OpenGL Registry Extension Specifications](https://www.khronos.org/registry/OpenGL/index_gl.php)可以分为以下两类:

- [OpenGL ARB Extensions Specifications](https://www.khronos.org/registry/OpenGL/index_gl.php#arbextspecs)
- [OpenGL Vendor and EXT Extension Specifications](https://www.khronos.org/registry/OpenGL/index_gl.php#arbextspecs)

## Khronos-approved Extensions
这里重点理一下**Khronos-approved** Extensions(或者**"GL_ARB_*"**和**"GL_KHR_*"**), 因为Khronos-approved Extensions与下面两个问题密切相关：

- OpenGL Extensions和OpenGL Versions有什么关系？
- OpenGL Extensions和conformant OpenGL implementation有什么关系？

先翻译一段[OpenGL 4.6 Core Profile Specification](https://www.khronos.org/registry/OpenGL/specs/gl/glspec46.core.pdf) **K.3 ARB and Khronos Extensions** 章节

{% blockquote %}
OpenGL extensions that have been approved by the Khronos OpenGL Architectural Review Board Working Group(ARB), or jointly approved by the ARB and the Khronos OpenGL ES Working Group(KHR), are summarized in this section. ARB and KHR extensions are NOT required to be supported by a CONFORMANT OpenGL implementation, but are expected to be widely available; they define functionality that is likely to move into the REQUIRED feature set in a future revision of the specification.
{% endblockquote %}

所有被ARB批准的，或者被ARB和KHR联合批准的OpenGL Extensions被罗列在本章节。这些Extensions不是一个**Conformant** OpenGL实现所必须支持的，但它们应该被大多数OpenGL实现都支持；它们所定义的功能很有可能在未来的OpenGL Specification里加到**必须支持的特性集**中。

