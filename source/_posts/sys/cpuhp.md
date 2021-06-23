---
title: CPU Hotplug in Linux
date: 2021-05-21 16:09:13
tags: [linux]
categories: sys
---

# A Puzzle
如果先调用`sched_setaffinity`将线程绑定到CPU1上，再将CPU1逻辑关闭(offline)，会发生什么？在Linux系统中，要回答这个问题，先要搞清楚Linux下的3个机制:

- CPU hotplug
- cpuset
- scheduling affinity


# [CPU hotplug](https://www.kernel.org/doc/html/latest/core-api/cpu_hotplug.html)
CPU hotplug的意思是, Linux允许**Logically** `Shutdown`和`Bring Up` CPU, 用户可以通过`sysfs`接口操作

{% codeblock "shutdown" %}
echo 0 > /sys/devices/system/cpu/cpu1/online
{% endcodeblock %}

{% codeblock "bring up" %}
echo 1 > /sys/devices/system/cpu/cpu1/online
{% endcodeblock %}

## What if the cpu is shutdown
`/proc/interrupts`, `/proc/cpuinfo`, `top`都将看不到它

## What happened if the cpu is shutdown
我只列出跟上面的疑问有关系的:

- All processes are migrated away from this outgoing CPU to new CPUs. The new CPU is chosen from each process' **current cpuset**, which may be a subset of all online CPUs.

当关闭CPU1后，原来运行在CPU1上的进程将被移到新的CPU们上调度，具体哪个CPU是从这个进程当前的**cpuset**中选择，所以疑问转换成了“`sched_setaffinity`对进程当前的**cpuset**做了什么？”

# [cpuset](https://www.kernel.org/doc/html/latest/admin-guide/cgroup-v1/cpusets.html)



