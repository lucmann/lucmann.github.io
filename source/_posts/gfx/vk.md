---
title: Vulkan
date: 2021-04-02 21:21:27
tags: vulkan
categories: graphics
---

Vulkan 是一个低开销、跨平台的二维和三维图形与计算的应用程序接口，由 Khronos 在2015年在 GDC 上首次发布。它旨在提供高效能和更均衡的 CPU 和 GPU 占用，类似于 Direct3D 12 和 AMD Mantle。

在这里主要收集一些与 Vulkan 强相关的一些项目，便于 Vulkan 的学习和使用。

<!--more-->

# [vulkan-sdk](https://vulkan.lunarg.com/)

Vulkan SDK 为开发者提供了开发和调试 Vulkan 应用程序的关键工具，主要包括:

- Vulkan-Headers
- Vulkan-Tools
- SPIRV-Tools
- glslang
- VulkanMemoryAllocator (vma)
- Vulkan-Utilities-Library (vul)

这些基本都是 C++ 编写的工具或库。

LunarG 官方的 vulkan-sdk 同时会提供 Binary 和一个 vulkansdk 脚本，以及一个环境变量配置脚本。通常解压 (`tar -xvJf`) 后只需要

`source setup-env.sh`

SDK 中包含的所有库，应用程序及 cmake 文件都会在当前终端生效。另外，vulkansdk 允许用户方便地有选择地安装 SDK 包含的工具，例如单独下载安装 Vulkan-Headers:

`./vulkansdk --skip-deps vulkan-headers`

(注意 vulkansdk 默认都会将指定安装的目标安装在当前路径下的 `arch` 目录下，这个安装路径在编译 Vulkan-Tools 时需要被设置为 `-DVULKAN_HEADERS_INSTALL_DIR=/home/luc/gh/1.3.290.0/x86_64` 原因是 Vulkan-Tools 需要用到这个路径下的 Python 模块)

# [Vulkan-Headers](https://github.com/KhronosGroup/Vulkan-Headers)

Vulkan-Headers 主要包含 Vulkan API 的头文件和一些用于(根据 Khronos XML 文件)自动生成头文件的Python 脚本。Vulkan-Headers 的版本变更和 vulkan-sdk 是同步的，基本上所有 Vulkan 的相关的工具或库都依赖它。

例如 Vulkan-Tools 中的 vulkaninfo, 当需要构建它时，就需要指定 `-DVULKAN_HEADERS_INSTALL_DIR`, 而且必须将 Vulkan-Tools 仓库的 tag 检出到和 Vulkan-Headers 仓库的一样才能编译成功。

# [Vulkan-Tools](https://github.com/KhronosGroup/Vulkan-Tools)

- vulkaninfo
    - `vulkaninfo --summary`
    ```
    Devices:
    ========
    GPU0:
            apiVersion         = 1.3.255
            driverVersion      = 0.0.1
            vendorID           = 0x10005
            deviceID           = 0x0000
            deviceType         = PHYSICAL_DEVICE_TYPE_CPU
            deviceName         = llvmpipe (LLVM 15.0.7, 256 bits)
            driverID           = DRIVER_ID_MESA_LLVMPIPE
            driverName         = llvmpipe
            driverInfo         = Mesa 23.2.1-1ubuntu3.1~22.04.2 (LLVM 15.0.7)
            conformanceVersion = 1.3.1.1
            deviceUUID         = 6d657361-3233-2e32-2e31-2d3175627500
            driverUUID         = 6c6c766d-7069-7065-5555-494400000000
    GPU1:
            apiVersion         = 1.3.292
            driverVersion      = 0.0.1
            vendorID           = 0x10005
            deviceID           = 0x0000
            deviceType         = PHYSICAL_DEVICE_TYPE_CPU
            deviceName         = llvmpipe (LLVM 16.0.0, 256 bits)
            driverID           = DRIVER_ID_MESA_LLVMPIPE
            driverName         = llvmpipe
            driverInfo         = Mesa 24.3.0-devel (git-d58f7a24d1) (LLVM 16.0.0)
            conformanceVersion = 1.3.1.1
            deviceUUID         = 6d657361-3234-2e33-2e30-2d6465766500
            driverUUID         = 6c6c766d-7069-7065-5555-494400000000
    ```
- vkcube/vkcubepp
    - `vkcube --gpu_number 1 --width 800 --height 600`
    ![vkcube](vkcube.gif)

# [SPIRV-Tools](https://github.com/KhronosGroup/SPIRV-Tools)

SPIRV-Tools 主要收集了与 shader 相关的一整套工具链，包括编译，链接，优化，反汇编等等。它依赖 [SPIRV-Headers](https://github.com/KhronosGroup/SPIRV-Headers)

SPIRV-Tools 包含的比较常用的工具:

- spirv-dis
- spirv-opt

# [Vulkan-Samples](https://github.com/KhronosGroup/Vulkan-Samples)

# [LAVApipe](https://gitlab.freedesktop.org/mesa/mesa/-/tree/main/src/gallium/frontends/lavapipe)

Vulkan 的软实现，不依赖任何 GPU。是 Mesa 的一部分，构建它依赖:

- LLVM
- [SPIRV-LLVM-Translator](https://github.com/KhronosGroup/SPIRV-LLVM-Translator)
    - (SPIRV-LLVM-Translator 的版本似乎与 LLVM 的版本有严格的耦合，亲测如果 `BASE_LLVM_VERSION=20.0.0`, 实际安装的 LLVM 是 16.0.0 编译失败)

# [Zink](https://docs.mesa3d.org/drivers/zink.html)

Zink 不是某个具体 GPU 的驱动，而是一个将OpenGL API 调用翻译成 Vulkan API, 以在只有Vulkan 驱动的平台上提供完整OpenGL支持。


