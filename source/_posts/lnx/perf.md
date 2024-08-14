---
title: perf 和火焰图
date: 2021-06-25 07:57:06
tags: perf
categories: linux
---

# 环境 & 版本

- Linux 5.10.16.3-microsoft-standard-WSL2 x86_64 x86_64
- Ubuntu 20.04.2 LTS
- perf version 5.10.16.3

NOTE: 如果没有特别说明，以下`perf`命令都在`root`权限下执行

<!--more-->

# 查询 & 罗列

## 列出Virtual Memory相关的`static tracepoint`

### vmscan
{% asset_img perf_list-vmscan.png "perf list 'vmscan:*'" %}

### kmem
{% asset_img perf_list-kmem.png "perf list 'kmem:*'" %}

## `perf stat`

```
perf stat -a sleep
```

{% asset_img perf_stat.png %}

# tracepoint 使能

通过 debugfs 中的相应文件使能 tracepoint

```
root@sie-luc:~# find /sys/kernel/debug/tracing/events/ -type d| grep -E 'dma|gpu'
/sys/kernel/debug/tracing/events/dma_fence
/sys/kernel/debug/tracing/events/dma_fence/dma_fence_emit
/sys/kernel/debug/tracing/events/dma_fence/dma_fence_init
/sys/kernel/debug/tracing/events/dma_fence/dma_fence_destroy
/sys/kernel/debug/tracing/events/dma_fence/dma_fence_enable_signal
/sys/kernel/debug/tracing/events/dma_fence/dma_fence_signaled
/sys/kernel/debug/tracing/events/dma_fence/dma_fence_wait_start
/sys/kernel/debug/tracing/events/dma_fence/dma_fence_wait_end
root@sie-luc:~# cat /sys/kernel/debug/tracing/events/dma_fence/dma_fence_enable_signal/
cat: /sys/kernel/debug/tracing/events/dma_fence/dma_fence_enable_signal/: Is a directory
root@sie-luc:~# cat /sys/kernel/debug/tracing/events/dma_fence/dma_fence_enable_signal/
enable   filter   format   id       trigger
root@sie-luc:~# cat /sys/kernel/debug/tracing/events/dma_fence/dma_fence_enable_signal/enable
0
```

# 常用命令

- `perf top`

{% asset_img perf_top-cycles.png "perf top -e cycles" %}

- `perf record -F 999 -p PID -g`
    上面输出的结果默认是 perf.data, 如果要生成火焰图还要进行以下几步:
    - `perf script | /path/to/FlameGraph/stackcollapse-perf.pl | /path/to/FlameGraph/framegraph.pl > result.svg`

# [FlameGraph](https://github.com/brendangregg/FlameGraph)
