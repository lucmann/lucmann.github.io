---
title: POSIX Threads Synchronization - Spinlock vs. Mutex 
date: 2020-06-03 11:03:49
tags:
---

## Spinlock
自旋锁是解决多处理器共享内存使用的一种底层同步机制。当一个线程试图获取一个已经被另一个线程占有的自旋锁时，这个线程
将以循环检查自旋锁是否被释放的方式(自旋)阻塞。一个线程不应该长时间占有一个自旋锁，因为被阻塞的线程仍然消耗CPU资源
(CPU cycles)

### POSIX interfaces
- Initializing a Spinlock
```c
int pthread_spin_init(pthread_spinlock_t *lock, int pshared); 
```

`pshared`属性可以取以下值的其中之一:

PTHREAD_PROCESS_SHARED - 这个自旋锁可以被任何线程操作，即使这些线程属于不同的进程，只要申请这个自旋锁的内存是被
这些进程共享的。

PTHREAD_PROCESS_PRIVATE - 这个自旋锁只能被那些与初始化它的线程同属一个进程的线程操作。

- Acquiring a Spinlock
```c
int pthread_spin_lock(pthread_spinlock_t *lock);
```

如果被请求的自旋锁没有被其它线程占有，则请求它的线程成功获取这个锁，否则这个线程不从`pthread_spin_lock`中返回(阻塞)，
直到这个锁被释放。一个线程尝试获取被它自己占有的自旋锁是未定义的行为。

- Acquiring a Non-Blocking Spinlock
```c
int pthread_spin_trylock(pthread_spinlock_t *lock);
```

尝试获取一个自旋锁，与`pthread_spin_lock`不同的是，如果所请求的锁被另一个线程占有，不阻塞，而是立即返回失败(Non-Blocking).

- Unlocking a Spinlock
```c
int pthread_spin_unlock(pthread_spinlock_t *lock);
```

释放被锁的自旋锁。

成功返回0，否则返回以下错误码之一:

EPERM - 调用线程不持有这个锁

EINVAL - lock指向的不是一个已初始化的自旋锁对象

- Destroying a Spinlock
```c
int pthread_spin_destroy(pthread_spinlock_t *lock);
```

