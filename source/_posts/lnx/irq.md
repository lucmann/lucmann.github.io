---
title: 中断上下文能睡眠吗?
date: 2025-04-01 11:38:10
tags: [kernel]
categories: linux
---

# 没有 task_struct 的 ISR 

众所周知，中断上下文不能睡眠，但是这句话细说起来，还是有很多值得深究的地方。所谓中断上下文，就是中断触发后，执行中断服务例程 (Interrupt Service Routine/Interrupt Handler) 的资源，这些资源有哪些呢？少得可怜，除了执行 ISR 代码的 CPU 核心外，中断上下文甚至都没有**自己的栈**来保存临时变量，所有的资源都是抢占中断触发时那个进程的，显然不能让这种“**抢占**”持续太久，所以 Linux 将 ISR 分成了 **top half** 和 **bottom half** 两部分，也就是**硬件中断上下文**和**软件中断上下文**， 所以严格来说是，在 Linux 内核中，**硬件中断上下文不能睡眠**。

<!--more-->

# 中断的触发
- 边沿触发 edge-triggered

    电平变化时产生一个短脉冲信号，并将这个信号保存在一个锁存器(latch), 电平由高到低时触发叫 falling edge triggered, 电平由低到高时触发叫 raising edge triggered
- 电平触发 level-triggered

    电平高或低时一直触发直到中断处理函数清除中断位
- 消息信号中断 Message Signal Interrupt

    通过写入特定内存地址来触发中断， 类似 AMDGPU 的 Doorbell 就是通过 CPU 将 wptr 写入一个特定 MMIO 寄存器触发 GPU 开始读取 Ring Buffer 的命令

# 参考

- [Concurrency Managed Workqueue之（一）：workqueue的基本概念](http://www.wowotech.net/irq_subsystem/workqueue.html)
