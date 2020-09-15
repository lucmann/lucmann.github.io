---
title: Draw Commands
date: 2020-09-15 16:44:37
tags: [OpenGL]
---

# Category
- Basic Draw
- Indexed Draw
- Instanced Draw
```c
void glDrawArraysInstancedBaseInstance(GLenum mode,
                                       GLint first,
                                       GLsizei count,
                                       GLsizei instancecount,
                                       GLuint baseinstance);

void glDrawElementsInstancedBaseInstance(GLenum mode,
                                         GLsizei count,
                                         GLenum type,
                                         const void* indices,
                                         GLsizei instancecount,
                                         GLuint baseinstance);
```
- Indirect Draw
