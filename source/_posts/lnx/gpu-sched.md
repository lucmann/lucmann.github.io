---
title: drm_gpu_scheduler
date: 2025-02-27 07:38:43
tags: [DRM/KMS]
categories: linux
---

```mermaid
flowchart TD
  subgraph GPU
    slot-0[HW Run Queue]
    slot-1[HW Run Queue]
    slot-2[HW Run Queue]
  end

  sched0[drm_gpu_scheduler]
  sched1[drm_gpu_scheduler]
  sched2[drm_gpu_scheduler]

  runq00[drm_sched_rq<br>KERNEL]
  runq01[drm_sched_rq<br>HIGH]
  runq02[drm_sched_rq<br>NORMAL]
  runq03[drm_sched_rq<br>LOW]

  runq10[drm_sched_rq<br>KERNEL]
  runq11[drm_sched_rq<br>HIGH]

  runq20[drm_sched_rq<br>KERNEL]
  runq21[drm_sched_rq<br>HIGH]
  runq22[drm_sched_rq<br>NORMAL]

  entity00@{shape: docs, label: "drm_sched_entity<br>job chain"}
  entity01@{shape: docs, label: "drm_sched_entity<br>job chain"}
  entity02@{shape: docs, label: "drm_sched_entity<br>job chain"}
  entity03@{shape: docs, label: "drm_sched_entity<br>job chain"}

  entity10@{shape: docs, label: "drm_sched_entity<br>job chain"}
  entity11@{shape: docs, label: "drm_sched_entity<br>job chain"}
  entity12@{shape: docs, label: "drm_sched_entity<br>job chain"}


  entity20@{shape: docs, label: "drm_sched_entity<br>job chain"}
  entity21@{shape: docs, label: "drm_sched_entity<br>job chain"}
  entity22@{shape: docs, label: "drm_sched_entity<br>job chain"}

  entity00 --> entity01 --> entity02 --> entity03 --> runq00
  entity10 --> entity11 --> entity12 --> runq10
  entity20 --> entity21 --> entity22 --> runq02

  runq00 --> sched0
  runq01 --> sched0
  runq02 --> sched0
  runq03 --> sched0

  runq10 --> sched1
  runq11 --> sched1

  runq20 --> sched2
  runq21 --> sched2
  runq22 --> sched2

  sched0 --> slot-0
  sched1 --> slot-1
  sched2 --> slot-2
```

<!--more-->

Notes:

