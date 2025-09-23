---
title: perf 和火焰图
date: 2021-06-25 07:57:06
tags: perf
categories: linux
---

![RedEclipse on R7340](/images/perf/radeon.svg)

<!--more-->

# 原理

perf 使用的是采样(Sampling)技术

# 环境 & 版本

- Linux 5.10.16.3-microsoft-standard-WSL2 x86_64 x86_64
- Ubuntu 20.04.2 LTS
- perf version 5.10.16.3

NOTE: 如果没有特别说明，以下`perf`命令都在`root`权限下执行

# 查询 & 罗列

列出Virtual Memory相关的`static tracepoint`

- `perf list 'vmscan:*'`
![](/images/perf/perf_list-vmscan.png)

- `perf list 'kmem:*'
k

- `perf stat -a sleep`
![](/images/perf/perf_stat.png)

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

- `perf top -e cycles`

![perf top](/images/perf/perf_top-cycles.png)

- `perf record -F 999 -p PID -g`
    - 默认会在当前目录下生成 perf.data, 后面如果再执行之前的 perf.data 会自动重命名 perf.data.old

- `perf script | /path/to/FlameGraph/stackcollapse-perf.pl | /path/to/FlameGraph/framegraph.pl > result.svg`
    - 生成火焰图需要这个脚本工具 [FlameGraph](https://github.com/brendangregg/FlameGraph)，而且如果想看到详细的调用栈需要编译跟踪对象为 **Debug** 版本
    - 最近有一个 Rust 写的 [flamegraph-rs/flamegraph](https://github.com/flamegraph-rs/flamegraph) 项目，用 `flamegraph` 二进制可执行程序代替了原来的 Perl 脚本，而且把原来分别执行 `perf record` 和 `perf script` 这两步合成一步，即可生成火焰图

        ```shell
        flamegraph -- vkgears
        ```
        ![flamegraph-rs/flamegraph](/images/perf/flamegraph.svg)

# References

- [FlameGraph](https://github.com/brendangregg/FlameGraph)
