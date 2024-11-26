---
title: 多线程中的同步
date: 2024-08-08 16:24:00
tags: [C/C++]
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

# 无锁队列

无锁队列的实现依靠的是CPU提供的原子操作指令(atomic) 和比较和交换指令 (cas). 其中cas 指令还有一种 double-width cas (dwcas), 就是能在 64位机器上原子地进行 128位(dword)值的cas (在 aarch64 cas 和 dwcas 是同一条指令)，CPU是否支持dwcas, 影响实现无锁队列时的一个策略，即队列容量是否固定。

## ABA 问题

无锁数据结构通常采用的是一种简单的“重试”策略，它的思想是线程0 获取一个ptr, 访问它指向的数据，然后尝试更新这个 ptr的值(通过 cas), 如果发现这个 ptr 已经变了(cas 比较测试失败), 那就重新获取有新值的ptr, 再访问新ptr 指向的数据。

ABA 问题出现的场景是，在线程0，前后两次读到ptr 的值的中间，有可能线程1，已经修改过 ptr 的值由A 到 B，而后又在线程0 cas 操作前，线程1又**恰巧**在地址A申请到一个元素遂将ptr 的值又改回A, 结果就是线程1对ptr 的修改(A->B->A)对线程0不可见，而实际上A指向的内容可能已经发生变化，对线程0来说可能导致未预期的结果。

这种场景的ABA问题可以通过将原来单独的一个ptr (64位系统上sizeof(ptr) = 8), 替换成一对(ptr, counter)来解决。每次ptr 被修改，counter就+1, 这样即使最后ptr 是相等的(A->B->A), 但counter 也不相等，这样只要同时对(ptr, counter) 做 cas 比较就可以准确判定ptr 是否被修改过(如果修改过，即使现在的值和之前的值相等，仍然要重新获取这个值，再次访问它指向的内容)。但现在要比较的数据由原来的8字节变成16字节，所以就需要 dwcas 指令。

由上面ABA问题的分析可以看到，之所以对线程0来说，A指向的内容可能会发生变化，原因是中间存在申请内存的操作。所以只要保证在整个队列操作中，不会动态申请元素(没有了线程1恰巧又重新分配的A这个地址的可能)，这种场景的ABA问题也就不存在了。这就是为什么 CPU 如果不支持 dwcas, 则无锁队列的容量必须是固定大小的。

## boost::lockfree::queue 源码分析

### 算法

- 出队
```c
pop(Q: pointer to queue, ret: pointer to data type): boolean
D1:    loop                             # Keep trying until pop is done
D2:      head = Q->Head                 # Read Head
D3:      tail = Q->Tail                 # Read Tail
D4:      next = head->next              # Read Head.ptr->next
D5:      if head == Q->Head             # Are head, tail, and next consistent?
D6:        if head.ptr == tail.ptr      # Is queue empty or Tail falling bebind?
D7:          if next.ptr == NULL        # Is queue empty?
D8:            return FALSE             # Queue is empty, couldn't pop
D9:          endif
             # Tail is falling behind. Try to advance it
D10:         CAS(&Q->Tail, tail, <next.ptr, tail.count+1>)
D11:       else                         # No need to deal with Tail
             # Read value before CAS, otherwise another pop might free the next node
D12:         *ret = next.ptr->value
             # Try to swing Head to the next node
D13:         if CAS(&Q->Head, head, <next.ptr, head.count+1>)
D14:           break                    # Pop is done, Exit loop
D15:         endif
D16:       endif
D17:     endif
D18:   endloop
D19:   free(head.ptr)                   # It is safe now to free the old dummy node
D20:   return TRUE                      # Queue was not empty, pop succeeded 
```

### C++实现

```c
private:
    std::atomic< tagged_node_handle > head_;
    std::atomic< tagged_node_handle > tail_;
```

