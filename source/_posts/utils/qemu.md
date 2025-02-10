---
title: QEMU
date: 2025-02-10 09:20:28+08:00
tags: [virtualization]
categories: utilities
---

# QEMU

| 特性                  | QEMU                                      | VirtualBox                      |
|:---------------------|:------------------------------------------|:-------------------------------|
| 开发者                | 开源（QEMU 项目, 最初由 Fabrice Bellard 开发）| Oracle（原 Sun Microsystems）  |
| 开源/闭源             | 完全开源（GPLv2）                           | 部分开源（扩展包闭源）     |
| 支持架构              | x86, ARM, RISC-V, MIPS, LoongArch, SPARC等| 仅 x86_64                  |
| 是否支持纯软件模拟      | 支持（TCG 模拟,也支持 KVM 加速）             | 不支持（必须 Intel VT-x/AMD-V）  |
| 适用场景              | 嵌入式开发, 跨架构仿真                       | 桌面用户运行 Win/Linux    |

<!--more-->
