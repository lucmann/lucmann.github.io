---
title: ARB_texture_storage
date: 2020-07-02 15:47:02
tags: [GL]
categories: [/usr]
---

# Overview(#117)

ARB_texture_storage的主要目的是简化纹理一致性(consistency)和完整性(completeness)检查，以减小实现的开销。

<!--more-->

在ARB_texture_storage之前，OpenGL允许给每个Mipmap级别分别指定不同的大小，像素格式(format), 数据类型(type)等等，把纹理一致性检查放在draw time阶段，这给实现增加了开销。

而ARB_texture_storage可以一次性(in a single call)指定纹理的整个结构，把一致性检查和内存申请提前。一旦指定，这个纹理的像素格式和维度都不可再变(Immutable)，这样就简化了纹理完整性检查的实现。

使用ARB_texture_storage, 就不能再用`TexImage*`上传纹理数据了(mutable), 而是用`TexSubImage*`(immutable)。或是用其它方式动态生成纹理数据(immutable), 比如*render-to-texture, mipmap generation, rendering to a sibling EGLImage*.

# Immutable-Format Texture Images

Immutable-Format Texture与ARB_texture_storage有直接的关系。所谓Immutable-Format Texture，就是有一组GL命令，他们可以一次性设定纹理所有level的属性，一旦调用成功，所有level的format和dimensions都不能再改变，除非它是一个代理纹理。然而它的内容和纹理parameters仍然可以修改，这样的纹理就是Immutable-Format Texture.

## A set of commands for immutable-format texture

- glTexStorage1D
- glTexStorage2D
- glTexStorage3D
- glTexStorage2DMultisample
- glTexStorage3DMultisample

