---
title: Mesa 对 LLVM 的依赖
date: 2023-06-25
tags: [Mesa]
categories: graphics
---

# LLVM 和 Mesa 简介

## LLVM (Low-Level Virtual Machine)

<!--more-->

## Mesa

Mesa 是 GPU 驱动的 UMD 部分（开源，遵循 MIT 开源协议). 它支持的 API 有 OpenGL, OpenCL, Vulkan, VAAPI, VDPAU 等

# Mesa 中的驱动对 LLVM 的依赖

| Mesa driver                                 | dependent LLVM version  |
|:--------------------------------------------|:-----------------------:|
| `vulkan-drivers=amd`                        | 15.0.0+                 |
| `gallium-drivers=radeonsi`                  | 15.0.0+                 |
| `intel-clc=enabled`                         | 13.0.0+                 |
| `gallium-opencl=icd`                        | 11.0.0+                 |
| `microsoft-clc=enabled`                     | 10.0.0+                 |
| `gallium-drivers=swrast` and `llvm=enabled` |  5.0.0+                 |

