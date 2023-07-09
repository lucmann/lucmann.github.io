---
title: Linux Kernel Debugging
date: 2023-05-19 11:38:43
tags: linux
categories: linux
---

# `CONFIG_DEBUG_FS`

## 如何在 WSL2 上启用 debugfs

确认下内核是否开启了 debugfs

```
zcat /proc/config.gz | grep CONFIG_DEBUG_FS
```

要想使用 debugfs，首先要挂载它到 `/sys/kernel/debug`

```
mount -t debugfs none /sys/kernel/debug/
```

<!--more-->

如果想让挂载在系统重启后自动挂载，在 `/etc/fstab` 加下面一行

```
debugfs     /sys/kernel/debug       debugfs defaults        0 0
```

在添加后，执行 `mount -a` 命令将看到 `/sys/kernel/debug` 文件系统的内容

```
$ ls -ld ./* | sort -k 1.2
--w-------  1 root root 0 May 19 17:03 ./clear_warn_once
--w-------  1 root root 0 May 19 17:03 ./split_huge_pages
-r--r--r--  1 root root 0 May 19 17:03 ./devices_deferred
-r--r--r--  1 root root 0 May 19 17:03 ./sleep_time
-rw-r--r--  1 root root 0 May 19 17:03 ./fault_around_bytes
drwx------  8 root root 0 May 19 17:03 ./tracing
drwxr-xr-x  2 root root 0 May 19 17:03 ./acpi
drwxr-xr-x  2 root root 0 May 19 17:03 ./btt
drwxr-xr-x  2 root root 0 May 19 17:03 ./ceph
drwxr-xr-x  2 root root 0 May 19 17:03 ./clk
drwxr-xr-x  2 root root 0 May 19 17:03 ./device_component
drwxr-xr-x  2 root root 0 May 19 17:03 ./dma_buf
drwxr-xr-x  2 root root 0 May 19 17:03 ./error_injection
drwxr-xr-x  2 root root 0 May 19 17:03 ./extfrag
drwxr-xr-x  2 root root 0 May 19 17:03 ./hid
drwxr-xr-x  2 root root 0 May 19 17:03 ./kprobes
drwxr-xr-x  2 root root 0 May 19 17:03 ./kvm
drwxr-xr-x  2 root root 0 May 19 17:03 ./ramdisk_pages
drwxr-xr-x  2 root root 0 May 19 17:03 ./ras
drwxr-xr-x  2 root root 0 May 19 17:03 ./swiotlb
drwxr-xr-x  2 root root 0 May 19 17:03 ./usb
drwxr-xr-x  2 root root 0 May 19 17:03 ./virtio-ports
drwxr-xr-x  2 root root 0 May 19 17:03 ./x86
drwxr-xr-x  3 root root 0 May 19 17:03 ./dri
drwxr-xr-x  3 root root 0 May 19 17:03 ./sched
drwxr-xr-x 29 root root 0 May 19 17:03 ./block
drwxr-xr-x 35 root root 0 May 19 17:03 ./bdi
```

`/sys/kernel/debug` 目录的权限是 700, 只有 root 用户才能进入，sudo 也不行

# `CONFIG_DEBUG_ATOMIC_SLEEP`

[`dma_fence_wait_timeout()`](https://www.kernel.org/doc/html/latest/driver-api/dma-buf.html?highlight=dma_fence_wait_timeout#c.dma_fence_wait_timeout) 会睡眠调用进程直到 fence 被 signaled 或者指定定时器超时。该函数中会调用 `might_sleep()` 来标识 (annotation) 调用进程可能进入睡眠状态，并打印源文件名和行号，帮助调试。 但只有内核配置了 `CONFIG_DEBUG_ATOMIC_SLEEP` 才有效，否则 `__might_sleep()` 是一个空函数。

# `CONFIG_DRM_USE_DYNAMIC_DEBUG`

# References

[1] https://access.redhat.com/solutions/5914171

