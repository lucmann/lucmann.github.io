---
title: AMDGPU 驱动(KMD + UMD)中的一些缩写词解释
date: 2023-08-19 18:30:15
tags: drm
categories: linux
---

![AMDGPU Microarchitecture Roadmap](amd-arch.jpg)

|  缩写      | 全称                       | 解释                                                                               |
|:-----------|:---------------------------|:-----------------------------------------------------------------------------------|
| IB         | Indirect Buffer            | AMD GPU 提供 Indirect Buffer 的方式由 CPU 向 GPU 传递渲染命令                      |
| CDNA       | Compute DNA                | AMD 第5代计算核心微架构名称                                                        |
| DCN        | Display Core Next          | AMD 第4代显存芯片的微架构名称                                                      |
| GCN        | Graphics Core Next         | AMD 第4代 GPU 微架构名称                                                           |
| KFD        | Kernel Fusion Driver       | AMD APU 芯片的内核驱动， 主要是驱动 HSA 芯片                                       |
| KGD        | Kernel Graphics Driver     | AMD GPU 芯片的内核驱动， 主要是驱动独立显卡和 OEM 上的 GPU 芯片                    |
| RDNA       | Radeon DNA                 | AMD 第5代 GPU 微架构名称， 也有说[RaDeon New Architecture](https://www.reddit.com/r/Amd/comments/i589qr/what_does_rdna_stand_for/)                                                                                     |
| UVD        | Unified Video Decoder      | AMD 开发的硬件视频解码引擎， 主要是完成硬件级别上加速视频解码任务                  |
| VCE        | Video Compression Engine   | AMD 开发的硬件视频编码引擎， 主是提供视频压缩和编码功能                            |
| VCN        | Video Codec Next           | AMD 第4代 VPU 微架构名称                                                           |
