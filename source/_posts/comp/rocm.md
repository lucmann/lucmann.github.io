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
    - ROCt libhsakmt HSA Kernel Mode Trunk 用户态库
    - ROCr libhsa-runtime64 

  ROCR 主要向用户提供 HSA 内核实现 (KFD) 的用户态封装和抽象。它的 libhsakmt 通过 KFD 提供的 IOCTL 直接访问 HSA 硬件，而 libhsa-runtime64 主要实现 HSA 标准的 Core Profile 和各厂家的扩展 (extension)。
