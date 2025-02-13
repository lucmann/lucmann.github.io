---
title: Render To Texture
date: 2025-01-11 11:21:27
tags: OpenGL
categories: graphics
---

Render-To-Texture 是一种十分常见和简单的渲染技术，它将纹理对象和 FBO 绑定，把场景渲染到纹理中，以便之后可以反复使用。RTT 被广泛应用在 in-game cameras(virtual camera systems), post-processing 和各种特效中。本文主要比较 RTT 技术在两种不同的渲染架构下的不同和一些思考。

![rtt-on-tbr](/images/rtt/rtt-on-tbr.png)

<!--more-->

# IMR vs TBR

IMR (Immediate Mode Rendering) 和 TBR (Tile-Based Rendering) 是两种不同的渲染架构，前者常见于桌面 GPU (NVIDIA, AMD), 后者常见于移动 GPU (Imagination, Mali)。

## IMR

- 一整块一整块渲染，需要大量访存，所以必须有**比较大的 L1, L2 Cache** 来降低访存延时
- 需要大量带宽，所以**功耗高**
- 渲染管线执行**流畅**， 从顶点到片段**一口气弄完**

```mermaid
block-beta
    columns 1
    block:pipeline
        A["Vertex<br>Processing"] space B["Clip<br>Cull"] space D["Raster"] space E["Early-Z<br>Test"] space F["Texture<br>Fragment"] space G["Alpha<br>Test"] space L["Late-Z<br>Test"] space H["Alpha<br>Blend"]
        A --> B
        B --> D
        D --> E
        E --> F
        F --> G
        G --> L
        L --> H
    end
    space
    block:vram
        VD["Geometry<br>Data"] space:7 TD["Texture<data>Data"] space:3 ZB["Depth<br>Buffer"] space FB["FrameBuffer"]
    end
    VD --> A
    TD --> F
    L --> ZB
    ZB --> L
    H --> FB
    FB --> H
```

## TBR

- 一小块一小块**渲染**, 对于每一小块来说，所需要的访存的带宽就相对较小，可以在片上做一**小块高速缓存**
- 需要的带宽小，所以**功耗低**

```mermaid
block-beta
    columns 1
    block:pipeline
        A["Vertex<br>Processing"] space B["Clip<br>Cull"] space C["Tiling"] space D["Raster"] space E["Early-Z<br>Test"] space F["Texture<br>Fragment"] space G["Late-Z<br>Test"] space H["Alpha<br>Blend"]
        A --> B
        B --> C
        D --> E
        E --> F
        F --> G
        G --> H
    end
    block:onchip
        space:12 ZC["On-Chip<br>Depth Buffer"] space CC["On-Chip<br>Color Buffer"]
    end
    space
    block:vram
        VD["Geometry<br>Data"] space:4 PL["Primitive List<br>Vertex Data"] space:4 TD["Texture<data>Data"] space:3 FB["FrameBuffer"]
    end
    VD --> A
    C --> PL
    PL --> D
    TD --> F
    G --> ZC
    ZC --> G
    H --> CC
    CC --> H
    CC --> FB
    FB --> CC

    style onchip   fill:#00bc00,stroke:#ff0000,stroke-dasharray:5 5
```

由上面的两张图可以看到，在 TBR 架构下，GPU 产生的每个像素并不是直接写入显存的，而是暂存在**片上的 Color Buffer** 中，对于 RTT 来说，如果渲染得到的纹理作为后面绘制命令的源纹理，那么如果没有显式地将还存在于片上的 Color Buffer 刷新入显存中，那么当前的绘制中，纹理采样获取的纹素就是旧数据。

# References

- [Tutorial 14: Render To Texture](https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/)
- [GPU 帧缓冲内存：了解瓦片渲染 TBR(tile-based rendering)](https://zhuanlan.zhihu.com/p/654389634)
- [GPU Framebuffer Memory: Understanding Tiling](https://developer.samsung.com/galaxy-gamedev/resources/articles/gpu-framebuffer.html)
- [Qualcomm Adreno GPU](https://docs.qualcomm.com/bundle/publicresource/topics/80-78185-2/gpu.html)
