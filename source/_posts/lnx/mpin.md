---
title: mlock vs mpin
date: 2021-08-19 15:42:45
tags: [DRM]
categories: linux
---

# 背景介绍

之前一直有一个疑问：[为什么在 drm_gpu_sheduler 的 run_job 路径里带 GFP_KERNEL 标志的内存申请可以造成死锁?](https://www.kernel.org/doc/html/latest/driver-api/dma-buf.html#dma-fence-cross-driver-contract), 直到了解了 **Locking pages** 和 **Pinning pages** 的概念后，好像是明白了死锁的过程。

<!--more-->

Linux 是基于页 (page) 来管理内存的，物理内存页和虚拟内存页一般有相同的大小，比较常见的是 4KB 页大小。



# 参考
[Locking and pinning](https://lwn.net/Articles/600502/)
[GFP flags and reclaim behavior](https://www.kernel.org/doc/html/next/core-api/memory-allocation.html#gfp-flags-and-reclaim-behavior)
[freelancer-leon's notes](https://github.com/freelancer-leon/notes/blob/master/kernel/graphic/Linux-Graphic.md)
[GEM wikipedia](https://en.wikipedia.org/wiki/Direct_Rendering_Manager#Graphics_Execution_Manager)
