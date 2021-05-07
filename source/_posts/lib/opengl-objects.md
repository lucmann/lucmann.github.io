---
title: OpenGL Objects
date: 2021-05-07 20:48:24
tags: [GL]
categories: lib
---

# Objects
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
