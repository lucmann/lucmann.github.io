---
title: dri3_alloc_render_buffer
date: 2024-07-30 21:21:27
tags: DRI
categories: graphics
---

mesa/loader 下的 [`dri3_alloc_render_buffer()`](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/loader/loader_dri3_helper.c#L1435), 这个函数不长，但却涉及到了DRI框架的许多重要概念，buffer共享(DMABUF), GPU offload (PRIME), 送显， modifiers等等，所以非常值得深入分析。

<!--more-->

```src/loader/loader_dri3_helper.c
static struct loader_dri3_buffer *
dri3_alloc_render_buffer(struct loader_dri3_drawable *draw,
                         unsigned int fourcc,
                         int width, int height, int depth);
```
