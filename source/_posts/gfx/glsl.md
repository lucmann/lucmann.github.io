---
title: OpenGL Shading Languages
date: 2024-08-22 16:16:12+08:00
tags: GLSL
categories: graphics
---

GLSL 是一种高级着色语言，语法接近 C 语言。它由 OpenGL ARB 创建，旨在让开发者能够更直接地控制图形管线，而无需使用 ARB 汇编语言或特定硬件语言。

实际上 GLSL 包括大概 6 种着色语言:

- 顶点着色语言 (.vert)
- 细分控制语言 (.tesc) 
- 细分评估语言 (.tese) 
- 几何着色语言 (.geom) 
- 片段着色语言 (.frag) 
- 计算着色语言 (.comp) 

<!--more-->

# [glslangValidator](https://github.com/KhronosGroup/glslang)

glslangValidator 是一个 GLSL/ESSL 的编译前端，同时也是一个 SPIR-V 的生成器。

```shell
/bin/glslangValidator \
  -V /home/luc/gh/VulkanExamples/data/shaders/deferredmultisampling/debug.frag \
  -o /home/luc/gh/VulkanExamples/data/shaders/deferredmultisampling/debug.frag.debug.spv
```


# 顶点着色语言

```
layout (location = 0) out vec3 outUV;

outUV = vec3((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2, 0.0);
gl_Position = vec4(outUV.st * 2.0f - 1.0f, 0.0f, 1.0f);
```

- `gl_VertexIndex` 和 `gl_Position` 都是顶点着色语言内置变量(无需用户声明)
- `out vec3 outUV` 说明 `outUV` 会从本阶段输出到下一个着色阶段
- `layout (location = 0)` 为这个输出变量指定一个位置(或槽位), 这样即使在与下一阶段着色器的输入变量名字不同，但只要它也声明同样的 location, 那么变量就可以准确传入下一阶段的着色器

这段着色器代码巧妙地利用 `gl_VertexIndex` 生成了顶点的 position 属性。

| gl_VertexIndex | outUV                     | gl_Position              |
|:---------------|:--------------------------|:-------------------------|
| 0              | (0, 0, 0)                 | (-1, -1, 0, 1)           |
| 1              | (2, 0, 0)                 | ( 3, -1, 0, 1)           |
| 2              | (0, 2, 0)                 | (-1,  3, 0, 1)           |
| 3              | (2, 2, 0)                 | ( 3,  3, 0, 1)           |

# 函数
## texelFetch

- `vec4 texelFetch(sampler2D sampler, ivec2 P, int lod);`

与普通的 `texture` 函数不同，`texelFetch` 使用的是未归一化的坐标直接访问纹理中的纹素，有点 `gl_FragCoord` 的意思

## textureSize

- `ivec2 textureSize(sampler2D sampler, int lod);`

获取纹理的尺寸，常用来将归一化的纹理坐标转换为像素坐标

