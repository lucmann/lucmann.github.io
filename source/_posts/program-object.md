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

# Program Object
创建和使用**Program Object**的API:
- `GLuint glCreateProgram();`
- `void glAttachShader(GLuint program, GLuint shader);`

{% asset_img "program-object.png" %}

# Program Pipeline Object

