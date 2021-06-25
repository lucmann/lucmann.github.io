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

## `perf list`

```
perf list 'sched:*'
```

