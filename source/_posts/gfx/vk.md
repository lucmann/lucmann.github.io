---
title: Vulkan
date: 2021-04-02 21:21:27
tags: vulkan
categories: graphics
---

Vulkan 是一个低开销、跨平台的二维和三维图形与计算的应用程序接口，由 Khronos 在2015年在 GDC 上首次发布。它旨在提供高效能和更均衡的 CPU 和 GPU 占用，类似于 Direct3D 12 和 AMD Mantle。

在这里主要收集一些与 Vulkan 强相关的一些项目，便于 Vulkan 的学习和使用。

<!--more-->


# [SPIRV-Tools](https://github.com/KhronosGroup/SPIRV-Tools)

SPIRV-Tools 主要收集了与 shader 相关的一整套工具链，包括编译，链接，优化，反汇编等等。它依赖 [SPIRV-Headers](https://github.com/KhronosGroup/SPIRV-Headers)

SPIRV-Tools 包含的比较常用的工具:

- vulkaninfo
- spirv-opt

# [Vulkan-Samples](https://github.com/KhronosGroup/Vulkan-Samples)

# lavapipe

Vulkan 的软实现，不依赖任何 GPU。是 Mesa 的一部分，构建它依赖

- LLVM
- [SPIRV-LLVM-Translator](https://github.com/KhronosGroup/SPIRV-LLVM-Translator)
    - (SPIRV-LLVM-Translator 的版本似乎与 LLVM 的版本有严格的耦合，亲测 `BASE_LLVM_VERSION=20.0.0`, 实现安装的 LLVM 如果是 16.0.0 是编译失败的)

# [Zink](https://docs.mesa3d.org/drivers/zink.html)

Zink 不是某个具体 GPU 的驱动，而是一个将OpenGL API 调用翻译成 Vulkan API, 以在只有Vulkan 驱动的平台上提供完整OpenGL支持。


