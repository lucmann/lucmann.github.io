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


```mermaid
graph TD;
	-->S1:glCreateShader;
	-->S2:glCreateShader;
	-->S3:glCreateShader;
	-->S4:glCreateShader;
	-->S5:glCreateShader;
	-->S6:glCreateShader;
```
