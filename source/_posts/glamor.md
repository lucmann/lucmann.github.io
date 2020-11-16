---
title: glamor
date: 2020-11-10 08:04:15
tags: [X]
---

# Introduction
glamor是一个开源的2D加速驱动，它作为Xorg的一个模块被实现。相比传统的DDX 2D加速驱动，glamor主要有以下2个好处:
- 使用非常通用的EGL/OpenGL API去操作GPU硬件，省去了手动为每个不同的GPU编写2D加速驱动的麻烦
- 有了glamor，不再需要传统的2D DDX驱动

<!--more-->

Xorg的glamor实现依赖3个组件:
- EGL/GLX   负责创建初始化reneder context
- GL/GLES   负责绘制2D图形
- GBM       负责管理Buffer Object（VRAM)

# What role do they play in Xorg drivers?
{% asset_img "glamor_sequence.png" "glamor_sequence" %}

# GBM - Generic Buffer Management
{% asset_img "gbm.png" "gbm internal & backend" %}

