---
title: GPU Scheduler in Linux Kernel
date: 2021-09-17 11:38:43
tags: [drm]
categories: linux
---

# 缩略词
| 缩略词                      | 解释                                      |
|:----------------------------|:------------------------------------------|
| rq                          | run queue                                 |
| SPSC                        | Single Producer Single Consumer           |

<!--more-->

- `drm_gpu_scheduler`
- `drm_sched_backend_ops`
- `drm_sched_rq`
- `drm_sched_entity`
- `drm_sched_job`
- `drm_sched_fence`

Linux内核的GPU scheduler是负责向GPU硬件提交作业的，它被编译到内核模块`gpu-sched`. 它本身是一个内核线程`kthread`, 这个线程的入口函数是`drm_sched_main`, 它的启动是在`drm_sched_init`. 


```c
/**
 * drm_sched_init - Init a gpu scheduler instance
 *
 * @sched: scheduler instance
 * @ops: backend operations for this scheduler
 * @hw_submission: number of hw submissions that can be in flight
 * @hang_limit: number of times to allow a job to hang before dropping it
 * @timeout: timeout value in jiffies for the scheduler
 * @timeout_wq: workqueue to use for timeout work. If NULL, the system_wq is
 *		used
 * @score: optional score atomic shared with other schedulers
 * @name: name used for debugging
 *
 * Return 0 on success, otherwise error code.
 */
int drm_sched_init(struct drm_gpu_scheduler *sched,
		   const struct drm_sched_backend_ops *ops,
		   unsigned hw_submission, unsigned hang_limit,
		   long timeout, struct workqueue_struct *timeout_wq,
		   atomic_t *score, const char *name)
```

# drm_sched_main

这个函数根据`dma_fence`的状态(signaled or unsignaled)反复进行睡眠和唤醒, 让下面这4个callbacks有条不紊地在CPU上执行。`drm_sched_init`中有一个`drm_sched_backend_ops`需要驱动去实现:

```c
static const struct drm_sched_backend_ops xxx_sched_ops = {
	.dependency = xxx_job_dependency,
	.run_job = xxx_job_run,
	.timedout_job = xxx_job_timedout,
	.free_job = xxx_job_free
};
```

`dma_fence`就像一个“令牌”一样，在这4个函数之间流转，gpu scheduler不单单用一个`dma_fence`, 它使用了一组`dma_fence`(2个， scheduled和finished), 这组`dma_fence`被封装在`drm_sched_fence`

```c
/**
 * struct drm_sched_fence - fences corresponding to the scheduling of a job.
 */
struct drm_sched_fence {
        /**
         * @scheduled: this fence is what will be signaled by the scheduler
         * when the job is scheduled.
         */
	struct dma_fence		scheduled;

        /**
         * @finished: this fence is what will be signaled by the scheduler
         * when the job is completed.
         *
         * When setting up an out fence for the job, you should use
         * this, since it's available immediately upon
         * drm_sched_job_init(), and the fence returned by the driver
         * from run_job() won't be created until the dependencies have
         * resolved.
         */
	struct dma_fence		finished;

        /**
         * @parent: the fence returned by &drm_sched_backend_ops.run_job
         * when scheduling the job on hardware. We signal the
         * &drm_sched_fence.finished fence once parent is signalled.
         */
	struct dma_fence		*parent;
        /**
         * @sched: the scheduler instance to which the job having this struct
         * belongs to.
         */
	struct drm_gpu_scheduler	*sched;
        /**
         * @lock: the lock used by the scheduled and the finished fences.
         */
	spinlock_t			lock;
        /**
         * @owner: job owner for debugging
         */
	void				*owner;
};
```

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
