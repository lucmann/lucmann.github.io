---
title: Windows Subsystem for Linux
date: 2020-02-08 17:57:46
tags: [WSL]
categories: utilities
---

# WSL 是何物?

[WSL](https://zh.wikipedia.org/zh-cn/%E9%80%82%E7%94%A8%E4%BA%8ELinux%E7%9A%84Windows%E5%AD%90%E7%B3%BB%E7%BB%9F)是一个能够在 Windows 系统(Windows 10及以上版本)上原生运行 Linux 可执行文件的兼容层。

个人感觉有了 WSL/WSL2 后，以下事物基本成为过去:

- 双系统 (Windows + Linux)
- [Cygwin](https://www.cygwin.com/)
- VMWare (Windows Host 上安装 Linux Guests)

The story begins with debugging an OpenGL demo on the WSL with the Mesa libraries compiled by myself. Every time the executable will crash when it stops at the breakpoint and steps into the function. The gdb outputs as follows:
``` bash
Thread 1 "osdemo" hit Breakpoint 1, OSMesaMakeCurrent (osmesa=0x8056bf0, buffer=0x7ffff85f0010, type=5121, width=400, height=400)
    at ../src/gallium/state_trackers/osmesa/osmesa.c:763
763     {
(gdb) n
The futex facility returned an unexpected error code.
The futex facility returned an unexpected error code.
The futex facility returned an unexpected error code.
The futex facility returned an unexpected error code.
The futex facility returned an unexpected error code.
The futex facility returned an unexpected error code.
The futex facility returned an unexpected error code.
The futex facility returned an unexpected error code.
The futex facility returned an unexpected error code.
The futex facility returned an unexpected error code.
The futex facility returned an unexpected error code.
The futex facility returned an unexpected error code.
The futex facility returned an unexpected error code.
The futex facility returned an unexpected error code.
The futex facility returned an unexpected error code.
The futex facility returned an unexpected error code.

Thread 13 "osdemo" received signal SIGABRT, Aborted.
[Switching to Thread 0x7fffe0f90700 (LWP 4073)]
__GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:50
50      ../sysdeps/unix/sysv/linux/raise.c: No such file or directory.
(gdb) bt
#0  __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:50
#1  0x00007ffffb485899 in __GI_abort () at abort.c:79
#2  0x00007ffffb4f009f in __libc_message (action=(do_abort | do_backtrace), fmt=0x7ffffb618fbe "%s", fmt=0x7ffffb618fbe "%s",
    action=(do_abort | do_backtrace)) at ../sysdeps/posix/libc_fatal.c:181
#3  0x00007ffffb4f0440 in __GI___libc_fatal (message=message@entry=0x7ffffb405000 "The futex facility returned an unexpected error code.\n")
    at ../sysdeps/posix/libc_fatal.c:191
#4  0x00007ffffb3fd34a in futex_fatal_error () at ../sysdeps/nptl/futex-internal.h:200
#5  futex_wake (private=<optimized out>, processes_to_wake=<optimized out>, futex_word=<optimized out>) at ../sysdeps/unix/sysv/linux/futex-internal.h:251
#6  __condvar_confirm_wakeup (private=<optimized out>, cond=<optimized out>) at pthread_cond_wait.c:55
#7  __pthread_cond_wait_common (abstime=0x0, clockid=0, mutex=0x806fad0, cond=0x806faf8) at pthread_cond_wait.c:425
#8  __pthread_cond_wait (cond=0x806faf8, mutex=0x806fad0) at pthread_cond_wait.c:638
#9  0x00007ffffc0be0cb in cnd_wait (mtx=0x806fad0, cond=0x806faf8) at ../include/c11/threads_posix.h:155
#10 lp_cs_tpool_worker (data=data@entry=0x806fad0) at ../src/gallium/drivers/llvmpipe/lp_cs_tpool.c:48
#11 0x00007ffffc0bdeab in impl_thrd_routine (p=<optimized out>) at ../include/c11/threads_posix.h:87
#12 0x00007ffffb3f6669 in start_thread (arg=<optimized out>) at pthread_create.c:479
#13 0x00007ffffb582323 in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:95
```
I did not consider this crash as a WSL bug until I found this [issue](https://github.com/Microsoft/WSL/issues/3618) on the github. Then I realized that I had to update my WSL that is updated as the Windows is updated. The easy way to get the latest Windows build version is to register the [Windows Insider Program](https://insider.windows.com/zh-cn/) that only requires

# WSLg

Microsoft在`Build 21364.co_release`版本内发布了[WSLg](https://github.com/microsoft/wslg)功能，该功能允许在**WSL**里运行**X11**和**Wayland**的客户端程序(GUI Application).

<!--more-->

如果你已经加入[Windows Insider Program](https://insider.windows.com/zh-cn/)计划并且也正在使用**WSL2**, 那么只需要如下操作即可激活**WSLg**功能。

```
PS C:\Windows\system32> wsl --update
正在检查更新...
正在下载更新...
正在安装更新...
此更改将在 WSL 下次完全重启时生效。若要强制重启，请运行“wsl --shutdown”。
内核版本： 5.10.60.1
```

启动 WSL2 后，查看内核版本

```
➜  ~ uname -r
5.10.60.1-microsoft-standard-WSL2
```

升级 WSL2 后，在 `/mnt` 目录下会比原来多出一个 `wslg` 的目录

```
➜  ~ cd /mnt/
c/     wsl/   wslg/
```

## nautilus

```
apt install nautilus -y
```

![WSLg nautilus](wsl/wslg-nautilus.png) 

## glmark2

![WSLg glmark2](wsl/wslg-glmark2.png) 

# WSL 如何支持 Nvidia

在 `/usr/lib/wsl/lib` 下默认安装了这些 shared libraries (usermode driver). 而且搭配了自动生成的 `/etc/ld.so.conf.d/ld.wsl.conf` 文件，能够让动态库加载器正确找到 nvidia 相关的库。

- libcuda.so
- libcuda.so.1
- libcuda.so.1.1
- libd3d12.so
- libd3d12core.so
- libdxcore.so
- libnvcuvid.so
- libnvcuvid.so.1
- libnvdxdlkernels.so
- libnvidia-encode.so
- libnvidia-encode.so.1
- libnvidia-ml.so.1
- libnvidia-opticalflow.so
- libnvidia-opticalflow.so.1
- libnvwgf2umx.so
- nvidia-smi