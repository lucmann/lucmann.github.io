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

# References

- [when-to-use-kernel-threads-vs-workqueues-in-the-linux-kernel](https://stackoverflow.com/questions/2147299/when-to-use-kernel-threads-vs-workqueues-in-the-linux-kernel)
- [Making workqueues non-reentrant](https://lwn.net/Articles/511421/)
