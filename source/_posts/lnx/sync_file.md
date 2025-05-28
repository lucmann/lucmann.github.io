---
title: Sync File, the carrier of fences
date: 2025-05-28 14:18:00
tags: [kernel]
categories: linux
---

![everything is file](/images/dot/everything-is-file.svg)

<!--more-->

# Introduction
从 Linux 的 *一切皆文件* 设计角度来看， Sync File 和 DMA-BUF, Sync object 类似，都是为了将内核空间的某个对象暴露给用户空间，并能使这个对象以 file descriptor 的形式在进程之间传递分享。

DMA-BUF 解决的是 Buffer 在设备之间(GPU 与 DISPLAY)，驱动之间(DRM 与 V4L2)，以及进程之间(Compositor 与 Client)的共享问题。Sync object 与 Sync File 相似，都是解决内核同步原语在进程间传递的问题，它们的区别是前者一个对象只封装**一个** fence, 而后者支持与一个 dma-buf 相关的**多个** fences 的集合。


