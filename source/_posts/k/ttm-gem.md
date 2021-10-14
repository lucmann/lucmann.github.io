---
title: TTM vs GEM
date: 2021-08-19 15:42:45
tags: [mm, vram]
categories: kernel
---

# TTM和GEM

Translation Table Maps(TTM)和Graphics Execution Manager(GEM)是Linux内核DRM子系统的两个内存管理器，前者先于后者被实现，也可以说GEM是为了解决TTM存在的一些问题才被设计出来的。下面的表格对它们两个的特点做了简单比较。

<!--more-->

|                   | TTM             | GEM              |
|:------------------|:---------------:|:----------------:|
| 复杂度            | 高              | 低               |
| 支持的内存模型    | UMA 和 独立显存 | 只支持UMA        |
| 接口              | 统一/难用       | 简单/易用        |

- UMA Unified/Uniform Memory Architecture设备，例如集成显卡
- NUMA Non-Unified/Uniform Memory Architecture设备, 例如独立显卡

## GEM

GEM一开始是Intel的工程师给i915驱动开发的显存管理器，i915支持的Intel的GMA 9xx系列都是UMA内存模型的集成显卡，也就是GPU和CPU共享物理内存，没有独立显存

## TTM

TTM的设计相较于GEM更复杂，它可以管理最多8种内存

```
#define TTM_NUM_MEM_TYPES 8
```

内核中预先定义了3种， 剩下的由驱动自己定义

```
#define TTM_PL_SYSTEM           0
#define TTM_PL_TT               1
#define TTM_PL_VRAM             2
#define TTM_PL_PRIV             3
```

例如, amdgpu驱动又定义了以下Placement

```
#define AMDGPU_PL_GDS		(TTM_PL_PRIV + 0)
#define AMDGPU_PL_GWS		(TTM_PL_PRIV + 1)
#define AMDGPU_PL_OA		(TTM_PL_PRIV + 2)
```

TTM中主要的数据结构有

- `ttm_bo_device`
- `ttm_bo_driver`
- `ttm_resource_manager`
- `ttm_resource_manager_func`

主要的API有

- `ttm_bo_device_init`

    ```
    int ttm_bo_device_init(struct ttm_bo_device *bdev,
                   struct ttm_bo_driver *driver,
                   struct device *dev,
                   struct address_space *mapping,
                   struct drm_vma_offset_manager *vma_manager,
                   bool use_dma_alloc, bool use_dma32)
    ```


# 参考
[freelancer-leon's notes](https://github.com/freelancer-leon/notes/blob/master/kernel/graphic/Linux-Graphic.md)
[GEM wikipedia](https://en.wikipedia.org/wiki/Direct_Rendering_Manager#Graphics_Execution_Manager)
