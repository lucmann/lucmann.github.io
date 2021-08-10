---
title: Draw Commands
date: 2020-09-15 16:44:37
tags: [GL]
categories: lib
---

# Category
OpenGL中的Draw Commands是一组生成GPU渲染Command Stream的API，我们可以将它们简单分为4类:

| Drawing   | 适用场景                       | Vertex Attributes Buffer Object Binding类型 |
|:----------|:-------------------------------|:--------------------------------------------|
| Array     | 普通                           | GL_ARRAY_BUFFER                             |
| Indexed   | 重复的顶点                     | GL_ELEMENT_ARRAY_BUFFER                     |
| Instanced | 重复的模型(Instance/Model)     | GL_ARRAY_BUFFER<br>GL_ELEMENT_ARRAY_BUFFER  |
| Indirect  | Drawing命令的参数直接放在GPU   | GL_DRAW_INDIRECT_BUFFER                     |

<!--more-->

Array Drawing是最基本的Draw命令，其它3类都是从它衍生来的，为了某种绘制便利或顶点复用对Array Drawing API进行扩展，从而得到相应的索引绘制、实例绘制、间接绘制。

# Array Drawing
```c
void glDrawArrays(GLenum mode,
                  GLint first,
                  GLsizei count);
```

`glDrawArrays`是OpenGL中最基本的绘制命令，`mode`接受的图元类型是下面的一个子集:

```c
/* Primitives */
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
#define GL_QUADS                          0x0007
#define GL_QUAD_STRIP                     0x0008
#define GL_POLYGON                        0x0009
#define GL_LINES_ADJACENCY                0x000A (since OpenGL 3.2) 
#define GL_LINE_STRIP_ADJACENCY           0x000B (since OpenGL 3.2)
#define GL_TRIANGLES_ADJACENCY            0x000C (since OpenGL 3.2)
#define GL_TRIANGLE_STRIP_ADJACENCY       0x000D (since OpenGL 3.2)
#define GL_PATCHES                        0x000E (since OpenGL 3.2)
```

例如顶点数组如下
```c
    vertices
   ----------
0 | (-1,  1) |
1 | (-1, -1) |
2 | ( 1, -1) |
3 | ( 1,  1) |
   ----------
```
用3个顶点绘制一个三角形，调用命令如下
```c
glDrawArrays(GL_TRIANGLES, 0, 3);
```

# Indexed Drawing

```c
void glDrawElements(GLenum mode,
                    GLsizei count,
                    GLenum type,
                    const void* indices);


void glDrawElementsBaseVertex(GLenum mode,
                              GLsizei count,
                              GLenum type,
                              void* indices
                              GLint basevertex);
```

`glDrawElements`不是直接用顶点数组去绘制，而是用顶点数组的索引去绘制，例如还是用3个顶点绘制一个三角形
```c
    vertices
   ----------
0 | (-1,  1) |
1 | (-1, -1) |
2 | ( 1, -1) |
3 | ( 1,  1) |
   ----------
```

这次除了顶点数组外，我们还要上传一个顶点索引数组
```c
  indices
   -----
0 |  0  |
1 |  1  |
2 |  2  |
   -----
```
调用命令如下

```c
glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, &indices);
```
当然上面这个只有3个顶点的例子体现不出来`glDrawElements`的复用顶点的好处，但是在三角形很多且这些三角形的顶点有很多重复的情形下就不同了，它的优势就体现出来了:
- 虽然我们额外增加了一个索引数组indices去存储所绘制顶点的索引，但相比一个顶点最多4个float类型的数据量来说，用一个unsigned byte来表示一个顶点还是划算的。
- 在顶点坐标一样，只需改变顶点顺序的图元绘制中，将大大减少数据量。

`glDrawElementsBaseVertex`的行为和`glDrawElements`一样，除了在所取到的索引数组相应元素值上加basevertex. 考虑如下情况:

```c
    vertices                indices
   ----------                -----
0 | (-1,  1) |            0 |  0  |
1 | (-1, -1) |            1 |  1  |
2 | ( 1, -1) |            2 |  2  |
3 | ( 1,  1) |            3 |  3  |
   ----------             4 |  0  |
                          5 |  2  |
                             -----
```

这种情况下，我们一般会调用
```c
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &indices);
```

但是如果所索引的顶点不在顶点数组的开始，而是从第100个顶点开始
```c
      vertices                indices2
     ----------                -----
        ....                0 | 100 |
100 | (-1,  1) |            1 | 101 |
101 | (-1, -1) |            2 | 102 |
102 | ( 1, -1) |            3 | 103 |
103 | ( 1,  1) |            4 | 100 |
        ....                5 | 102 |
     ----------                -----
```

这种情况下原来的indices不能用了，只能再创建并绑定一个新的VBO `indices2`, 而创建绑定VBO是内存操作开销很大，`glDrawElementsBaseVertex`就是解决这个问题的。我们只需要在原来的索引值上加一个offset
```c
glDrawElementsBaseVertex(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, &indices, 100);
```

