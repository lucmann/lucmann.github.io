---
title: Cache 及一致性
date: 2023-06-27 15:42:45
tags: drm
categories: linux
---

# Cache

音同 cash, 不管是在硬件还是软件都是影响性能的一个重要因素之一。Cache 无论是 CPU cache 还是 GPU cache 一般都分级， L1，L2, 在 Multi-processor CPU/GPU 架构中， L1 一般是分开的，每个 Processor 有一个自己的 L1 Cache, 而 L2 Cache 是全局的，所有 processors 共享的。

<!--more-->

不管 Cache 有多少级，反正是为了降低 processor 访问 memory 的时延的。但是另一方面，当数据缓存到 Cache 中后，就必然引出一致性问题，这里的一致性应该有两个方面

- 各个 processor 看到的 cache 里的内容一致
- cache 与 memory 里的内容一致

注：“内容一致”的另一种说法是“数据是不是最新的”，

