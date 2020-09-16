---
title: Draw Commands
date: 2020-09-15 16:44:37
tags: [OpenGL]
---

# Category
- Basic Draw
```c
void glDrawArrays(GLenum mode,
                  GLint first,
                  GLsizei count);
```
<!--more-->
- Indexed Draw
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

- Instanced Draw
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
- Indirect Draw
