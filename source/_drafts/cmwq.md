---
title: Concurrency Managed Workqueue
date: Wed Sep 10 13:55:24 CST 2025
tags: [kernel]
categories: linux
---



<!--more-->

# workqueue vs ordered_workqueue

# work_struct vs delayed_work vs rcu_work

整个 workqueue 的核心目的是实现让某个函数在一个特定的时间点或事件后开始执行，这个待执行的函数被封装成 `work_struct`, 它还有两个变种 `delayed_work` 和 `rcu_work`。这个函数的原型是没有返回值，只接受一个 `struct work_struct *` 参数的函数。

```c
typedef void (*work_func_t)(struct work_struct *work);
```

# shared workqueue vs non-shared workqueue

shared workqueue 和 non-shared workqueue 的主要区别点是看这个workqueue 里的work item 的执行者，也就是 **kworker 是共享的还是专属的**。如果一个 workqueue 的执行者 kworker 只执行这个 workqueue 上的 work item, 那这个 workqueue 就是 **non-shared**

# 参考

- [工作队列](https://docs.kernel.org/translations/zh_CN/core-api/workqueue.html)
- [Concurrency Managed Workqueue之（一）：workqueue的基本概念](http://www.wowotech.net/irq_subsystem/workqueue.html)
- [The end of tasklets](https://lwn.net/Articles/960041/)
