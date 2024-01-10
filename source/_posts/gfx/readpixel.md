---
title: ReadPixels in Mesa
date: 2024-01-09 16:57:44
tags: [OpenGL, mesa]
categories: graphics
---

# glReadPixels

glReadPixels 是将当前绑定的 FBO 里的内容从显存(renderbuffer, 通常是 read renderbuffer) 读到 `*data` 指向的内存。

```
void glReadPixels(GLint x, GLint y,
                  GLsizei width, GLsizei height,
                  GLenum format, GLenum type, void *data);
```

<!--more-->

在这次从显存到主存的数据传输中可能源(renderbuffer) 数据格式有可能与请求的(glReadPixels参数 format, type 指定的)格式不相同，这时就需要数据被放入 `*data` 前做一次转换。这就产生了基于 Blit 的 glReadPixels 的实现。 还有与之对应的所谓 slow path (其实就是让 CPU 做这次格式转换)

## 为什么 slow path 要比 blit-based 慢呢？

主要是 CPU 做这次格式转换慢，无论是CPU做格式转换还是基于 blit 的 GPU 做这次格式转换，在转换完成后都要将转换为要求格式的数据 memcpy 到 `*data`.


