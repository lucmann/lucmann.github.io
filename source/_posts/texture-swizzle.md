---
title: ARB_texture_swizzle
date: 2020-06-30 15:52:24
tags: GL
categories: [CG]
---

# 概述(#84)
传统的OpenGL纹理格式(texture format)的纹理数据存储和解释是统一的，纹理就是被解释成颜色。而在现代OpenGL应用中，大多数纹理并不是代表颜色，而是代表像shadow maps, normal maps, page tables, occlusion data等等这样的数据。对于后者，我们将纹理数据称作"RGBA"，只是对数据在现有模型上的一种方便的映射，但并不是数据实际上的准确解释。

<!--more-->

已有的纹理格式几乎是纹理数据的类型，分量的个数，分量的位数的集合的正交组合，但是对于着色器或固定管线所读取的数据如何解解并不那么正交。之前的扩展也增加了一些最迫切要求的纹理格式，但问题没有被完全解决。

ARB_texture_swizzle提供了一种被使用之前或者被着色器读取之前，重排一个纹理像素的分量的机制。比方说可以设置让shader读取纹理数据时只取Red分量。
