---
title: AMDGPU 驱动(KMD + UMD)中的一些缩写词解释
date: 2023-08-19 18:30:15
tags: [DRM/KMS]
categories: linux
---

![AMDGPU Microarchitecture Roadmap](/images/amdgpu-glossary/amd-arch.jpg)

<!--more-->

# Glossary

## Hardware

|  缩写      | 全称                       | 解释                                             |
|:-----------|:---------------------------|:-----------------------------------------------|
| CDNA       | Compute DNA                | AMD 第5代计算核心微架构 |
| DCN        | Display Controller Next    | AMD 第4代显示芯片的微架构 |
| GCN        | Graphics Core Next         | AMD 第4代 GPU 微架构 |
| GDS        | Global Data Storage        | 片上全局数据存储，shader threads 之间共享 |
| GDS OA     | GDS Ordered Append         | 专门被 graphics/compute engines 用来增量存储数据 |
| GMC        | Graphics Memory Controller | 控制 GPU 芯片上各种 IP 访问的硬件模块 | 
| GWS        | Global Wave Sync           | 同步所有 waves 的硬件模块 (HW Scheduler?) |
| RDNA       | Radeon DNA                 | AMD 第5代 GPU 微架构， 也有说[RaDeon New Architecture](https://www.reddit.com/r/Amd/comments/i589qr/what_does_rdna_stand_for/) |
| UVD        | Unified Video Decoder      | AMD 开发的硬件视频解码引擎， 主要是完成硬件级别上加速视频解码任务 |
| VCE        | Video Compression Engine   | AMD 开发的硬件视频编码引擎， 主是提供视频压缩和编码功能 |
| VCN        | Video Codec Next           | AMD 第4代 VPU 微架构 |
| XDNA       |                            | AMD NPU 微架构, 专为 PC 中的生成式 AI 打造 |

## Software

|  缩写      | 全称                       | 解释                                                        |
|:-----------|:---------------------------|:----------------------------------------------------------|
| IB         | Indirect Buffer            | AMD GPU 提供 Indirect Buffer 的方式由 CPU 向 GPU 传递渲染命令 |
| KFD        | Kernel Fusion Driver       | AMD APU 芯片的内核驱动， 主要是驱动 HSA 芯片 |
| KGD        | Kernel Graphics Driver     | AMD GPU 芯片的内核驱动， 主要是驱动独立显卡和 OEM 上的 GPU 芯片 |
| RAS        | Reliability, Availability, Serviceability | AMDGPU 驱动的一个功能特性，帮助错误检测上报，错误处理和调试 |

# References
- [Kernel Doc](https://docs.kernel.org/gpu/amdgpu/driver-core.html)
