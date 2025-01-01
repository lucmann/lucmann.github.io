---
title: Vulkan
date: 2021-04-02 21:21:27
tags: Vulkan
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

(注意 vulkansdk 默认都会将指定安装的项目安装在当前路径下的 `$(arch)` 目录下，这个安装路径在编译 Vulkan-Tools 时需要被设置到 Vulkan-Tools 的 cmake 变量 `-DVULKAN_HEADERS_INSTALL_DIR=/home/luc/gh/1.3.290.0/x86_64` 原因是 Vulkan-Tools 需要用到这个路径下的 Python 模块)

## [Vulkan-Headers](https://github.com/KhronosGroup/Vulkan-Headers)

Vulkan-Headers 主要包含 Vulkan API 的头文件和一些用于(根据 Khronos XML 文件)自动生成头文件的Python 脚本。Vulkan-Headers 的版本变更和 vulkan-sdk 是同步的，基本上所有 Vulkan 的相关的工具或库都依赖它。

例如 Vulkan-Tools 中的 vulkaninfo, 当需要构建它时，就需要指定 `-DVULKAN_HEADERS_INSTALL_DIR`, 而且必须将 Vulkan-Tools 仓库的 tag 检出到和 Vulkan-Headers 仓库的一样才能编译成功。

# [Vulkan-Hpp](https://github.com/KhronosGroup/Vulkan-Hpp)

Vulkan-Hpp 旨在为 Vulkan C API 提供头文件(header only)形式的 C++ 绑定, 以此来简化 Vulkan 应用的开发过程同时不引入额外的 CPU 运行时开销。 Vulkan-Hpp 依赖 Vulkan-Headers, 而且它俩同时是 vulkan-sdk 的一部分, 它们三者的版本号应该都要一致 (定义在 `${CMAKE_INSTALL_PREFIX}/include/vulkan/vulkan_core.h` 的 `VK_HEADER_VERSION`)。 实际上， Vulkan-Hpp 里包含一个 `VulkanHppGenerator` 程序，它会自动重新生成 vulkan 的一些头文件。

```mermaid
flowchart LR
    A["Vulkan Registry XML"]
    B["Vulkan .h/.hpp headers"]
    C["Vulkan .hpp headers"]
    A-->|Vulkan-Headers .py|B
    B-->|VulkanHppGenerator|C
```

## [Vulkan-Tools](https://github.com/KhronosGroup/Vulkan-Tools)

- `vulkaninfo --summary`

