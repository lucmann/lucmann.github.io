---
title: ROCm
date: 2024-09-03 15:10:39+08:00
tags: [ROCm]
categories: computing
---

> ### *高端硅片可以构建市场，但最终，软件支持往往决定了赢家和输家*
> -- Jack Huynh, AMD 高级副总裁兼计算和图形业务集团总经理

AMD 的通用计算平台 ROCm (Radeon Open Compute) 是在通用计算领域与 NVIDIA 的 CUDA （Compute Unified Device Architecture) 相抗衡的软件基础设施。ROCm 实现的其实是由 AMD 发起的 HSA (Heterogeneous System Architecture) 标准，所以它的运行时主要由 3 个部分组成:

OOC: ROCm 平台又叫 Boltzmann 平台，原因据说是为了纪念统计力学和热力学领域著名物理学家路德维希·玻尔兹曼，他的工作对理解物质的微观行为和能量分布有着深远的影响。AMD 选取这个名字，可能是为了强调该平台在高性能计算领域的强大计算力和科学计算的应用前景。

<!--more-->

- [AMD Compute Common Language Runtimes](https://github.com/ROCm/clr)
    - [HIP runtime](https://github.com/ROCm/HIP) (C++ Heterogeneous-Compute Interface for Portability)
        - 可以算是一种 C++ 方言
        - AMD 提供工具 [HIPIFY](https://github.com/ROCm/HIPIFY) 可以把 CUDA 源代码翻译成 HIP C++
    - [OpenCL runtime](https://www.khronos.org/opencl/) （Open Computing Language)
        - 由 Khronos Group 发布的开放异构系统并行编程语言
        - 语言风格更接近 C
    - [rocclr](https://github.com/ROCm/clr/tree/amd-staging/rocclr) HIP 和 OpenCL 共用的 runtime
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

ROCR 是 KMD 的封装(主要是 KMD 实现的 ioctl 和设备,内存管理接口)，类似 libdrm, 但 ROCR 也对驱动动本身(因为KMD可能会有 KFD, AMDGPU, XDNA)进行了抽象，便于扩展到新驱动架构。例如，在最近 ROCR 就增加了对 [AMD NPU 驱动 XDNA](https://patchwork.freedesktop.org/series/136294/) 的支持。

OOC: libhsa-runtime64.so 打开的是 `/dev/kfd`

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

# [ROCm/llvm-project](https://github.com/ROCm/llvm-project)


[ROCm/llvm-project](https://github.com/ROCm/llvm-project) 是 AMD 专为 ROCm fork 出来的 LLVM 仓库，它里面包含所有 [llvm-project](https://github.com/llvm/llvm-project) 上游变更，再加上 AMD 特有的 **amd** 子目录。 在 amd 目录下存在 3 个组件:

- amd_comgr
- device-libs
- hipcc

这 3 个组件可以有两种构建方式:
- 因为每个子目录下都有自己的 CMakeLists.txt, 所以可以通过 cmake 的 `-S` 选项分别逐个构建
  - 虽然这 3 个组件是可以独立构建的，但 amd_comgr 依赖 device-libs
- 也可以通过 llvm 构建选项 `LLVM_EXTERNAL_PROJECTS`, `LLVM_EXTERNAL_DEVICELIBS_SOURCE_DIR`, `LLVM_EXTERNAL_COMGR_SOURCE_DIR` 与 llvm 一起构建
  - 这种方式好像需要运行 `amd/utils/omnibus.sh`, 试了一下没有成功，还是第一种方式相对简单些

构建顺序应该只能是：

- `cmake -S llvm -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=On -DCMAKE_INSTALL_PREFIX=~/.local/rocm/6.2.0/llvm -DLLVM_ENABLE_PROJECTS="clang;lld;llvm" -DLLVM_TARGETS_TO_BUILD="BPF;AMDGPU;host" -DLLVM_LIBDIR_SUFFIX=64 -DLLVM_BUILD_LLVM_DYLIB=OFF -DBUILD_SHARED_LIBS=OFF -DLLVM_USE_LINKER=gold`
  - 构建 comgr 不支持 shared libraries LLVM,  所以必须将 LLVM 编译成静态库 (`-DBUILD_SHARED_LIBS=OFF`)
  - [`CMAKE_BUILD_TYPE` 不要选 Debug 或 RelWithDebInfo, 除非你有足够的硬盘空间](https://github.com/ROCm/llvm-project/issues/37)
- ` cmake -S amd/device-libs -B build-rocm -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=~/.local/rocm/6.2.0 -DLLVM_DIR=~/.local/rocm/6.2.0/llvm/lib64/cmake/llvm`
  - `LLVM_DIR` 要设置的是包含 `LLVMConfig.cmake` 的路径, 而不是 `llvm-config --prefix` 输出的路径
- `cmake -S amd/comgr -B build-rocm -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=~/.local/rocm/6.2.0 -DLLVM_DIR=~/.local/rocm/6.2.0/llvm/lib64/cmake/llvm -DCMAKE_MODULE_PATH=~/local/rocm/6.2.0/lib/cmake/AMDDeviceLibs`

# [AMDKFD](https://github.com/ROCm/ROCK-Kernel-Driver)

计算世界的 AMDGPU

AMDKFD 是 AMD Kernel Fusion Driver, 它基本上是 AMD HSA 在内核中的实现，所以 ROCm/OpenCL 都需要它。[AMDKFD 在 2014 年合入 Linux kernel 3.19 的主线](https://lwn.net/Articles/619581/), 而在 2018 年 AMDKFD 正式与 AMDGPU 合并成为一个单一内核模块 [amdgpu.ko](https://lists.freedesktop.org/archives/amd-gfx/2018-July/023673.html)。

OOC: AMD "Fusion" 是 AMD 在 2011 年推出的一系列处理器，旨在将 CPU 和 GPU 集成到一个芯片上，称为 APU (加速处理单元)。这些处理器的目标是提供更高的性能和能效，特别是在图形和并行计算任务方面。

AMDKFD 有 3 个内核配置选项:
- HSA_AMD     基本上在 X86_64, ARM64 和 PPC64 系统上是默认打开的
- HSA_AMD_SVM 基于 HMM 的共享虚拟内存管理器
- HSA_AMD_P2P 基于 PCI_P2PDMA 的多 GPU 间数据传输功能

# [UDNA](https://winbuzzer.com/2024/09/10/amd-combines-rdna-and-cdna-into-udna-to-rival-nvidia-cuda-xcxwbn/)

在未来， AMD 计划将 RDNA 和 CDNA 架构统一为 UDNA 架构，并为该架构引入张量计算单元，以更好地支持 AI 计算，统一的 UDNA 架构对开发者来说将更友好，可以让开发者利用相同的底层微架构来实现 AI, HPC 和游戏。

# [HSA](https://hsafoundation.com/) vs [UXL](https://uxlfoundation.org/)
  HSA 标准是 ROCm 的基石之一，它让用户在通用计算领域除了 CUDA 之外能有另外一个选择。而 UXL (Unified Acceleration) Foundation 旨在为不同架构不同厂商的加速器构建统一的软件生态系统，从而也成为另一个能与 nVIDIA 的 GPU 和 CUDA 相抗衡的计算平台。 

# 参考
- [What's ROCm](https://rocm.docs.amd.com/en/latest/what-is-rocm.html)
- [Heterogeneous System Architecture standards](https://hsafoundation.com/standards/)
- [HSA Programmer's Reference Manual: HSAIL Virtual ISA](https://www.hsafoundation.com/wp-content/uploads/2021/02/HSA-PRM-1.1.1.pdf)
- [AMDKFD Kernel Driver](https://lwn.net/Articles/619581/)