---
title: OpenGL Objects
date: 2021-05-07 20:48:24
tags: [OpenGL]
categories: graphics
---

# Objects

<!--more-->

| Object             | Shareability | Containability              |
|:-------------------|:-------------|:----------------------------|
| Buffer             | shared       | No                          |
| Shader             | shared       | No                          |
| Program            | shared       | No                          |
| Program Pipeline   | non-shared   | Program Objects             |
| Texture            | shared       | No                          |
| Sampler            | shared       | No                          |
| Renderbuffer       | shared       | No                          |
| Framebuffer        | non-shared   | Renderbuffer and/or Texture |
| Vertex Array       | non-shared   | Buffer Objects              |
| Transform Feedback | non-shared   | Buffer Objects              |
| Query              | non-shared   | No                          |
| Sync               | shared       | No                          |

NOTE:
- 对于一类Objects是否可以共享是指是否可以在多个**OpenGL contexts**之间共享

# Buffer Object

## Indexed Buffer Target

| Target                       | Limit                                 | Version |
|:-----------------------------|:--------------------------------------|---------|
| GL_ATOMIC_COUNTER_BUFFER     | GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS | 4.2     |
| GL_TRANSFORM_FEEDBACK_BUFFER | GL_MAX_TRANSFORM_FEEDBACK_BUFFERS     |         |
| GL_UNIFORM_BUFFER            | GL_MAX_UNIFORM_BUFFER_BINDINGS        |         |
| GL_SHADER_STORAGE_BUFFER     | GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS | 4.3     |
