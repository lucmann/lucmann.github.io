---
title: Sync File, the carrier of fences
date: 2025-05-28 14:18:00
tags: [kernel]
categories: linux
---

![everything is file](/images/dot/everything-is-file.svg)

<!--more-->

# Introduction
从 Linux 的 *一切皆文件* 设计角度来看， Sync file 和 DMA-BUF, Sync object 类似，都是为了将内核空间的某个对象暴露给用户空间，并能使这个对象以 file descriptor 的形式在进程之间传递分享。

DMA-BUF 解决的是 Buffer 在设备之间(GPU 与 DISPLAY)，驱动之间(DRM 与 V4L2)，以及进程之间(Compositor 与 Client)的共享问题。Sync object 与 Sync file 相似，都是解决内核同步原语在进程间传递的问题，它们的区别是前者一个对象只封装**一个** fence, 而后者支持与一个 dma-buf 相关的**多个** fences 的集合。

Sync file 起初是从 Android kernel 引入的， 当 Dave Airlie 和 Faith Ekstrand 分别在 2017 年 和 2022 年提交了 [drm/syncobj: add sync_file interaction. (v1.2)](https://lists.freedesktop.org/archives/dri-devel/2017-June/143204.html) 和 [https://patchwork.freedesktop.org/series/104898/](https://patchwork.freedesktop.org/series/104898/) 两个补丁集后，Sync file 便分别与 DRM Syncobj 和 DMA-BUF 产生了关联，而后者的合入标志着Linux 图形世界已经在 implicit sync 和 explicit sync 之间建起了一座桥梁。

# Background

## Implicit sync vs Explicit sync

从宏观层面讲，Implicit sync 指同步由 UMD 和 KMD 完全处理，3D 应用程序不参与(3D应用程序包括 compositor)， 相反，Explicit sync 指什么时候等待，等待谁，都交给应用程序自己决定和处理。Android graphics 从一开始就采用 Explicit sync 机制，而 Linux desktop 在 Vulkan 之前基本上都是采用 Implicit sync, 因为 Vulkan API 被设计成支持 Explicit sync, 因为在整个图形栈里，从渲染到合成到显示，任何一个环节只要不支持 Explicit sync，整个路径就只能走 implicit sync, 这就导致 Vulkan 应用与其它采用 Implict sync 的应用之间，尤其是与 X Server 之间的同步变成一个问题。

# References

- [Explicit sync](https://zamundaaa.github.io/wayland/2024/04/05/explicit-sync.html)
- [Bridging the synchronization gap on Linux](https://www.collabora.com/news-and-blog/blog/2022/06/09/bridging-the-synchronization-gap-on-linux/)


