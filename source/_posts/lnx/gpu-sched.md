---
title: drm_gpu_scheduler
date: 2021-09-17 11:38:43
tags: [DRM/KMS]
categories: linux
---

# Task State

```mermaid
stateDiagram-v2
    R: running
    S: sleeping
    D: disk sleep
    T: stopped
    t: tracing stop
    X: dead
    Z: zombie
    P: parked
    I: idle

    R --> S: schedule_timeout()
    R --> D: Wait for Disk I/O
    R --> T: SIGTSTP
    R --> t: gdb/strace
    S --> R: wake_up_process()
    D --> R: I/O Completed
    T --> R: SIGCONT
    T --> t: gdb/strace
    T --> Z: SIGKILL But Sth Wrong with Its Parent
    R --> Z: Exit But Sth Wrong with Its Parent
    t --> T: Quit gdb
```

<!--more-->

Notes:
- `disk sleep` 也就是 **uninterruptible sleep** 状态
- `zombie` 状态的进程在 `ps` 中被标记为 `<defunct>`
- 正常的空闲用户进程一般是 `sleeping` 状态，空闲的 kthread 是 `idle` 状态

#  数据结构

Linux DRM 子系统的 `drm_gpu_scheduler` 负责提交和调度 GPU job，以一个单独的内核模块(`gpu-sched`) 的形式存在。

- `drm_gpu_scheduler`

调度器实例 (instance)，运行时实际上是一个内核线程 (kthread), 这个线程启动是在 `drm_sched_init()`。

实际上，自从内核 v6.8-rc1 [a6149f039369 ("drm/sched: Convert drm scheduler to use a work queue rather than kthread")](https://lore.kernel.org/all/20231031032439.1558703-3-matthew.brost@intel.com/) `drm_gpu_scheduler` 的实现已经从 kthread 变成 work queue 了。 这个修改与 Intel Gen9+ 引入的 microcontrollers (μC) 之一 [GuC](https://igor-blue.github.io/2021/02/10/graphics-part1.html#the-guc) 有关。

- `drm_sched_rq`

若干个 `drm_sched_entity` (list) 的封装。一个 scheduler 实例最多可以有 `DRM_SCHED_PRIORITY_COUNT` 个 `drm_sched_rq`。调度器调度的其实就是一个个 entity。 这么多个 entity 按什么顺序提交给 GPU 由具体的 **调度策略 (Scheduling Policy)** 决定，而**调度优先级 (Scheduling Priority)** 由 `drm_sched_rq` 实现，有多少个优先级，一个 `drm_gpu_scheduler` 里就有多少个 `drm_sched_rq`，每个优先级对应一个 `drm_sched_rq`。 

- `drm_sched_entity`

若干个 `drm_sched_job` (list) 的封装

- `drm_sched_job`

被 entity 运行的一个 job, 一个 job 总是属于某一个 entity

# 内核中和调度相关的 APIs

- `signed long __sched shedule_timeout_interruptible(signed long timeout);`
    - 调用者 task 开始*睡眠直到超时*

- `wait_event_timeout(wq_head, condition, timeout)`
    - `wq_head`: 正在等待的一个队列 *waitqueue*, 每次 *wq_head* 被唤醒， *condition* 都会被检查一次 

# 其它

- `static inline int signal_pending(struct task_struct *)`
    - 检查当前 task 是否有信号处理，返回*非 0* 表示有信号需要处理

# References

- [when-to-use-kernel-threads-vs-workqueues-in-the-linux-kernel](https://stackoverflow.com/questions/2147299/when-to-use-kernel-threads-vs-workqueues-in-the-linux-kernel)
- [Making workqueues non-reentrant](https://lwn.net/Articles/511421/)
