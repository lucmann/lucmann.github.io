---
title: CUDA Thread Hierarchy vs OpenGL Compute Shader Thread Hierarchy
date: 2021-06-28 15:33:52
tags: [cuda, compute]
categories: usr
---

# CUDA Thread Hierarchy

{% asset_img cuda-thread-hier.png %}

CUDA中的`kernel`是一段可在GPU独立运行的小程序，而且这段程序会被实例化(类似的说法还有**issued**, **executed**, **invoked**)成非常多的`thread`, 这些线程可**并发地**在GPU上运行。程序员或者说编译器将这些线程组织成

- Thread
- Thread Block
- Grid of Thread Blocks

## Thread
一个单独的线程必然属于一个`Thread Block`, 它有自己的

- thread ID
- program counter
- registers
- per-thread private memory
- inputs
- output

## Thread Block
一个`Thread Block`必然属于一个`Grid`, 它是一组并发执行的线程的集合，这些线程之间可以通过`barrier synchronization`和`per-Block shared memory`互相协同, 它也有一个`Block ID`用来索引它在`Grid`中的位置。

## Grid of Thread Blocks
一个`Grid`由一组执行相同`kernel`的`Thread Blocks`组成。可以这样说，一个`Grid`是所有执行同一个`Kernel`的线程的集合。

# CUDA Thread Map
CUDA的threads层级结构可以映射到GPU的processors的层级结构上。

- 一个GPU执行一个或多个grids(kernels)
- 一个SM(Streaming Multiprocessor)执行一个或多个Thread Blocks
- CUDA Cores和一个SM里的其它执行单元(SFU, LDST)执行多个线程

SM执行线程的单位是`warp`, 一般是32个线程一组，一个`warp`里的32个线程是真正并行执行的。

{% asset_img cuda-thread-map.png %}

## CUDA Memory
```
cudaError_t cudaMemcpy(void *dst,
                       const void *src,
                       size_t count,
                       cudaMemcpyKind kind);
```
除了最后一个参数，其它参数都顾名知意。`kind`指示内存拷贝的方向:

- cudaMemcpyHostToHost
- cudaMemcpyHostToDevice
- cudaMemcpyDeviceToHost
- cudaMemcpyDeviceToDevice

# OpenGL Compute Shader Thread Hierarchy

一个OpenGL `Compute Shader`(CS)就相当于一个cuda中的`__kernel__`函数， 这些任务最终被分发到GPU的各个物理核心上去执行。在`cs`中，单个任务叫做`work item`, 除了`work item`之外, 还有以下概念:

- subgroups

也就是`warps`或者`wavefronts`或者`Compute Units`, 它们实际上就是`threads`, `shader cores`, `cuda cores`

- local workgroups

它的大小(维度)在`cs`中通过`layout`修饰符指定

```
layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
```

`local workgroups`相当于cuda

- global workgroup (x, y, z)

下面是一个典型的`cs`样子

```
#version 430 core

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;

void main(void)
{

}

```

References:
1. [NVIDIA Fermi Architecture Whitepaper](https://www.nvidia.com/content/PDF/fermi_white_papers/NVIDIAFermiComputeArchitectureWhitepaper.pdf)
2. [NVIDIA CUDA Programming Guide](https://developer.download.nvidia.cn/compute/DevZone/docs/html/C/doc/CUDA_C_Programming_Guide.pdf)

