---
title: OpenGL Program Object
date: 2021-04-06 14:24:55
tags: [shader]
categories: media
---

# Shader Object
创建和使用**Shader Object**的API:
- `GLuint glCreateShader(GLenum shaderType);`
- `void glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);`
- `void glCompileShader(GLunit shader);`

# Shader Stage
OpenGL里没有专门的Shader Stage Object去封装Stage相关的状态。但是事实上，一个或多个特定类型的Shader Object(s)组成一个特定的Shader Stage, 多个**Shader Stages**链接成一个Program Object. 只不过大多情况下，一个Shader Stage只来自于一个Shader Object，但是多个同一类型的Shader Objects组成一个Program Object的一个Shader Stage是完全符合OpenGL规范的。

{% asset_img "shader-stage.png" %}

# Program Object
创建和使用**Program Object**的API:
- `GLuint glCreateProgram();`
- `void glAttachShader(GLuint program, GLuint shader);`

{% asset_img "program-object.png" %}

# Program Pipeline Object

