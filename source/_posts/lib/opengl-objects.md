---
title: OpenGL Objects
date: 2021-05-07 20:48:24
tags: [GL]
categories: lib
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

