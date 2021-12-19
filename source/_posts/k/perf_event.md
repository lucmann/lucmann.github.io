---
title: Linux perf初探
date: 2021-06-25 07:57:06
tags: [perf]
categories: [bin]
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

# 动态跟踪

## `perf top`

{% asset_img perf_top-cycles.png "perf top -e cycles" %}
