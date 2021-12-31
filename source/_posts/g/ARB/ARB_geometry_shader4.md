---
title: Geometry Shader
date: 2021-12-28 11:45:52
tags: OpenGL
categories: graphics
---

# Geometry Shader Output Layout Qualifiers

Geometry shader只能在接口限定符(interface qualifier)**out**前加Output Layout Qualifiers, 不能在输出块(output block)或变量声明(variable declaration)前加Output Layout Qualifiers。

<!--more-->

这些Output Layout Qualifiers有

- **points**
- **line_strip**
- **triangle_strip**
- **max_vertices** = *integer-constant*

# Geometry Shader Built-in Variables

```
in gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
} gl_in[];

in int gl_PrimitiveIDIn;

out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
    float gl_ClipDistance[];
};

out int gl_PrimitiveID;
out int gl_Layer;
```