```
VULKANINFO
==========

Vulkan Instance Version: 1.3.261


Instance Extensions: count = 17
-------------------------------
VK_EXT_debug_report                    : extension revision 10
VK_EXT_debug_utils                     : extension revision 2
VK_EXT_headless_surface                : extension revision 1
VK_EXT_surface_maintenance1            : extension revision 1
VK_EXT_swapchain_colorspace            : extension revision 5
VK_KHR_device_group_creation           : extension revision 1
VK_KHR_external_fence_capabilities     : extension revision 1
VK_KHR_external_memory_capabilities    : extension revision 1
VK_KHR_external_semaphore_capabilities : extension revision 1
VK_KHR_get_physical_device_properties2 : extension revision 2
VK_KHR_get_surface_capabilities2       : extension revision 1
VK_KHR_portability_enumeration         : extension revision 1
VK_KHR_surface                         : extension revision 25
VK_KHR_surface_protected_capabilities  : extension revision 1
VK_KHR_xcb_surface                     : extension revision 6
VK_KHR_xlib_surface                    : extension revision 6
VK_LUNARG_direct_driver_loading        : extension revision 1

Instance Layers: count = 13
---------------------------
VK_LAYER_FROG_gamescope_wsi       Gamescope WSI (XWayland Bypass) Layer (x86_64) 1.3.221  version 1
VK_LAYER_INTEL_nullhw             INTEL NULL HW                                  1.1.73   version 1
VK_LAYER_KHRONOS_profiles         Khronos Profiles layer                         1.3.280  version 1
VK_LAYER_KHRONOS_shader_object    Khronos Shader object layer                    1.3.280  version 1
VK_LAYER_KHRONOS_synchronization2 Khronos Synchronization2 layer                 1.3.280  version 1
VK_LAYER_KHRONOS_validation       Khronos Validation Layer                       1.3.280  version 1
VK_LAYER_LUNARG_api_dump          LunarG API dump layer                          1.3.280  version 2
VK_LAYER_LUNARG_gfxreconstruct    GFXReconstruct Capture Layer Version 1.0.3     1.3.280  version 4194307
VK_LAYER_LUNARG_monitor           Execution Monitoring Layer                     1.3.280  version 1
VK_LAYER_LUNARG_screenshot        LunarG image capture layer                     1.3.280  version 1
VK_LAYER_MANGOHUD_overlay_x86_64  Vulkan Hud Overlay                             1.3.0    version 1
VK_LAYER_MESA_device_select       Linux device selection layer                   1.3.211  version 1
VK_LAYER_MESA_overlay             Mesa Overlay layer                             1.3.211  version 1

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
    ![vkcube](/images/vk/vkcube.gif)

## [SPIRV-Tools](https://github.com/KhronosGroup/SPIRV-Tools)

SPIRV-Tools 主要收集了与 shader 相关的一整套工具链，包括编译，链接，优化，反汇编等等。它依赖 [SPIRV-Headers](https://github.com/KhronosGroup/SPIRV-Headers)

SPIRV-Tools 包含的比较常用的工具:

- spirv-dis
- spirv-opt

# [Vulkan-Samples](https://github.com/KhronosGroup/Vulkan-Samples)

`./build/app/bin/Debug/x86_64/vulkan_samples sample surface_rotation`
![surface_rotation](/images/vk/surface_rotation.png)

# [VulkanExamples](https://github.com/jherico/VulkanExamples)

VulkanExamples 是将大部分 [Sascha Willems 的 Vulkan examples](https://github.com/SaschaWillems/Vulkan) 移植到 Vulkan-Hpp, 但当我试着在 Ubuntu 20.04.3 上编译并在 LAVApipe 是运行时，发现一个问题，大多数 Demo 会 assert:

```
gears: ../src/vulkan/runtime/vk_render_pass.c:2347: vk_common_CmdBeginRenderPass2: Assertion `image_view->format == pass_att->format' failed.
```

原因是 VulkanExamples 的 `ExampleBase::colorformat` 默认是 `vk::Format::eB8G8R8A8Unorm`, 而驱动侧 wsi 从窗口系统(Ubuntu 20.04.3是 X11) 获取的 visual 格式会在下面的 format 数据里顺序匹配:

```
static const VkFormat formats[] = {
   VK_FORMAT_R5G6B5_UNORM_PACK16,
   VK_FORMAT_B8G8R8A8_SRGB,
   VK_FORMAT_B8G8R8A8_UNORM,
   VK_FORMAT_A2R10G10B10_UNORM_PACK32,
};
```

而 VulkanExamples base 只会从 `physicalDevice.getSurfaceFormatsKHR()` 返回的列表里取第一个格式，而它取回的是 `vk::Format::eB8G8R8A8Srgb`, 所以才会导致驱动 `vk_common_CmdBeginRenderPass2()` 的断言失败。mesa 的 wsi common 层之所以会将 `VK_FORMAT_B8G8R8A8_SRGB` 放到 `VK_FORMAT_B8G8R8A8_UNORM` 前面，应该是考虑到在应用程序中可能 sRGB 颜色空间使用更为广泛吧(doge)。

