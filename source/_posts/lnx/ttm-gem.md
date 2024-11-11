---
title: mlock vs mpin
date: 2021-08-19 15:42:45
tags: [DRM]
categories: linux
---

# 背景介绍

之前一直有一个疑问：[为什么在 drm_gpu_sheduler 的 run_job 路径里带 GFP_KERNEL 标志的内存申请可以造成死锁?](https://www.kernel.org/doc/html/latest/driver-api/dma-buf.html#dma-fence-cross-driver-contract), 直到了解了 **Locking pages** 和 **Pinning pages** 的概念后，好像是明白了死锁的过程。

<!--more-->

Linux 是基于页 (page) 来管理内存的，物理内存页和虚拟内存页一般有相同的大小，比较常见的是 4KB 页大小。虚拟内存和分页的一个主要好处就是让进程看到的内存远远大于物理内存，而且进程之间的内存地址空间是隔离的，但带来的一个开销就是物理内存里的页要与硬盘里的页时不时地进行交换，这必然会影响性能。对于那些对实时性要求比较高的应用可能就是问题，所以 Linux 提供了 **Locking Pages** 的概念，这些被**锁定**的页是不会从物理内存里换出的，也就没有了 Swapout 的 I/O 开销。这是通过 `mlock()` 这个系统调用实现的，但 `mlock()` 只是让页"常驻"上物理内存，但并没有规定不能被移动 (Moveable), 一旦页被移动，就会触发 soft page fault, 这种 page fault 虽然不用进行"昂贵"的 I/O 就能处理，但是毕竟也是有开销，而且还有[其它各种问题](https://lwn.net/Articles/600502/), **Pinning Pages** 就是将页固定死在内存中 (Fix memory in place), 既不能被换出，也不能被移动。

这个 Pinning Pages 在 DMA-BUF 中用得比较多。

![](/images/mpin/linux-mem-page.png)

# 参考
[Locking and pinning](https://lwn.net/Articles/600502/)
[pin_user_pages()](https://docs.kernel.org/core-api/pin_user_pages.html)
[GFP flags and reclaim behavior](https://www.kernel.org/doc/html/next/core-api/memory-allocation.html#gfp-flags-and-reclaim-behavior)
[freelancer-leon's notes](https://github.com/freelancer-leon/notes/blob/master/kernel/graphic/Linux-Graphic.md)
[GEM wikipedia](https://en.wikipedia.org/wiki/Direct_Rendering_Manager#Graphics_Execution_Manager)
