---
title: Display Pipeline in Linux
date: 2023-09-12 08:30:15
tags: [DRM/KMS]
categories: linux
---

在 Linux GPU 软件栈这块，大体包含 3 个 Pipelines

- Graphics Pipeline
- Display Pipeline
- Compute Pipeline

<!--more-->

Graphics Pipeline 和 Compute Pipeline 基本上都是围绕 GPU 这个 IP 进行的，Vulkan API 的设计也可以看出这一点。 而 Display Pipeline （这里指显示）主要围绕 DC (Display Controller) 这个 IP 进行。

本文所涉及的知识本身并没有什么新的东西，但对我来说是新的，所以这篇文章也是作为一个学习笔记来整理和记录自己对 Display Pipeline 的理解。




# 参考

1. [From pre-history to beyond the global thermonuclear war](https://ppaalanen.blogspot.com/2014/06/from-pre-history-to-beyond-global.html)
2. [RFCv2 00/10 Universal plane support](https://lists.freedesktop.org/archives/dri-devel/2014-March/055222.html)
3. [Multiplane Overlay (MPO)](https://www.kernel.org/doc/html/latest/gpu/amdgpu/display/mpo-overview.html?highlight=mpo)
