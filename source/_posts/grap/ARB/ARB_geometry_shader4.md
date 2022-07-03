---
title: Geometry Shader
date: 2021-12-28 11:45:52
tags: OpenGL
categories: graphics
---

# Geometry Shader Input Layout Qualifiers

Geometry shader只能在接口限定符(interface qualifier)**in**前加Input Layout Qualifiers, 不能在输入块(input block), 块成员(block member), 或变量声明(variable declaration)前加Input Layout Qualifiers。

这些Input Layout Qualifiers有

<!--more-->

- **points**
- **lines**
- **lines_adjacency**
- **triangles**
- **triangles_adjacency**

[GL_ARB_gpu_shader5](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_gpu_shader5.txt)(4.0)增加了对instanced geometry shader的支持，这个扩展引入了一个新的Input Layout Qualifier

- **invocation** = *num_instances*

注意，这里的instanced和instanced rendering是两码事，这里的instanced是专门针对GS说的，意思是GS可以对同一个输入图元执行多次。对一个输入图元的每次调用(invocation)都会有一个不同的`gl_InvocationID`值。请注意这里的"**一个图元**", 如果你有两个图元，`num_instances = 3`, 那么GS被执行的顺序和`gl_InvocationID`的值是这样的

0. (prim0, inst0)
1. (prim0, inst1)
2. (prim0, inst2)
3. (prim1, inst0)
4. (prim1, inst1)
5. (prim1, inst2)

这就是"对一个输入图元的每次调用`gl_InvocationID`都会有一个不同的值"的含义。

# Geometry Shader Output Layout Qualifiers

Geometry shader只能在接口限定符**out**前加Output Layout Qualifiers, 不能在输出块(output block)或变量声明前加Output Layout Qualifiers。

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

# Geometry Shader Built-in Functions

- `void EmitVertex()`

    向当前的输出图元增加一个顶点，也即表示一个顶点完成了。增加一个顶点的意思是，使用 GS 中输出变量的当前值做为这个顶点的相关值，一旦 `EmitVertex()` 返回后，这些输出变量的值都将变成未定义的，这些输出变量包括

    * `gl_Position` (vec4)
    * `gl_PointSize` (float)
    * `gl_ClipDistance` (float)
    * `gl_PrimitiveID` (integer)
    * `gl_Layer` (integer)

    前3个其实就是 `gl_PerVertex`

- `void EndPrimitive()`

    表示当前的图元输出完成，开始由后续的 `EmitVertex()` 输出下一个图元。如果 GS 只写一个图元，可以不用调 `EndPrimitive()`。
