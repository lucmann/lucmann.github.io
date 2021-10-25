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

# Depth-Stencil Format

# Depth & Depth-Stencil Format Image 与 Shadow Samplers

Shadow Samplers

| GLSL sampler               | OpenGL texture target      | 
|:---------------------------|:---------------------------|
| sampler1DShadow            | GL_TEXTURE_1D              |
| sampler2DShadow            | GL_TEXTURE_2D              |
| samplerCubeShadow          | GL_TEXTURE_CUBE_MAP        |
| sampler2DRectShadow        | GL_TEXTURE_RECTANGLE       |
| sampler1DArrayShadow       | GL_TEXTURE_1D_ARRAY        |
| sampler2DArrayShadow       | GL_TEXTURE_2D_ARRAY        |
| samplerCubeArrayShadow     | GL_TEXTURE_CUBE_MAP_ARRAY  |

当一个纹理格式是Depth或Depth-Stencil格式时，它就不能再使用通常的采样器(Sampler), 而必须使用Shadow Sampler. 这种类型的Sampler Texture Lookup函数被重载成比通常的纹理坐标多一个component. 而且返回值永远是一个float标量值, 而且这个返回值的范围永远在[0, 1]之间，当纹理坐标的最后一个component(比通常多出的那个)与采样值比较通过时，返回值为1，如果比较失败，返回值为0，当有多个采样点时，分别进行比较，结果为比较通过占总采样点个数的比例，例如，如果是4个采样值，只有一个比较通过，则返回值是0.25.

```c
float texture(sampler1DShadow sampler, vec3 P[, float bias])
float texture(sampler2DShadow sampler, vec3 P[, float bias])
float texture(samplerCubeShadow sampler, vec4 P[, float bias])
float texture(sampler1DArrayShadow sampler, vec3 P[, float bias])
float texture(sampler2DArrayShadow sampler, vec4 P)
float texture(sampler2DRectShadow sampler, vec3 P)
float texture(samplerCubeArrayShadow sampler, vec4 P, float compare)
```

说明：

1. sampler1DShadow的纹理坐标的第二个component不使用
2. samplerCubeArray的Texture Lookup函数的纹理坐标本身就是一个4维的，所以samplerCubeArrayShadow的重载新加了一个单独用来比较的参数，避免使用5维向量。

