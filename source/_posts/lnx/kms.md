---
title: Atomic Mode Setting 设计概要 第一部分 （译）
date: 2021-09-15 18:30:15
tags: [kms]
categories: linux
---

本文翻译 Daniel Vetter 于 2015 年 8 月 5 日发表在 LWN.net 的文章 "Atomic mode setting design overview, part 1". 文章主要阐述了为什么要引入一个全新的 Kernel Display Driver 接口以及新老实现的一些细节。

<!--more-->

过去几年[1], 两大趋势产生了对一个全新的 Kernel Display Driver 接口的需要。一方面，人们不再欣赏部分重绘和窗口撕裂当他们的图形界面有任何变化的时候。像 Wayland 这样的新的合成器(compositor) 宣称 "every frame is perfect". 另一方面，出现了很多电池供电的手机和平板, 这些设备虽然有复杂绚丽的图形界面，但也有严重的功耗限制。这导致了一系列特殊用途的显示硬件的爆发，以帮助相对耗电量较高但更通用的GPU在合成屏幕内容时发挥作用。将这些趋势结合起来，需要以一种全或无(all-or-nothing)的原子方式更新大量显示硬件状态，以确保每一帧都是完美的，并尽可能地使用功耗优化的硬件。

经过几年的开发后，DRM 子系统的 atomic display 驱动 ioctl() 终于在黄金时间(prime time)准备好了。 


[阅读原文](https://lwn.net/Articles/653071/)
