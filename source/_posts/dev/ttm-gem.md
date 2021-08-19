---
title: TTM vs GEM
date: 2021-08-19 15:42:45
tags: [mm, vram]
---

# TTM和GEM

Translation Table Maps(TTM)和Graphics Execution Manager(GEM)是Linux内核DRM子系统的两个内存管理器，前者先于后者被实现，也可以说GEM是为了解决TTM存在的一些问题才被设计出来的。下面的表格对它们两个的特点做了简单比较。

|                   | TTM             | GEM              |
|:------------------|:---------------:|:----------------:|
| 复杂度            | 高              | 低               |
| 支持的内存模型    | UMA 和 独立显存 | 只支持UMA        |
| 接口              | 统一/难用       | 简单/易用        |

- UMA Unified Memory Architecture设备，例如SoC
- NUMA Non Unified Memory Architecture设备, 例如独立显卡
