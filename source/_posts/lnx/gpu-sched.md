---
title: drm_gpu_scheduler
date: 2021-09-17 11:38:43
tags: [DRM]
categories: linux
---


Linux DRM 子系统的 `drm_gpu_scheduler` 负责提交和调度 GPU job，以一个单独的内核模块(`gpu-sched`) 的形式存在。

<!--more-->

#  数据结构

- `drm_gpu_scheduler`

调度器实例 (instance)，运行时实际上是一个内核线程 (kthread), 这个线程启动是在 `drm_sched_init()`。

实际上，自从内核 v6.8-rc1 [a6149f039369 ("drm/sched: Convert drm scheduler to use a work queue rather than kthread")](https://lore.kernel.org/all/20231031032439.1558703-3-matthew.brost@intel.com/) `drm_gpu_scheduler` 的实现已经从 kthread 变成 work queue 了。 这个修改与 Intel Gen9+ 引入的 microcontrollers (μC) 之一 [GuC](https://igor-blue.github.io/2021/02/10/graphics-part1.html#the-guc) 有关。

- `drm_sched_rq`

若干个 `drm_sched_entity` (list) 的封装。一个 scheduler 实例最多可以有 `DRM_SCHED_PRIORITY_COUNT` 个 `drm_sched_rq`。调度器调度的其实就是一个个 entity。 这么多个 entity 按什么顺序提交给 GPU 由具体的 **调度策略 (Scheduling Policy)** 决定，而**调度优先级 (Scheduling Priority)** 由 `drm_sched_rq` 实现，有多少个优先级，一个 `drm_gpu_scheduler` 里就有多少个 `drm_sched_rq`，每个优先级对应一个 `drm_sched_rq`。 

- `drm_sched_entity`

若干个 `drm_sched_job` (list) 的封装

- `drm_sched_job`

被 entity 运行的一个 job, 一个 job 总是属于某一个 entity


# drm_scheduler 的一些问题

## 如果在 `run_job()` path 存在内存申请，就有可能导致内核死锁

我们试着从 [dri-devel IRC 频道](https://oftc.irclog.whitequark.org/dri-devel/2023-03-10#31964051;) 上的一段讨论来理解这个问题。

> 16:51 <gfxstrand> The dma_fence design itself is fine. It's designed that way for very good reasons. There are problems we need to solve but they're more around how things have become tangled up inside drm than they are about dma_fence.
> 16:54 <bbrezillon> DemiMarie: if you have a way to swapout some memory accessed by in-flight jobs, you might be able to unblock the situation, but I'm sure this 'no allocation in the scheduler path' rule is here to address the problem where a job takes too long to finish and the shrinker decides to reclaim memory anyway.
> 16:56 <bbrezillon> I think the problem is that drm_sched exposes a fence to the outside world, and it needs a guarantee that this fence will be signaled, otherwise other parties (the shrinker) might wait for an event that's never going to happen
> 16:56 <gfxstrand> Yup
> 16:57 <bbrezillon> that comes from the fact it's not the driver fence that's exposed to the outside world, but an intermediate object, which is indirectly signaled by the driver fence, that's created later on when the scheduler calls ->run_job()
> 16:58 <gfxstrand> Once a fence has been exposed, even internally within the kernel, it MUST signal in finite time.
> 16:59 <gfxstrand> If you allocate memory, that could kick off reclaim which can then have to wait on the GPU and you're stuck.
> 17:00 <bbrezillon> so the issue most drivers have, is that they allocate this driver fence in the ->run_job() path with GFP_KERNEL (waitable allocation), which might kick the GPU driver shrinker, which in turn will wait on the fence exposed by the drm_sched, which will never be signaled because the driver is waiting for memory to allocate its driver fence :-)
