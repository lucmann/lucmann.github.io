---
title: ROCm
date: 2024-09-03 15:10:39+08:00
tags: [ROCm]
categories: computing
---

AMD 的通用计算平台 ROCm (Radeon Open Compute) 是在通用计算领域与 NVIDIA 的 CUDA （Compute Unified Device Architecture) 相抗衡的软件基础设施。ROCm 实现的其实是由 AMD 发起的 HSA (Heterogeneous System Architecture) 标准，所以它的运行时主要由 3 个部分组成:

OOC: ROCm 平台又叫 Boltzmann 平台，原因据说是为了纪念统计力学和热力学领域著名物理学家路德维希·玻尔兹曼，他的工作对理解物质的微观行为和能量分布有着深远的影响。AMD 选取这个名字，可能是为了强调该平台在高性能计算领域的强大计算力和科学计算的应用前景。

- [AMD Compute Common Language Runtimes](https://github.com/ROCm/clr)
    - [HIP runtime](https://github.com/ROCm/HIP) (C++ Heterogeneous-Compute Interface for Portability)
        - 可以算是一种 C++ 方言
        - AMD 提供工具 [HIPIFY](https://github.com/ROCm/HIPIFY) 可以把 CUDA 源代码翻译成 HIP C++
    - [OpenCL runtime](https://www.khronos.org/opencl/) （Open Computing Language)
        - 由 Khronos Group 发布的开放异构系统并行编程语言
        - 语言风格更接近 C
    - [rocclr](https://github.com/ROCm/clr/tree/amd-staging/rocclr) HIP 和 OpenCL 共用的 runtime

<!--more-->

- [HIP](https://github.com/ROCm/HIP)
    - 这个仓库以前主要提供一个 compiler driver utility `hipcc` 脚本，目前 hipcc 已经被移到 AMD 自己 forked [llvm-project/amd/hipcc](https://github.com/ROCm/llvm-project/tree/amd-staging/amd/hipcc)
    - hipcc 之于 HIP-Clang 就像 gcc 之于 GCC
- [ROCR](https://github.com/ROCm/ROCR-Runtime)
    - ROCt libhsakmt-staticdrm HSA Kernel Mode Trunk 用户态库
        - libhsakmt 总是被构建成静态库 .a
        - libhsakmt 通过 DRM 设备节点与 KFD 交互
    - ROCr libhsa-runtime64 
        - 用户可以通过 CMAKE 变量 `BUILD_SHARED_LIBS` 选择构建 libhsa-runtime64 为静态库或动态库

    - ROCR 主要向用户提供 AMD GPU/NPU/HSA 设备内核驱动 (包括 KFD 和 [XDNA](https://patchwork.freedesktop.org/series/136294/)) 的用户态封装和抽象, 类似 amdgpu.ko 与 libdrm_amdgpu 的关系。它的 libhsakmt 通过 KFD 提供的 IOCTL 直接访问 HSA 硬件，而 libhsa-runtime64 主要实现 HSA 标准的 Core Profile 和各厂家的扩展 (extension)。

可以说，ROCm 的整个运行时环境分的 3 大块，分别负责打通
- CLR   向上，面向编程语言
- ROCR  向下，面向内核驱动
- HIP   面向编译系统(HIP 原来提供的 hipcc 已并入 ROCm/llvm-project)

# [CLR](https://github.com/ROCm/clr)

所谓 Common Language Runtimes, 就是指 HIP 和 OpenCL 两种编程语言的运行时, 类似 C/C++ 的 libc.so 和 libstdc++.so。
- libamdhip64.so
- libamdocl64.so.2.1.nnn

这两个动态库各自都需要链接 rocclr

```
target_link_libraries(amdocl PUBLIC rocclr)
```

```
# ROCclr abstracts the usage of multiple AMD compilers and runtimes.
# It is possible to support multiple backends concurrently in the same binary.
option(ROCCLR_ENABLE_HSAIL "Enable support for HSAIL compiler" OFF)
option(ROCCLR_ENABLE_LC    "Enable support for LC compiler"    ON)
option(ROCCLR_ENABLE_HSA   "Enable support for HSA runtime"    ON)
option(ROCCLR_ENABLE_PAL   "Enable support for PAL runtime"    OFF)
```

因为无论 amdhip64 还是 amdocl ，它们也都要调到编译器后端，在 CLR 的实现里支持两个编译器后端：
- `ROCCLR_ENABLE_HSAIL` HSAIL (HSA Intermediate Language, 似乎已经废弃)
    - CLR 的源码中是通过宏 `WITH_COMPILER_LIB` guard
- `ROCCLR_ENABLE_LC`    [COMGR](https://github.com/ROCm/llvm-project/tree/amd-staging/amd/comgr) (Code Object Manager) 目前在 `ROCm/llvm-project/amd/comgr/` 下维护
    - CLR 的源码中是通过宏 `USE_COMGR_LIBRARY` guard

```
bool Program::compileImpl(const std::string& sourceCode,
                          const std::vector<const std::string*>& headers,
                          const char** headerIncludeNames, amd::option::Options* options,
                          const std::vector<std::string>& preCompiledHeaders) {
  if (isLC()) {
    return compileImplLC(sourceCode, headers, headerIncludeNames, options, preCompiledHeaders);
  } else {
    return compileImplHSAIL(sourceCode, headers, headerIncludeNames, options);
  }
}
```

CLR 也支持两个运行时后端：
- `ROCCLR_ENABLE_HSA`
    - 如果使用 HSA runtime, 那么 CLR 需要调用 ROCR (libhsa-runtime64)
    ```
    find_package(hsa-runtime64 1.11 REQUIRED CONFIG
      PATHS
        /opt/rocm/
        ${ROCM_INSTALL_PATH}
      PATH_SUFFIXES
        cmake/hsa-runtime64
        lib/cmake/hsa-runtime64
        lib64/cmake/hsa-runtime64)
    target_link_libraries(rocclr PUBLIC hsa-runtime64::hsa-runtime64)
    ```
- `ROCCLR_ENABLE_PAL` [PAL](https://github.com/GPUOpen-Drivers/pal) (Platform Abstraction Library)
    - 如果使用 PAL runtime, rocclr 需要链接 libpal.so 和 libamdhsaloader.so
    ```
    find_package(AMD_PAL)
    find_package(AMD_HSA_LOADER)
    target_link_libraries(rocclr PUBLIC pal amdhsaloader)
    ```

# [ROCR](https://github.com/ROCm/ROCR-Runtime)

计算世界的 libdrm

ROCR 提供底层设备驱动用户态封装，它将驱动本身进行抽象，很方便扩展到对新设备驱动的支持，例如，在最近 ROCR 就增加了对 [AMD NPU 驱动 XDNA](https://patchwork.freedesktop.org/series/136294/) 的支持

```
hsa_status_t XdnaDriver::DiscoverDriver() {
  const int max_minor_num(64);
  const std::string devnode_prefix("/dev/accel/accel");

  for (int i = 0; i < max_minor_num; ++i) {
    std::unique_ptr<Driver> xdna_drv(
        new XdnaDriver(devnode_prefix + std::to_string(i)));
    if (xdna_drv->Open() == HSA_STATUS_SUCCESS) {
      if (xdna_drv->QueryKernelModeDriver(
              core::DriverQuery::GET_DRIVER_VERSION) == HSA_STATUS_SUCCESS) {
        core::Runtime::runtime_singleton_->RegisterDriver(xdna_drv);
        return HSA_STATUS_SUCCESS;
      } else {
        xdna_drv->Close();
      }
    }
  }

  return HSA_STATUS_ERROR;
}
```

# [AMDKFD](https://github.com/ROCm/ROCK-Kernel-Driver)

计算世界的 AMDGPU

AMDKFD 是 AMD Kernel Fusion Driver, 它基本上是 AMD HSA 在内核中的实现，所以 ROCm/OpenCL 都需要它。[AMDKFD 现在已经是内核 AMDGPU 驱动的一部分](https://lists.freedesktop.org/archives/amd-gfx/2018-July/023673.html), 就像 DAL 一样。

OOC: AMD "Fusion" 是 AMD 在 2011 年推出的一系列处理器，旨在将 CPU 和 GPU 集成到一个芯片上，称为 APU (加速处理单元)。这些处理器的目标是提供更高的性能和能效，特别是在图形和并行计算任务方面。

# 参考
- [What's ROCm](https://rocm.docs.amd.com/en/latest/what-is-rocm.html)
- [Heterogeneous System Architecture standards](https://hsafoundation.com/standards/)
- [HSA Programmer's Reference Manual: HSAIL Virtual ISA](https://www.hsafoundation.com/wp-content/uploads/2021/02/HSA-PRM-1.1.1.pdf)