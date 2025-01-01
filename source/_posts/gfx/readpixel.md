---
title: ReadPixels in Mesa
date: 2024-01-09 16:57:44
tags: [OpenGL, Mesa]
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

在这次从显存到主存的数据传输中可能源(renderbuffer) 数据格式有可能与请求的(glReadPixels参数 format, type 指定的)格式不相同，需要数据在被回读到 `*data` 前做一次格式或字节序转换。为了能够加速这个转换过程，就产生了基于 Blit 的 glReadPixels 的实现。 还有与之对应的所谓 slow path (其实就是让 CPU 做这次格式转换)

# Why prefer to blit-based ReadPixels？

原因有两方面：

- 格式转换 (required than preferred)
- memcpy 的效率 (row by row or one-shot)

```
if (tex_xfer->stride == bytesPerRow && destStride == bytesPerRow) {
   memcpy(dest, map, bytesPerRow * height);
} else {
   GLuint row;

   for (row = 0; row < (unsigned) height; row++) {
      memcpy(dest, map, bytesPerRow);
      map += tex_xfer->stride;
      dest += destStride;
   }
}
```

第 2 个问题需要理解一张图片或一块像素它在内存中的存储方式

![The usual layout of pixels of an image in memory](/images/readpixel/image-in-memory.png)
![How to compute the address of a pixel](/images/readpixel/image-in-address.png)
