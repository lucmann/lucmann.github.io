---
title: Draw Commands
date: 2020-09-15 16:44:37
tags: [GL]
categories: lib
---

# Category
OpenGL中的Draw Commands是一组生成GPU渲染Command Stream的API，我们可以将它们简单分为4类:
- Basic Draw
- Indexed Draw
- Instanced Draw
- Indirect Draw

<!--more-->

Basic Draw是最基本的Draw命令，其它3类都是从它衍生来的，为了某种绘制便利或顶点复用对Basic Draw API进行扩展，从而得到相应的索引绘制、实例绘制、间接绘制。

## Basic Draw
```c
void glDrawArrays(GLenum mode,
                  GLint first,
                  GLsizei count);
```

`glDrawArrays`是OpenGL中最基本的绘制命令，它的第一个参数可接受的图元类型有:
- GL_POINTS
- GL_LINE_STRIP
- GL_LINE_LOOP
- GL_LINES
- GL_LINE_STRIP_ADJACENCY
- GL_LINES_ADJACENCY
- GL_TRIANGLE_STRIP
- GL_TRIANGLE_FAN
- GL_TRIANGLES
- GL_TRIANGLE_STRIP_ADJACENCY
- GL_TRIANGLES_ADJACENCY
- GL_PATCHES

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

## Indexed Draw
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

## Instanced Draw
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

## Indirect Draw
**Indirect Draw**有基本和Instanced Draw一样，除了那些**Draw Parameters**会被上传到由`indirect`指向的专门的`GL_DRAW_INDIRECT_BUFFER`缓冲区。
### DrawArraysIndirect
```
void glDrawArraysIndirect(GLenum mode, const void *indirect);
```

在OpenGL ES 3.1及以上，DrawArraysIndirect的Draw Parameters被定义成下面的结构体:
```
typedef struct {
    uint count;
    uint instanceCount;
    uint first;
    uint reservedMustBeZero;
} DrawArraysIndirectCommand;
```

在OpenGL 4.0及以上，DrawArraysIndirect的Draw Parameters被定义成下面的结构体:
```
typedef struct {
    uint count;
    uint instanceCount;
    uint first;
    uint baseInstance;
} DrawArraysIndirectCommand;
```

这所以在OpenGL ES和OpenGL里有`baseInstance`的区别，是因为在OpenGL ES中没有下面的draw command:
```
void glDrawArraysIntancedBaseInstance(GLenum mode,
				      GLint first,
				      GLsizei count,
				      GLsizei instancecount,
				      GLuint baseinstance);
```

因此在OpenGL ES中，`glDrawArraysIndirect`相当于
```
DrawArraysIndirectCommand *cmd = (DrawArraysIndirectCommand *)indirect;
DrawArraysInstanced(mode, cmd->first, cmd->count, cmd->instanceCount);
```

而在OpenGL中，`glDrawArraysIndirect`相当于
```
DrawArraysIndirectCommand *cmd = (DrawArraysIndirectCommand *)indirect;
DrawArraysInstancedBaseInstance(mode, cmd->first, cmd->count,
                cmd->instanceCount, cmd->baseInstance);
```

### DrawElementsIndirect
```
void glDrawElementsIndirect(GLenum mode, GLenum type, const void *indirect);
```

在ES 3.1中, DrawElementsIndirect的Draw Parameters被定义成下面这个结构体:
```
typedef struct {
    uint count;
    uint instanceCount;
    uint firstIndex;
    int  baseVertex;
    uint reservedMustBeZero;
} DrawElementsIndirectCommand;
```

而在OpenGL 4.0及以上, DrawElementsIndirect的`indirect`指向的Draw Parameters被定义成下面的结构体:
```
typedef struct {
    uint count;
    uint primCount;
    uint firstIndex;
    uint baseVertex;
    uint baseInstance;
} DrawElementsIndirectCommand;
```