- [std::memory_order](https://en.cppreference.com/w/cpp/atomic/memory_order)
    ```c
    typedef enum memory_order {
        memory_order_relaxed,
        memory_order_consume,
        memory_order_acquire,
        memory_order_release,
        memory_order_acq_rel,
        memory_order_seq_cst, // Sequentially-consistent ordering
    } memory_order;
    ```

    这个memory_order的顺序，由上到下对原子变量的操作和读写顺序的保证应该是越来越严格的(没有读写顺序保证->单个原子变量读写顺序保证->全局读写顺序一致)。

    - `memory_order_relaxed`只保证原子操作，不保证指令顺序。
    - `memory_order_acquire`
        - 用于 `atomic<T>::load()`
        - 对于使用memory_order_acquire的指令，该指令后面的所有读写操作**不能重排在该指令之前**
        - 当前线程执行的memory_order_acquire指令能够保证读到其他线程memory_order_release指令之前的所有内存写入操作
    - `memory_order_release`
        - 用于 `atomic<T>::store()`
        - 对于使用memory_order_release的指令，该指令之前的所有读写操作**不能重排在该指令之后**
        - 当前线程memory_order_release指令之前的所有内存写操作对于其他线程的memory_order_acquire指令都可见。
    - `memory_order_acq_rel`
        - 用于 `atomic<T>::fetch_add()`, `atomic<T>::compare_exchange_weak()` 这类 read-modify-write 操作。如果用在普通的 `atomic<T>::load()`，效果与用memory_order_acquire 无异。
        - 把 memory_order_acquire 和 memory_order_release 结合起来，它可以保证单个原子变量的读写顺序，下面的例子就是不适用 memory_order_acq_rel 的

        ```c
        #include <atomic>
        #include <cassert>
        #include <thread>

        std::atomic<bool> x = {false};
        std::atomic<bool> y = {false};
        std::atomic<int> z = {0};

        void write_x()
        {
            x.store(true, std::memory_order_acq_rel);
        }

        void write_y()
        {
            y.store(true, std::memory_order_acq_rel);
        }

        void read_x_then_y()
        {
            while (!x.load(std::memory_order_acq_rel))
                ;
            if (y.load(std::memory_order_acq_rel))
                ++z;
        }

        void read_y_then_x()
        {
            while (!y.load(std::memory_order_acq_rel))
                ;
            if (x.load(std::memory_order_acq_rel))
                ++z;
        }

        int main()
        {
            std::thread a(write_x);
            std::thread b(write_y);
            std::thread c(read_x_then_y);
            std::thread d(read_y_then_x);
            a.join(); b.join(); c.join(); d.join();
            assert(z.load() != 0); // will never happen
        }

        ```

        上面的例子修改自 [cppreference.com/atomic/memory_order Sequentially-consistent ordering](https://en.cppreference.com/w/cpp/atomic/memory_order) 部分。理论上这个例子应该能够说明 memory_order_acq_rel 与 memory_order_seq_cst 的区别，当把原例中的 seq_cst 换成 acq_rel，应该是有可能触发 `z.load() == 0`的情况的(至少在 aarch64 上，因为x86的存储器模型本身就是按序一致性模型)。但实际上却没有触发，其原因在 [stackoverflow 上有解释](https://stackoverflow.com/questions/67397460/does-stlrb-provide-sequential-consistency-on-arm64)。 通过在 [godbolt.org](https://godbolt.org/z/8KbxMEY6s) 上查看这个程序的汇编代码，确实 `y.load(std::memory_order_acq_rel)`和`y.store(true, std::memory_order_acq_rel)`是被分别翻译成了 `ldarb` 和 `stlrb`。

    - `memory_order_seq_cst`
        - 提供最严格的全局读写顺序一致性保证，上面的例子就只能使用 memory_order_seq_cst
        - 对于使用memory_order_seq_cst 的 `atomic<T>::store()` 指令来说，它强制flush 每个CPU核心的store buffer, 所以这个store 操作后面的 load (读操作)将被延迟直到store 操作完成所有 store buffer 的刷新(即对全局可见)。


- `std::atomic<T>::load(std::memory_order order = std::memory_order_seq_cst)`

- [`std::atomic<T>::compare_exchange_*()`](https://en.cppreference.com/w/cpp/atomic/atomic/compare_exchange)

   C++ std::atomic<T> 的 cas 有8个不同的声明，主要区分在 weak/strong, 参数和是否有 volatile. 而boost::lockfree::queue 里使用的是下面两个版本:
    - `std::atomic<T>::compare_exchange_strong(T& expected, T desired, std::memory_order    order = std::memory_order_seq_cst)`
    - `std::atomic<T>::compare_exchange_weak(T& expected, T desired, std::memory_order order = std::memory_order_seq_cst)`

    - 为什么 `expected` 参数是一个引用类型，而`desired`参数是一个值传递？

# 参考

- [Simple, Fast, and Practical Non-Blocking and Blocking Concurrent Queue Algorithms](https://www.cs.rochester.edu/u/scott/papers/1996_PODC_queues.pdf)
- [boost::lockfree::queue](https://github.com/boostorg/lockfree/blob/develop/include/boost/lockfree/detail/tagged_ptr_dcas.hpp#L120)
- [inline_asm_lockfree_queue](https://github.com/fangcun010/inline_asm_lockfree_queue)
- [Implementing generic double-word compare and swap for x86/64](https://blog.lse.epita.fr//2013/02/27/implementing-generic-double-word-compare-and-swap.html)
- [linux - futex 原理分析](https://www.openeuler.org/zh/blog/wangshuo/Linux_Futex_Principle_Analysis/Linux_Futex_Principle_Analysis.html)
