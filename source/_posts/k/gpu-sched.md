---
title: GPU Scheduler in Linux Kernel
date: 2021-09-17 11:38:43
tags: drm
categories: kernel
---

Linux内核的GPU scheduler是负责向GPU硬件提交作业的，它被编译到内核模块`gpu-sched`. 它本身是一个内核线程`kthread`, 这个线程的入口函数是`drm_sched_main`, 它的启动是在`drm_sched_init`. 

<!--more-->

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
