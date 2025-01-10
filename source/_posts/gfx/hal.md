---
title: Android Graphics Overview
date: 2025-01-01 14:44:56
tags: [Android]
categories: graphics
---


![Android Graphics](/images/hal/android-graphics.png)

<!--more-->

# [Android Stack](https://source.android.com/docs/core/architecture?hl=zh-cn)

![Android Stack](/images/hal/android-stack.svg)

# Android Graphics Stack

```mermaid
block-beta
  columns 3
  block:group1:3
    %% columns auto (default)   
    A["WindowManager"]
    B["SurfaceFlinger"]
    C["..."]
  end
  block:group2:3
    a["Gralloc"]
    b["HWComposer"]
    c["..."]
  end
  block:group3:3
    a1["GPU"]
    b1["DC"]
    c1["..."]
  end
  %% **蓝
  style group1 fill:#55AEF7
  %% **橙
  style group2 fill:#F87529
  %% **绿
  style group3 fill:#25A768
```

# SurfaceFlinger (`/f'lɪngər/`:抛投器)

# [HAL](https://android.googlesource.com/platform/hardware/libhardware/)

## [Gralloc](https://android.googlesource.com/platform/hardware/libhardware/+/refs/heads/main/modules/gralloc/)

## [HWComposer](https://android.googlesource.com/platform/hardware/libhardware/+/refs/heads/main/modules/hwcomposer/)

### [drm-hwcomposer](https://gitlab.freedesktop.org/drm-hwcomposer/drm-hwcomposer)

一个基于 KMS 的 HWComposer 实现。

### [ARM drm-hwcomposer](https://github.com/ARM-software/drm-hwcomposer)

给 Mali DP 量身定制的 drm-hwcomposer.

# Android Sync Framework



# References

- [Android Graphics](https://blog.csdn.net/liuning1985622/article/details/138453346?utm_medium=distribute.pc_relevant.none-task-blog-2~default~baidujs_baidulandingword~default-4-138453346-blog-45080305.235^v43^pc_blog_bottom_relevance_base6&spm=1001.2101.3001.4242.3&utm_relevant_index=6)
- [Android 文档：实现硬件混合渲染器 HAL](https://source.android.com/docs/core/graphics/implement-hwc?hl=zh-cn)
- [Android drm-hwcomposer](https://blog.csdn.net/stray2b/article/details/130291840)
- [Fence](https://blog.csdn.net/MoLiYw/article/details/118829051)
- [Android Sync](https://blog.linuxplumbersconf.org/2014/ocw/system/presentations/2355/original/03%20-%20sync%20&%20dma-fence.pdf)
