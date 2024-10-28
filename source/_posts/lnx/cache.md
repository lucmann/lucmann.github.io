---
title: Cache 一致性
date: 2023-06-27 15:42:45
tags: [perf]
categories: linux
---

# Cache

音同 cash, 不管是在硬件还是软件都是影响性能的一个重要因素之一。Cache 无论是 CPU cache 还是 GPU cache 一般都分级， L1，L2, 在 Multi-processor CPU/GPU 架构中， L1 一般是分开的，每个 Processor 有一个自己的 L1 Cache, 而 L2 Cache 是全局的，所有 processors 共享的。

<!--more-->

不管 Cache 有多少级，反正是为了降低 processor 访问 memory 的时延的。但是另一方面，当数据缓存到 Cache 中后，就必然引出一致性问题，这里的一致性应该有两个方面

- 各个 processor 看到的 cache 里的内容一致
- cache 与 memory 里的内容一致

注：“内容一致”的另一种说法是“数据是不是最新的”，

# WC vs GRE

Write-Combining 是 X86-64 平台上一种特殊的内存类型 (Memory Type), 它被广泛应用在 X86-64 平台的 I/O 和其它各种外设的交互中。它的主要含义是**将多个 stores (写内存) 收集到 burst transactions中**，所以它主要是为了优化 CPU 访存(写)的效率。

同样的想法和技术在不同的平台下，只是叫法不同。Arm 平台自然也有类似的优化技术，即 GRE (Gathering, Reordering, Early write acknowledgement), Arm 架构定义了两种**不使用 Cache (Non-Cacheable**) 的内存类型

- Normal Non-Cacheable (Normal NC)
- Device memory with GRE attributes (Device-GRE)

这两种内存类型的共同点是：

- 不使用 Cache
- 支持 GRE (相当于支持 Write-Combining)

下表将 X86-64, Arm Normal NC, Arm Device-GRE 做了一个对比

|                 | X86-64           | Arm Normal NC    | Arm Device-GRE     |
|:----------------|:-----------------|:-----------------|:-------------------|
|Relaxed Order    | Yes              | Yes              | Yes                |
|Gathering        | Yes              | Yes              | Yes                |
|Read Speculation | Yes              | Yes              | No                 |
|Unaligned Access | Yes              | Yes              | No                 |
|Gathering Size   | 64 Bytes         | micro-arch       | micro-arch         |

注：micro-arch 的意思是具体大小取决于具体微架构设计

从上表可以看出，Arm 的 Normal NC 等同于 X86-64 的 WC, 而 Arm 上的两种非 cache 的内存的主要区别在读预测和非对齐访问，实际上，个人理解 Arm Device-GRE 之所以会存在只是因为"读预测" 和 "非对齐访问" 这两种特性在 Arm 架构下本来就**不常用**, 单就优化 CPU 写内存来说，它俩并不是什么影响因素。
