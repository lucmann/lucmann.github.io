---
title: Linux Kernel Synchronization Primitives
date: 2025-01-14 11:38:43
tags: linux
categories: linux
---

# Spinlock

**spin-in-place** (busy waiting, no preemption, no sleep)

# Semaphore

信号量的实现基于一个变量，根据这个变量的取值范围可以把信号量分为

- binary semaphore (= 1, mutex)
- normal semaphore (> 1, counting semaphore)

信号量的操作叫 **Acquire** 和 **Release**, 对应的实现是 `void down(struct semaphore *)` 和 `void up(struct semaphore *)`

- down

```c
static inline int __sched ___down_common(struct semaphore *sem, long state,
								long timeout)
{
	struct semaphore_waiter waiter;

	list_add_tail(&waiter.list, &sem->wait_list);
	waiter.task = current;
	waiter.up = false;

	for (;;) {
		if (signal_pending_state(state, current))
			goto interrupted;
		if (unlikely(timeout <= 0))
			goto timed_out;
		__set_current_state(state);
		raw_spin_unlock_irq(&sem->lock);
		timeout = schedule_timeout(timeout);
		raw_spin_lock_irq(&sem->lock);
		if (waiter.up)
			return 0;
	}

 timed_out:
	list_del(&waiter.list);
	return -ETIME;

 interrupted:
	list_del(&waiter.list);
	return -EINTR;
}
```

- up

```c
static noinline void __sched __up(struct semaphore *sem)
{
	struct semaphore_waiter *waiter = list_first_entry(&sem->wait_list,
						struct semaphore_waiter, list);
	list_del(&waiter->list);
	waiter->up = true;
	wake_up_process(waiter->task);
}
```

# References

- [Synchronization primitives in Linux kernel](https://0xax.gitbooks.io/linux-insides/content/SyncPrim/)
- [https://yarchive.net/comp/linux/semaphores.html](https://yarchive.net/comp/linux/semaphores.html)
