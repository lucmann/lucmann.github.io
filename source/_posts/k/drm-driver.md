---
title: 了解 Linux DRM 子系统
date: 2021-04-12 17:07:27
tags: drm
categories: kernel
---

# DRM driver features

## 新DRM驱动的特性

- DRIVER_GEM

使用**GEM**内存管理器，所有新的驱动都应该支持。

- DRIVER_MODESET

支持**Kernel Modesetting Interfaces**

- DRIVER_RENDER

支持专门的渲染节点， 即**/dev/dri/renderD**

- DRIVER_ATOMIC

支持所有的**Atomic Modesetting**用户空间API

- DRIVER_SYNCOBJ

支持**drm_syncobj**, 用于渲染命令提交的显式同步。

- DRIVER_SYNCOBJ_TIMELINE

支持**drm_syncobj**的**timeline**特性

## 旧DRM驱动的特性

- DRIVER_USE_AGP
- DRIVER_LEGACY
- DRIVER_PCI_DMA
- DRIVER_SG
- DRIVER_HAVE_DMA
- DRIVER_HAVE_IRQ
- DRIVER_KMS_LEGACY_CONTEXT

# 内核 DRM 子系统是如何维护的

内核 DRM 子系统的代码仓库托管在cgit上，[Upstream DRM Subsystem Repository](https://cgit.freedesktop.org/drm/drm), 这个仓库最主要的两个分支

- drm-next

    这个分支的补丁包括 DRM 核心和所有 GPU 驱动的新特性

- drm-fixes

# 参考

[[0] DRM Maintainer Tools](https://drm.pages.freedesktop.org/maintainer-tools/repositories.html)
