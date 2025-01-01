---
title: AMD GPU Vulkan Drivers for Linux
date: 2024-09-02 11:01:15+08:00
tags: Vulkan
categories: graphics
---

AMD 在 Linux 下维护着两套开源 Vulkan 驱动: 
- [AMDVLK](https://github.com/GPUOpen-Drivers/AMDVLK)
    - 从闭源 Windows Vulkan 驱动适配而来，主要不同在 shader 编译后端, AMDVLK 使用基于 LLVM 的 LLPC
- [RADV](https://gitlab.freedesktop.org/mesa/mesa/-/tree/main/src/amd/vulkan)
    - 是 Mesa 项目的一部分，主要由社区驱动。使用的 shader 编译后端是基于 NIR 的 ACO (AMD COmpiler)

这里主要是记录一下两者的异同点，学习两者不同的设计策略及其作出选择的背后理念。

<!--more-->

# [AMDVLK](https://github.com/GPUOpen-Drivers/AMDVLK)

AMDVLK 的 README 有一张[架构图](https://github.com/GPUOpen-Drivers/AMDVLK?tab=readme-ov-file#amd-open-source-driver-for-vulkan), 非常清晰地展示了整个驱动包含的组件和层次结构。

![topLevelArch](/images/radv/topLevelArch.png)

AMDVLK 驱动由 5 个代码仓库构建:

- [LLVM](https://github.com/GPUOpen-Drivers/llvm-project)
- [XGL](https://github.com/GPUOpen-Drivers/xgl) Vulkan API Translator
- [LLPC](https://github.com/GPUOpen-Drivers/llpc) LLVM-based Pipeline Compiler
- [GPURT](https://github.com/GPUOpen-Drivers/gpurt) GPU Ray Tracing Library
- [PAL](https://github.com/GPUOpen-Drivers/pal) Platform Abstraction Library

其中 XGL, LLPC, GPURT 都与 **PAL** 有关联。
