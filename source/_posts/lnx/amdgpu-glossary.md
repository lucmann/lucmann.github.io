---
title: AMDGPU 驱动(KMD + UMD)中的一些缩写词解释
date: 2023-08-19 18:30:15
tags: [DRM/KMS]
categories: linux
---

![AMDGPU Microarchitecture Roadmap](/images/amdgpu-glossary/amd-arch.jpg)

<!--more-->

# Glossary

## [GPU Hardware](https://docs.kernel.org/gpu/amdgpu/driver-core.html#gpu-hardware-structure)

这里的 hardware block, IP (Intellectual Property) block, controller, processor, engine 基本上都是指一个具有相对独立功能的**硬件处理单元**。

|Abbr.| Stands for                 | Description                                    |
|:----|:---------------------------|:-----------------------------------------------|
| GMC | Graphics Memory Controller | 管理 GPU 上不同 IP 如何获取内存 (VRAM) 的控制器，它也提供每进程 GPU 虚拟地址空间的支持(听起来有点像 MMU) |
| IH  | Interrupt Handler          | GPU 上的中断控制器 |
| PSP | Platform Security Processor| 处理 SoC 上的安全策略，执行 trusted app, 为其它 IP 验证和加载固件程序 |
| SMU | System Management Unit     | SoC 的电源管理微控制器，驱动通过这个模块来控制芯片的时钟，电压域，电源轨等 |
| DCN | Display Controller Next    | 显示控制器 |
| SDMA| System DMA                 | 多功能 DMA engine, KMD 利用它完成分页，GPU 页表更新，而且它通过 UMD 暴露给用户态使用 |
| GC  | Graphics and Compute       | GFX/Compute engine, 这是 GPU 上最大的 IP, 它包含 3D pipeline 和 shader cores |
| VCN | Video Codec Next           | Multi-media engine，它处理视频和图像的编解码，通过 UMD 暴露给用户态使用 |
| CP  | Command Processor          | 包含 GFX/Compute engine 的前端，一批微控制器，包括 PFP,ME,CE,MEC，它们上面运行固件代码，为驱动提供与 GFX/Compute engine 进行交互的接口 |
| CE  | Constant Engine            | GFX CP 里的一个小处理器，主要用来更新 buffer descriptor 以便异步地将 PFP/ME 使用的 buffer 加载进 cache |
| PFP | Pre-Fetch Parser           | GFX CP 里的一个小处理器，需要给它加载 µCode 去执行, 从名字能看出来它是预取 packets 的 |
| ME  | MicroEngine                | GFX CP 里的一个小处理器，它和 PFP 组成一个 Drawing Engine，可以和 CE 异步执行 |
| MEC | MicroEngine Compute        | 微控制器用来控制 GFX/Compute engine 上的 compute queues, Compute Engine 一般有 2 个 MEC, 而且每个 MEC 支持 32 个 HW ring(queue) |
| MES | MicroEngine Scheduler      | 一个新的微控制器用来控制 queues, 估计既可以控制 compute queues, 也可以控制 gfx queues, 而且它上面运行的固件可能取代现有的内核 gpu scheduler, 而变成 firmware-based scheduling :) 猜 | 
| RLC | RunList Controller         | 又一个 GFX/Compute engine 里的微控制器，用来处理 GFX/Compute engine 内部的电源管理，至于名字是历史遗留，与它的功能没有毛关系 |

## [Driver](https://docs.kernel.org/gpu/amdgpu/driver-core.html#driver-structure)

|Abbr.| Stands for                 | Description                                    |
|:----|:---------------------------|:-----------------------------------------------|
| KIQ | Kernel Interface Queue     | KMD 的一个控制队列，用来管理 GFX/Compute engine 上的其它队列 |
| IB  | Indirect Buffer            | 某个特定 engine 的 command buffer, 通常不是直接将命令写入硬件 queue 里，而是先将命令写入一块内存，然后再将内存的地址写入硬件 queue |
| HQD | Hardware Queue Descriptor  | kernel queues 或 user queues 将“映射”到一个 HQD, 一个 HQD 可能就是一个 MMIO 地址寄存器。kernel queues 和 user queues 映射的区别是， kernel queues 总是**静态地**映射到**一个 HQD**, 而 user queues 由 MES **动态地** 映射到剩余的所有其它 HQDs |
| MQD | Memory Queue Descriptor    | 定义 queue 的状态，包括 GPU virtual address, doorbell 等，驱动为每个它创建的 queue 设置一个 MQD， MQD 被交给 MES firmware 去映射 |
| KFD | Kernel Fusion Driver       | AMD APU 芯片的内核驱动， 主要用于 HSA 架构的芯片 |
| KGD | Kernel Graphics Driver     | AMD GPU 芯片的内核驱动， 主要用于独立显卡和 OEM 上的 GPU 芯片 |
| RAS | Reliability, Availability, Serviceability | AMDGPU 驱动的一个功能特性，帮助错误检测上报，错误处理和调试 |

# References
- [Kernel Doc](https://docs.kernel.org/gpu/amdgpu/driver-core.html)