- 每个 hw run queue 对应一个 [drm_gpu_scheduler](https://elixir.bootlin.com/linux/v6.13.4/source/drivers/gpu/drm/panfrost/panfrost_job.c#L36)
- 每个 scheduler 对应多个不同优先级的 scheduler run queue (sw run queue)
- 每个 scheduler run queue 是一个等待被调试的 entity 队列
- 每个 entity 由包含若干个 gpu job 的链表组成

# 数据结构

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

# 初始化 Scheduler 实例

## v6.8

```c
int drm_sched_init(
  struct drm_gpu_scheduler *sched,

  // 需要由驱动实现的一组回调函数, 有
  // prepare_job(), run_job(), timedout_job(), free_job()
  const struct drm_sched_backend_ops *ops,

  // 一个 workqueue(6.8 之前是 kthread) 用来向 hw run queue 提交 job
  // 如果驱动没有提供，默认是一个 ordered workqueue
  struct workqueue_struct *submit_wq, 

  // 这个 sched 下的 drm_sched_rq 的个数，最多 4 个，分别对应
  // LOW, NORMAL, HIGH, KERNEL 4 个优先级
  u32 num_rqs,

  // 用来 job flow control, sched 最多能提交多少个 job(chain) 给 hw,
  // 防止 ring buffer overflow
  // 这里的每个job 的概念因不同 GPU 而异
  u32 credit_limit,

  // 允许一个 job 在被丢弃前 hang 多少次
  unsigned int hang_limit,

  // job 超时时长 (jiffies)
  long timeout,

  // 另外一个 workqueue 用来执行超时之后的逻辑。驱动可以不指定，
  // 默认是 system_wq (让这个 workqueue 执行的任务不要太长)
  struct workqueue_struct *timeout_wq,

  atomic_t *score, // 与其它 sched 共享的原子整型的 score
  const char *name, // 用来调试
  struct device *dev // 所属 struct device
);
```

## v5.4

```c
int drm_sched_init(
  struct drm_gpu_scheduler *sched,
  // 需要由驱动实现的一组回调函数，有
  // dependency(), run_job(), timedout_job(), free_job()
  const struct drm_sched_backend_ops *ops,
  unsigned hw_submission, // 允许有多少个 hw 提交同时存在
  unsigned hang_limit, // 允许一个 job 在被丢弃前 hang 多少次
  long timeout, // job 超时时长 (jiffies)
  const char *name // 用来调试
);
```

Note:

- 5.4 没有让驱动提供一个 timeout_wq, 而是固定使用 delayable workqueue 去执行 [drm_sched_job_timedout()](https://elixir.bootlin.com/linux/v5.19.17/source/drivers/gpu/drm/scheduler/sched_main.c#L1016)
- 参数中的 `timeout` 是以 jiffies 计算的，如果设置成 `MAX_SCHEDULE_TIMEOUT`， 表示由驱动自己处理超时

# Scheduler 如何工作

Job 提交一般由用户驱动通过 **IOCTL_SUBMIT** 命令触发，将 job 下发给 hw, 所谓下发就是将 64 位的 job(chain) 的起始地址写入 MMIO 寄存器或 ringbuffer, 然后再触发 doorbell, hw 就开始执行

```mermaid
sequenceDiagram
  participant UMD
  participant KMD
  participant Kworker

  KMD ->> KMD : drm_sched_init()
  note right of KMD : 创建一个 work item
  KMD ->> KMD : INIT_WORK(&sched->work_run_job,<br>drm_sched_run_job_work)
  UMD ->> KMD : ioctl(SUBMIT)
  KMD ->> KMD : drm_sched_job_arm()
  KMD ->> KMD : drm_sched_entity_push_job()
  KMD ->> KMD : drm_sched_waitup()
  note right of KMD : 将 work item 扔到 submit_wq 上去
  KMD ->> KMD : drm_sched_run_job_queue()
  note right of KMD : 一旦 workqueue 上有了 work,<br>空闲的 kworker 就会执行 work item
  KMD ->> Kworker : wakeup
  Kworker ->> Kworker : drm_sched_run_job_work()
  Kworker ->> Kworker : drm_sched_entity_pop_job()
  Kworker ->> Kworker : sched->ops->run_job()
  note left of Kworker : writel(jc, dev->iomem + reg)<br>Go!
```

Note:
- `drm_sched_free_job_work()` 和 `drm_sched_run_job_work()` 是分开的两个 work item, 但它俩都会被扔到同一个 workqueue 上 `submit_wq` (workqueue 的实现很有意思，异步执行的单位是函数，而且函数们是被放在一个队列里，一个个执行) 

# 参考资料

- [linux DRM GPU scheduler 笔记](https://www.cnblogs.com/yaongtime/p/14305463.html)
- [drm/panfrost: Add initial panfrost driver](https://patchwork.freedesktop.org/patch/297644/)
- [drivers/gpu 下的 `drm_sched_backend_ops`](https://pastebin.com/MssJk6Ky)
- [PowerVR Rogue Command Stream format](https://gitlab.freedesktop.org/mesa/mesa/-/blob/f8d2b42ae65c2f16f36a43e0ae39d288431e4263/src/imagination/csbgen/rogue_kmd_stream.xml)
- [Linux kernel workqueue 机制分析](https://www.cnblogs.com/jimbo17/p/8885814.html)

