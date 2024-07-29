---
title: 龙芯CPU&GPU
date: 2024-07-29 09:09:13
tags: [LS]
categories: hardware
---

# CPU
## [LS3A6000](https://www.loongson.cn/product/show?id=26)

龙芯的芯片产品可以分为信息化领域和工控/嵌入式领域, 3A6000 也有分别应用于这两个领域的配置，7A2000同样有两种。3A6000 是龙芯第四代微架构的首款产品，同样采用龙芯自主指令集架构LoongArch64(首款LoongArch64架构的处理器是[3A5000](https://www.loongson.cn/product/show?id=10))

<!--more-->

# GPU
## [LS7A2000](https://www.loongson.cn/product/show?id=16)

7A2000是第二代龙芯3号系列处理器配置桥片，片内首次集成了自研GPU(应该是 Vivante GC2000), 搭配32位DDR4显存接口，最大支持16GB显存容量。

## 关于驱动

根据龙芯工程师提交的Linux 内核驱动[补丁](https://patchwork.freedesktop.org/series/133512/)，LS7A2000 是一个多功能 PCIe 设备(同时有2D, 3D 功能)，为了避免可能的循环依赖问题，驱动采用了内核的 [component framework](https://www.kernel.org/doc/html/latest/driver-api/component.html)。


# 参考

[1. Unofficial LoongArch FAQ](https://blog.xen0n.name/posts/tinkering/loongarch-faq/)
