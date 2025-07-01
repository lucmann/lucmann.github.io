---
title: AMD GPU Vulkan Drivers for Linux
date: 2024-09-02 11:01:15+08:00
tags: Vulkan
categories: graphics
---

AMD 在 Linux 下维护着两套开源 Vulkan 驱动: 
- [AMDVLK](https://github.com/GPUOpen-Drivers/AMDVLK)
    - 从闭源 Windows Vulkan 驱动适配而来，主要不同在 shader 编译后端, AMDVLK 使用基于 LLVM 的 LLPC
- [RADV](https://docs.mesa3d.org/drivers/radv.html)
    - 包含在 Mesa 内，有两套编译后端: 基于 LLVM 的后端和基于 NIR 的 [ACO](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/amd/compiler/README.md) (**A**MD **CO**mpiler)，但默认的是 ACO

<!--more-->

# AMDVLK

AMDVLK 的 README 有一张[架构图](https://github.com/GPUOpen-Drivers/AMDVLK?tab=readme-ov-file#amd-open-source-driver-for-vulkan), 非常清晰地展示了整个驱动包含的组件和层次结构。

![topLevelArch](/images/radv/topLevelArch.png)

AMDVLK 驱动由 5 个代码仓库构建:

- [LLVM](https://github.com/GPUOpen-Drivers/llvm-project)
- [XGL](https://github.com/GPUOpen-Drivers/xgl) Vulkan API Translator
- [LLPC](https://github.com/GPUOpen-Drivers/llpc) LLVM-based Pipeline Compiler
- [GPURT](https://github.com/GPUOpen-Drivers/gpurt) GPU Ray Tracing Library
- [PAL](https://github.com/GPUOpen-Drivers/pal) Platform Abstraction Library

其中 XGL, LLPC, GPURT 都与 **PAL** 有关联。

# RADV (Radeon Vulkan)

RADV 和其它 Mesa 的 GPU UMD 一样都是使用 libdrm 库与 KMD 打交道，但后来为了[GPU 虚拟化(amdgpu-virto) 支持 Native Context](https://gitlab.freedesktop.org/mesa/mesa/-/merge_requests/32067) 将大部分 libdrm_amdgpu 库的函数换成了 **ac_drm_** 开头的函数, 后来为了[GPU 虚拟化(amdgpu-virto) 支持 Native Context](https://gitlab.freedesktop.org/mesa/mesa/-/merge_requests/21658) 也对 ac_drm 函数接口进行了的修改。 经过这两次修改，RADV 使用的 libdrm_amdgpu 函数基本分为两类：

- 直接调用型
- "inline"型

| Function                      | IOCTL CMD                       | Read/Write |
|:------------------------------|:--------------------------------|:-----------|
| ac_drm_bo_set_metadata        | GEM_METADATA                    | RW         |
| ac_drm_bo_query_info          | GEM_METADATA, GEM_OP            | RW         |
| ac_drm_bo_wait_for_idle       | GEM_WAIT_IDLE                   | RW         |
| ac_drm_bo_va_op_raw           | GEM_VA                          | RW         |
| ac_drm_cs_query_reset_state2  | CTX                             | RW         |
| ac_drm_cs_query_fence_status  | DRM_IOCTL_AMDGPU_WAIT_CS        | N/A        |
| ac_drm_cs_submit_raw2         | CS                              | RW         |
| ac_drm_query_hw_ip_count      | INFO                            | W          |
| ac_drm_query_hw_ip_info       | INFO                            | W          |
| ac_drm_query_firmware_version | INFO                            | W          |
| ac_drm_query_uq_fw_area_info  | INFO                            | W          |
| ac_drm_read_mm_registers      | INFO                            | W          |
| ac_drm_query_info             | INFO                            | W          |
| ac_drm_query_sensor_info      | INFO                            | W          |
| ac_drm_query_video_caps_info  | INFO                            | W          |
| ac_drm_query_gpuvm_fault_info | INFO                            | W          |
| ac_drm_vm_reserve_vmid        | VM                              | RW         |
| ac_drm_vm_unreserve_vmid      | VM                              | RW         |

