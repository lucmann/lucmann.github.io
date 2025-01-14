---
title: strace - System Call Tracing
date: 2025-01-14 07:57:06
tags: tools
categories: linux
---


```strace -c -w -o glmark2.strace glmark2
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ------------------
 55.82   14.421660          55    258416           ioctl
 16.68    4.309981          18    238578    205238 recvmsg
 14.62    3.777039          17    210879        42 futex
  5.06    1.306534          16     77652           getpid
  2.73    0.705886          21     33276           writev
  2.51    0.648318          17     36385           close
  2.40    0.620114          18     33335           ppoll
  0.04    0.011076          24       452           read
  0.04    0.009150          32       281        47 openat
  0.02    0.005639          30       187           mmap
  0.01    0.003267          28       113           munmap
  0.01    0.003240          21       152       120 readlinkat
  0.01    0.002085          26        80           mprotect
  0.01    0.002033          19       104           newfstatat
  0.01    0.001976          26        75           brk
  0.01    0.001364          71        19           recvfrom
  0.00    0.001138          54        21           clone3
  0.00    0.000778          18        43           rt_sigprocmask
  0.00    0.000679          27        25           write
  0.00    0.000645          29        22           getdents64
  0.00    0.000493         492         1           execve
  0.00    0.000323          17        18           lseek
  0.00    0.000249          20        12           getrusage
  0.00    0.000232          17        13           getrandom
  0.00    0.000163          27         6           memfd_create
  0.00    0.000150          24         6           sendmsg
  0.00    0.000136          22         6           ftruncate
  0.00    0.000131          16         8           fcntl
  0.00    0.000054          18         3           geteuid
  0.00    0.000054          18         3           getuid
  0.00    0.000047          23         2         1 faccessat
  0.00    0.000037          18         2           uname
  0.00    0.000033          32         1           socket
  0.00    0.000031          30         1           shutdown
  0.00    0.000028          27         1           connect
  0.00    0.000018          17         1           getpeername
  0.00    0.000018          17         1           sched_getaffinity
  0.00    0.000017          17         1           rt_sigaction
  0.00    0.000017          16         1           prlimit64
  0.00    0.000017          16         1           set_tid_address
  0.00    0.000016          16         1           rseq
  0.00    0.000016          16         1           set_robust_list
------ ----------- ----------- --------- --------- ------------------
100.00   25.834885          29    890185    205448 total
```

<!--more-->

[strace](https://github.com/strace/strace) 主要是用来跟踪分析 Linux 系统调用的，因为系统调用是用户程序与内核交互的唯一途径，所以 strace 对于调试像用户态驱动程序这样的系统软件很有帮助。
