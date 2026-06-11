---
title: 龙芯 CPU & GPU
date: 2024-07-29 09:09:13
tags: [Hardware]
categories: hardware
---

# CPU
## [LS3A6000](https://www.loongson.cn/product/show?id=26)

龙芯的芯片产品可以分为信息化领域和工控/嵌入式领域, 3A6000 也有分别应用于这两个领域的配置，7A2000同样有两种。3A6000 是龙芯第四代微架构的首款产品，同样采用龙芯自主指令集架构LoongArch64(首款LoongArch64架构的处理器是[3A5000](https://www.loongson.cn/product/show?id=10))

<!--more-->

# GPU

龙芯的 GPU 目前基本存在于与龙芯 CPU 配套使用的桥片中，使用的是 Vivante GC 系列的低端 GPU IP.

NOTE: Vivante Corporation 于 2015 年被芯原收购后，后来芯原推出了最新的 [Arcturus GC8000 系列](https://www.verisilicon.com/en/IPPortfolio/VivanteGPUIP)，但开源驱动 etnaviv 并不支持该最新系列 GPU.

## [LS7A1000](https://www.loongson.cn/product/show?id=14)

龙芯7A1000是面向服务器及个人计算机领域的龙芯3号系列处理器配套桥片。它里面应该集成了 Vivante GC1000 V5037 IP

> There is a Vivante GC1000 V5037 in LS2K1000 and LS7A1000,
> the gpu is a PCI device and it have 2D and 3D in the same core.
> Therefore, this patch try to provide PCI device driver wrapper
> for it by mimic the platform counterpart.
> 
> LS7A1000 is a bridge chip, this bridge chip typically use
> with LS3A4000 (4 core 1.8gHz, Mips64r5) and LS3A5000 (4 core
> loongarch 2.5Ghz). While LS2K1000 is a double core 1.0Ghz
> Mips64r2 SoC.
> 
> loongson CPU's cache coherency is maintained by the hardware.
> 
> Both LS7A1000 and LS2K1000 have a display controller integrated,
> named lsdc. The drm driver of this display controller is not
> upstream yet, but we have a demo version and it works.
> By using KMS-RO framework, lsdc and gc1000 made a compatible pair.

## [LS7A2000](https://www.loongson.cn/product/show?id=16)

7A2000是第二代龙芯3号系列处理器配置桥片，片内首次集成了自研GPU(应该是 Vivante GC2000), 搭配32位DDR4显存接口，最大支持16GB显存容量。

## 关于驱动

根据龙芯工程师提交的Linux 内核驱动[补丁](https://patchwork.freedesktop.org/series/133512/)，LS7A2000 是一个多功能 PCIe 设备(同时有2D, 3D 功能)，为了避免可能的循环依赖问题，驱动采用了内核的 [component framework](https://www.kernel.org/doc/html/latest/driver-api/component.html)。


# 参考

- [Unofficial LoongArch FAQ](https://blog.xen0n.name/posts/tinkering/loongarch-faq/)
- [Vivante GPUs Comparison](https://dev.visucore.com/etna_viv/gpus_comparison.html)
