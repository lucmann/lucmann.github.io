---
title: ARB_texture_storage
date: 2020-07-02 15:47:02
tags: [OpenGL]
---

# 编号

ARB Extension #117

# 概述

ARB_texture_storage的主要目的是简化纹理一致性(consistency)和完整性(completeness)检查，以减小实现的开销。

在ARB_texture_storage之前，OpenGL允许给每个Mipmap级别分别指定不同的大小，像素格式(format), 数据类型(type)等等，把纹理一致性检查放在draw time阶段，这给实现增加了开销。

而ARB_texture_storage可以一次性(in a single call)指定纹理的整个结构，把一致性检查和内存申请提前。一旦指定，这个纹理的像素格式和维度都不可再变(Immutable)，这样就简化了纹理完整性检查的实现。

使用ARB_texture_storage, 就不能再用`TexImage*`上传纹理数据了(mutable), 而是用`TexSubImage*`(immutable)。或是用其它方式动态生成纹理数据(immutable), 比如*render-to-texture, mipmap generation, rendering to a sibling EGLImage*.