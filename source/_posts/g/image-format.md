---
title: Image Format in OpenGL
date: 2021-10-25 17:05:33
tags: [OpenGL]
categories: graphics
---

# Color Format

# Depth Format

## Normalized Integer

事实上，这里的Normalized Integer是`Unsigned Normalized Integer`, 意思是对于一个n-bit数，所有位是0表示0.0f, 所有位是1表示1.0f, 将所有整数的二进制表示平均映射到`[0, 1]`的深度值区间。例如，2-bit的UNORM Integer数有下面的映射关系: 

| Integer Bits  | Depth Values  |
|:--------------|:--------------|
| 00            | 0.0f          |
| 01            | 1/3           |
| 10            | 2/3           |
| 11            | 1.0f          |

- `GL_DEPTH_COMPONENT16`
- `GL_DEPTH_COMPONENT24`
- `GL_DEPTH_COMPONENT32`

## Floating-point

- `GL_DEPTH_COMPONENT32F`

# Depth/Stencil Format

