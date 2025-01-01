---
title: Android HAL
date: 2025-01-01 14:44:56
tags: [Android]
categories: graphics
---

# [Android Stack](https://source.android.com/docs/core/architecture?hl=zh-cn)

![Android Stack](/images/hal/android-stack.svg)

<!--more-->

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
    a["EGL/GLES"]
    b["HWComposer"]
    c["..."]
  end
  block:group3:3
    a1["GPU"]
    b1["DC"]
    c1["..."]
  end
  %% 海沫蓝
  style group1 fill:#19C5AE
  %% **橙
  style group2 fill:#F87529
  %% **绿
  style group3 fill:#25A768
```

# References
- [Andorid 文档：实现硬件混合渲染器 HAL](https://source.android.com/docs/core/graphics/implement-hwc?hl=zh-cn)
