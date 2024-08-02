---
title: 多线程中的同步
tags: [pthread]
categories: programming
---

# 同步原语

同步原语(Synchronization Primitive)的[确切定义没有一个官方的](https://www.cs.columbia.edu/~hgs/os/sync.html)。但大体来说同步原语可能有以下几种:

- semaphores
- mutex
- locks
- condition variables
- test-and-set machine instructions

<!--more-->

但这些同步原语是如何实现多线程同步的呢？

- 当我们谈论线程同步时，我们在说什么？

当我们谈论线程同步时，所谓同步，就是必须保证多个线程对同一数据的预期访问次序，也就是"临界区"的保护。之所以多个线程对同一数据的访问可能会产生不预期的结果，是因为内存(这块全局可见存储)中的数据可能会因为CPU读写顺序的差异导致其与各个CPU(核心)本地缓存中的数据不一致。所以线程同步，实质上就是需要保证如果一个CPU核心要读内存之前，另一个CPU已经将新值写入那个地址的内存，如果没有写入，就要让所有准备执行读操作的CPU核心停下来。这个过程中我们不难发现有两个关键点: CPU所执行指令(读写操作)的原子性和内存数据的可见性(本地缓存是否更新到内存)。

- 原子操作
- 内存屏障


## xshmfence
- 基于 futex 和原子操作实现
    ```c
    int
    xshmfence_await(struct xshmfence *f)
    {
        while (__sync_val_compare_and_swap(&f->v, 0, -1) != 1) {
            if (futex_wait(&f->v, -1)) { // blocking the caller process until f->v reaches to 0
                if (errno != EWOULDBLOCK)
                    return -1;
            }
        }

        return 0;
    }
    ```
- 基于 pthread 条件变量实现

# POSIX Threads 实现 - `-lpthread`
## Spinlock
自旋锁是解决多处理器共享内存使用的一种底层同步机制。当一个线程试图获取一个已经被另一个线程占有的自旋锁时，这个线程将以循环检查自旋锁是否被释放的方式(自旋)阻塞。一个线程不应该长时间占有一个自旋锁，因为被阻塞的线程仍然消耗CPU资源 (CPU cycles)

### POSIX interfaces
#### Initializing a Spinlock
```c
int pthread_spin_init(pthread_spinlock_t *lock, int pshared); 
```

`pshared`属性可以取以下值的其中之一:

PTHREAD_PROCESS_SHARED - 这个自旋锁可以被任何线程操作，即使这些线程属于不同的进程，只要申请这个自旋锁的内存是被这些进程共享的。

PTHREAD_PROCESS_PRIVATE - 这个自旋锁只能被那些与初始化它的线程同属一个进程的线程操作。

#### Acquiring a Spinlock
```c
int pthread_spin_lock(pthread_spinlock_t *lock);
```

如果被请求的自旋锁没有被其它线程占有，则请求它的线程成功获取这个锁，否则这个线程不从`pthread_spin_lock`中返回(阻塞)，直到这个锁被释放。一个线程尝试获取被它自己占有的自旋锁是未定义的行为。

#### Acquiring a Non-Blocking Spinlock
```c
int pthread_spin_trylock(pthread_spinlock_t *lock);
```

尝试获取一个自旋锁，与`pthread_spin_lock`不同的是，如果所请求的锁被另一个线程占有，不阻塞，而是立即返回失败(Non-Blocking).

#### Unlocking a Spinlock
```c
int pthread_spin_unlock(pthread_spinlock_t *lock);
```

释放被锁的自旋锁。成功返回0，否则返回以下错误码之一:

EPERM - 调用线程不持有这个锁

EINVAL - lock指向的不是一个已初始化的自旋锁对象

#### Destroying a Spinlock
```c
int pthread_spin_destroy(pthread_spinlock_t *lock);
```

# 参考
- [linux - futex 原理分析](https://www.openeuler.org/zh/blog/wangshuo/Linux_Futex_Principle_Analysis/Linux_Futex_Principle_Analysis.html)