# Instanced Draw
```c
void glDrawArraysInstanced(GLenum mode,
                           GLint first,
                           GLsizei count,
                           GLsizei instancecount);

void glDrawArraysInstancedBaseInstance(GLenum mode,
                                       GLint first,
                                       GLsizei count,
                                       GLsizei instancecount,
                                       GLuint baseinstance);

void glDrawElementsInstanced(GLenum mode,
                             GLsizei count,
                             GLenum type,
                             const void* indices,
                             GLsizei instancecount);

void glDrawElementsInstancedBaseInstance(GLenum mode,
                                         GLsizei count,
                                         GLenum type,
                                         const void* indices,
                                         GLsizei instancecount,
                                         GLuint baseinstance);

void glDrawElementsInstancedBaseVertex(GLenum mode,
                                       GLsizei count,
                                       GLenum type,
                                       void* indices,
                                       GLsizei instancecount,
                                       GLint basevertex);
```

要理解上面这些OpenGL Instanced Draw命令，首先我们要理解一个OpenGL里并不存在的Draw命令

```
void glDrawArraysOneInstance(GLenum mode,
                             GLint first,
                             GLsizei count,
                             GLint instance,
                             GLuint baseinstance);
```

Instanced Drawing简单说就是一次Draw Call绘制多个实例，比如一个布满树叶的场景，每片树叶的顶点数据可能是相似的，可能就是在世界坐标系中的位置不同，其它顶点属性可能都相同。那么Instanced Drawing是如何复制相同的实例到不同的位置上的呢?

OpenGL主要通过以下两个变量控制Instanced Drawing

- gl_InstanceID (Vertex Shader)
- divisor (glVertexAttribDivisor)

## gl_InstanceID
gl_InstanceID是Vertex Shader里的内置变量，如果你这样调用

```c
glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 100);
```

那么`gl_InstanceID`的取值范围就是[0, 99]

## divisor

假设location为2的Vertex Attribute用来设置每个实例的位置偏移，shader如下:

```c
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

out vec3 fColor;

void main()
{
    gl_Position = vec4(aPos + aOffset, 0.0, 1.0);
    fColor = aColor;
        
}
```

我们可以通过下面的API告诉OpenGL, 每画一个实例更新一下index=2这个属性，而不是默认的，每一个顶点更新一次属性。

```c
glEnableVertexAttribArray(2);
glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
glBindBuffer(GL_ARRAY_BUFFER, 0);
glVertexAttribDivisor(2, 1);
```

# Indirect Drawing

Indirect Drawing是将Array/Indexed Drawing命令的参数存入专门的Buffer Object，也就是GPU Storage里，这里的Buffer Object的绑定类型是`GL_DRAW_INDIRECT_BUFFER`。之所以这样做，是为了能让GPU直接写回这些值，比方Compute Shader, 或者为Transform Feedback设计的Geometry Shader, 亦或是OpenCL/CUDA kernel函数。这样避免了这些参数在GPU和CPU之间来回地复制(round-trip)

## DrawArraysIndirect

```c
void glDrawArraysIndirect(GLenum mode, const void *indirect);
```

在OpenGL ES 3.1及以上，DrawArraysIndirect的Draw Parameters被定义成下面的结构体:

```c
typedef struct {
    uint count;
    uint instanceCount;
    uint first;
    uint reservedMustBeZero;
} DrawArraysIndirectCommand;
```

在OpenGL 4.0及以上，DrawArraysIndirect的Draw Parameters被定义成下面的结构体:

```c
typedef struct {
    uint count;
    uint instanceCount;
    uint first;
    uint baseInstance;
} DrawArraysIndirectCommand;
```

这所以在OpenGL ES和OpenGL里有`baseInstance`的区别，是因为在OpenGL ES中没有下面的draw command:

```c
void glDrawArraysIntancedBaseInstance(GLenum mode,
				      GLint first,
				      GLsizei count,
				      GLsizei instancecount,
				      GLuint baseinstance);
```

因此在OpenGL ES中，`glDrawArraysIndirect`相当于

```c
DrawArraysIndirectCommand *cmd = (DrawArraysIndirectCommand *)indirect;
DrawArraysInstanced(mode, cmd->first, cmd->count, cmd->instanceCount);
```

而在OpenGL中，`glDrawArraysIndirect`相当于

```c
DrawArraysIndirectCommand *cmd = (DrawArraysIndirectCommand *)indirect;
DrawArraysInstancedBaseInstance(mode, cmd->first, cmd->count,
                cmd->instanceCount, cmd->baseInstance);
```

## DrawElementsIndirect

```
void glDrawElementsIndirect(GLenum mode, GLenum type, const void *indirect);
```

在ES 3.1中, DrawElementsIndirect的Draw Parameters被定义成下面这个结构体:

```c
typedef struct {
    uint count;
    uint instanceCount;
    uint firstIndex;
    int  baseVertex;
    uint reservedMustBeZero;
} DrawElementsIndirectCommand;
```

而在OpenGL 4.0及以上, DrawElementsIndirect的`indirect`指向的Draw Parameters被定义成下面的结构体:

```c
typedef struct {
    uint count;
    uint primCount;
    uint firstIndex;
    uint baseVertex;
    uint baseInstance;
} DrawElementsIndirectCommand;
```

# References

[[1] https://learnopengl.com/Advanced-OpenGL/Instancing](https://learnopengl.com/Advanced-OpenGL/Instancing)
