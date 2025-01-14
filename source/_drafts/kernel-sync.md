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

- binary semaphore (0/1)
- normal semaphore (non-negative integer)

信号量的操作叫 **Acquire** 和 **Release**, 对应的实现是 `void down(struct semaphore *)` 和 `void up(struct semaphore *)`

# References

- [Synchronization primitives in Linux kernel](https://0xax.gitbooks.io/linux-insides/content/SyncPrim/)