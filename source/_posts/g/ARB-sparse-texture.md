---
title: ARB_sparse_texture
date: 2020-07-23 11:33:59
tags: [OpenGL]
categories: graphics
---

# Overview(#158)

复杂的OpenGL应用和对高分辨率的追求使纹理的大小巨增，但是可供一个GPU使用的物理内存是有限的，一旦内存使用殆尽，内存换页就可能发生，性能严重下降，或者更糟，应用直接挂了。但另一方面GPU可访问的地址空间目前已经可以达到GB，甚至TB.

<!--more-->

ARB_sparse_texture允许你使用一个比GPU物理内存大得多的纹理(e.g. 16384x16384 32bpp)，它使用一种虚拟页的方法将大的纹理分成多个页， 这些页有的驻留在物理内存，有的没有，只有当需要的时候才加载它们。比方我们根据相机的视角变化，只加载那些在视野范围内可见的pages. 因此sparse texture也叫做tiled texture或mega texture.

{% asset_img "SparseTextureExample.jpg" "sparse texture" %}

# Restrictions

- 只有Immutable-Format Texture才能作为Sparse Texture
- 不是所有的internalformat都可以作为Sparse Texture, 具体哪些支持参见[ARB_sparse_texture spec](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_sparse_texture.txt)

# How to Use

```c
GLuint page_texture;

glGenTextures(1, &page_texture);
glBindTexture(GL_TEXTURE_2D, page_texture);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SPARSE_ARB, GL_TRUE);
glTexStorage2D(GL_TEXTURE_2D, 15, GL_RGBA32F, 16384, 16384);

// commit a level 0 page.
glTexPageCommitmentARB(GL_TEXTURE_2D, 0, 0, 0, 0, 64, 64, 0, GL_TRUE);
// 4KB increase here.
```

