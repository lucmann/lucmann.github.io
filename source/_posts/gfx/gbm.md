---
title: Generic Buffer Manager 
date: 2025-03-07 14:44:56
tags: [Mesa]
categories: graphics
---

![gbm dri backend implemented in Mesa](/images/gbm/gbm.png)

<!--more-->

GBM 是 Linux 图形栈中的重要组件，它定义了一组分配 GPU 缓冲区 (Buffer Object) 的[接口](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/gbm/main/gbm.h)。Mesa 中提供一个基于 DRI 的后端[实现](https://gitlab.freedesktop.org/mesa/mesa/-/tree/main/src/gbm/backends/dri)

# [Gralloc](https://android.googlesource.com/platform/hardware/libhardware/)

# [minigbm](https://chromium.googlesource.com/chromiumos/platform/minigbm/)
