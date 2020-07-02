---
title: ARB_texture_view
date: 2020-06-09 12:17:07
tags: [OpenGL]
---

# 编号

ARB Extension #124

# 概述

ARB_texture_view的主要目的是通过创建已有纹理对象的视图(view), 来产生新的纹理对象，以达到共享同一纹理存储(data store)的目的。它可以从以下3个方面创建纹理视图：
- texture type 它可以指定一个新的纹理类型(在约束范围内)来创建新的纹理对象
- internal format 它可以定义一个新的纹理格式(在约束范围内)来解释已有的data store
- sub mipmap/array range 它可以从已有纹理的mipmap levels或array slices中截取一个子集作为新的纹理对象

NOTE: 上述最后一种方式只指定新的范围(level/index)，而不进行data store拷贝。



没有增加新的纹理类型，原来的纹理对象被划分为以下几个部分:
- 数据存储(Data Store)
- 状态信息，用来描述Data Store的哪一部分被使用，如何解释所用的数据元素
- 嵌入的采样器对象(Sampler)
- 各种其它纹理参数，像Filter, Mipmap等

# 原则
ARB_texture_view规定的关于创建Texture View的几个原则(限制)主要有以下3个:
- 只能给Immutable纹理创建View,也就是那些由TexStorage创建的纹理
- 原纹理和新纹理的格式要兼容
- 原纹理和新纹理的类型要兼容

## 为什么只给Immutable纹理类型创建Texture View

Texture View本质上并没有上传纹理data store, 而是像指针一样，增加了对已有纹理对象的引用。如果是Mutable纹理，那么它的格式，大小等都是可以修改的，这样就无法保证纹理的consistency和completeness. 而Immutable纹理不会存在这样的问题。