而且 [KhronosGroup/Vulkan-Samples](https://github.com/KhronosGroup/Vulkan-Samples) 自从 [38d628b032a8](https://github.com/KhronosGroup/Vulkan-Samples/commit/38d628b032a88cf032e88877c1b75aa470333c32) 就已经首选 `vk::Format::eR8G8B8A8Srgb` 作为 swapchain image 的格式了。

将 `ExampleBase::colorformat` 改为 sRGB 后的对比效果(哪个是 sRGB 呢?)
![gears-srgb](/images/vk/gears-srgb.gif)
![gears-rgb](/images/vk/gears-rgb.gif)

# Vulkan ICD

Vulkan ICD (Installable Client Driver) 可安装客户端驱动程序是 Vulkan 生成系统中的关键组件。它在 Vulkan 应用程序和系统上安装的各种 Vulkan 驱动程序之间充当桥梁。每个 Vulkan 驱动程序有带有一个 ICD JSON 文件，它里面描述了驱动动态库文件的路径，以便 Vulkan-Loader 可以枚举系统安装的每个 Vulkan 驱动。ICD Json 文件的路径和命名都是规范的:

- 命名规范
    - `driver_name`_icd.`$arch`.json
- 路径规范
    - `${CMAKE_INSTALL_PREFIX}`/`${CMAKE_INSTALL_DATADIR}`/vulkan/icd.d/ 

Vulkan ICD 可以类比 OpenGL 世界的 [GLVND (GL Vendor-Neutral Dispatch library)](https://gitlab.freedesktop.org/glvnd/libglvnd), GLVND 的目的同样是在同一系统上允许多个 OpenGL 驱动库共存，并在运行时决定将每个 API 调用分派给哪个供应商的驱动。只不过 GLVND 的 GLX 没有使用 JSON 文件，而是依靠环境变量:

export __GLX_VENDOR_LIBRARY_NAME=`driver_name`

`driver_name` 就是 OpenGL 驱动库文件 libGLX_`driver_name`.so 的一部分, 这种方式更依赖于动态链接库 *dl.so* 

而 GLVND 的 EGL 实现基本上与 Vulkan ICD 是一样的(可能Vulkan ICD 就是来源于 GLVND EGL ICD)。GLVND EGL ICD 的 JSON 文件命名规范和安装路径规范:

- 命名规范
    - 10_`myvendor`.`$arch`.json
- 路径规范
    - `${CMAKE_INSTALL_PREFIX}`/`${CMAKE_INSTALL_DATADIR}`/glvnd/egl_vendor.d/

# Vulkan WSI

Vulkan WSI (Window System Integration) 是Vulkan API 的一个重要组成部分，用于将渲染结果显示在不同平台的窗口系统上。WSI 通过一系列可选的 Vulkan 扩展来实现，这些扩展抽象了每个平台的窗口机制，使得 Vulkan API 可以在各种平台上使用。

- VK_KHR_surface (object/handle `VkSurfaceKHR`)
    - VK_KHR_win32_surface
    - VK_KHR_android_surface
    - VK_KHR_wayland_surface
    - VK_KHR_xcb_surface, VK_KHR_xlib_surface
    - VK_MVK_macos_surface
    - VK_MVK_ios_surface
- VK_KHR_swapchain (object/handle `VkSwapchainKHR`)

*注意: `VkSurfaceKHR` 和 `VkSwapchainKHR` 本质上都是一个指向结构体的指针类型，即 `struct VkSurfaceKHR_T *` (详见 [vulkan_core.h](https://github.com/KhronosGroup/Vulkan-Headers/blob/main/include/vulkan/vulkan_core.h#L57))*

- Vulkan SDK 自带的 vulkaninfo 在 Ubuntu 20.04/22.04 (llvmpipe Vulkan 驱动)段错误
    - Vulkan SDK 自带的 vulkaninfo 在编译时应该没有关闭 `-DBUILD_WSI_WAYLAND_SUPPORT`
    - Ubuntu 20.04/22.04 的 Mesa 实现中 `vkIcdWsiPlatform` 不支持 wayland 平台
        - mesa 中的 `struct wsi_device` 定义了一个 `struct wsi_interface *` 的一个数组 `wsi[VK_ICD_WSI_PLATFORM_MAX]`, 在 Ubuntu 20.04/22.04 上该表 `VK_ICD_WSI_PLATFORM_WAYLAND` 对应的 wsi_interface 是空的
            ```
            (gdb) p wsi_device->wsi
            $2 = {0x0, 0x0, 0x0, 0x555555744e00, 0x555555744e00, 0x0, 0x0, 0x0, 0x555555702560, 0x555555753330}
            (gdb) ptype surface->platform
type = enum {VK_ICD_WSI_PLATFORM_MIR, VK_ICD_WSI_PLATFORM_WAYLAND, VK_ICD_WSI_PLATFORM_WIN32, VK_ICD_WSI_PLATFORM_XCB, VK_ICD_WSI_PLATFORM_XLIB, VK_ICD_WSI_PLATFORM_ANDROID, VK_ICD_WSI_PLATFORM_MACOS, VK_ICD_WSI_PLATFORM_IOS, VK_ICD_WSI_PLATFORM_DISPLAY, VK_ICD_WSI_PLATFORM_HEADLESS, VK_ICD_WSI_PLATFORM_METAL, VK_ICD_WSI_PLATFORM_DIRECTFB, VK_ICD_WSI_PLATFORM_VI, VK_ICD_WSI_PLATFORM_GGP, VK_ICD_WSI_PLATFORM_SCREEN, VK_ICD_WSI_PLATFORM_FUCHSIA}
            ```

# [LAVApipe](https://gitlab.freedesktop.org/mesa/mesa/-/tree/main/src/gallium/frontends/lavapipe)

Vulkan 的软实现，不依赖任何 GPU, 但它和 llvmpipe 一样，依赖 LLVM。构建它依赖:

- LLVM
- [SPIRV-LLVM-Translator](https://github.com/KhronosGroup/SPIRV-LLVM-Translator)
    - (SPIRV-LLVM-Translator 的版本似乎与 LLVM 的版本有严格的耦合，亲测如果 `BASE_LLVM_VERSION=20.0.0`, 实际安装的 LLVM 是 16.0.0 编译失败)

# [Zink](https://docs.mesa3d.org/drivers/zink.html)

Zink 不是某个具体 GPU 的驱动，而是一个将OpenGL API 调用翻译成 Vulkan API, 以在只有Vulkan 驱动的平台上提供完整OpenGL支持。例如，同时编译安装 Mesa 的 Zink (-Dgallium-drivers=zink)和 LAVApipe (-Dvulkan-drivers=swrast) 驱动，你就不仅可以跑 vkcube 也可以跑 glxgears。

```
  User defined options
    buildtype                    : debug
    prefix                       : /usr
    cpp_rtti                     : false
    gallium-drivers              : zink
    glx                          : dri
    llvm                         : enabled
    platforms                    : x11
    vulkan-drivers               : swrast
```

但是在软渲染上跑需要提供一些环境变量 `MESA_LOADER_DRIVER_OVERRIDE=zink LIBGL_KOPPER_DRI2=true LIBGL_ALWAYS_SOFTWARE=true ZINK_DEBUG=nir glxgears`

- `LIBGL_KOPPER_DRI2=true` 是因为我的环境上没有 DRI3, Zink 在没有 DRI3 支持, 且用户没有要求使用 DRI2 的情况下是不被使能的。
- `LIBGL_ALWAYS_SOFTWARE=true` 是因为 zink 的实现要求 [`/* allow software rendering only if forced by the user */`](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/gallium/drivers/zink/zink_screen.c#L1763)
- `ZINK_DEBUG=nir` 为了能直观地看到驱动使用的是 Zink 而非 LLVMpipe 的OpenGL实现，特意加了 Zink 调试环境变量，将 shader 的 NIR 打印出来
    - glxgears on Zink
    ![glxgears on Zink](/images/vk/glxgears-on-zink.gif)
    - glmark2 on Zink
    ![glmark2 on Zink](/images/vk/glmark2-on-zink.gif)
