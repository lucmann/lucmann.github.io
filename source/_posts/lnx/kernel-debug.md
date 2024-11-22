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

# CONFIG_DYNAMIC_DEBUG

Dynamic Debug 就通过 `/sys/kernel/debug/dynamic_debug/control` 文件打开或关闭特定文件的某行或函数里的打印，但它只对使用

- `pr_debug()`
- `dev_dbg()`
- `print_hex_dump_debug()`
- `print_hex_dump_bytes()`

这 4 个函数的打印来有用。

# `CONFIG_DRM_USE_DYNAMIC_DEBUG`

这是内核 6.1 (2022-12-11) 才引入的专门控制 `drm_dbg()` 是否用 **Dynamic Debug** 来实现的可配置选项。

```c
#if !defined(CONFIG_DRM_USE_DYNAMIC_DEBUG)
#define drm_dev_dbg(dev, cat, fmt, ...)				\
	__drm_dev_dbg(NULL, dev, cat, fmt, ##__VA_ARGS__)
#else
#define drm_dev_dbg(dev, cat, fmt, ...)				\
	_dynamic_func_call_cls(cat, fmt, __drm_dev_dbg,		\
			       dev, cat, fmt, ##__VA_ARGS__)
#endif
```

# `CONFIG_DEBUG_ATOMIC_SLEEP`

[`dma_fence_wait_timeout()`](https://www.kernel.org/doc/html/latest/driver-api/dma-buf.html?highlight=dma_fence_wait_timeout#c.dma_fence_wait_timeout) 会睡眠调用进程直到 fence 被 signaled 或者指定定时器超时。该函数中会调用 `might_sleep()` 来标识 (annotation) 调用进程可能进入睡眠状态，并打印源文件名和行号，帮助调试。 但只有内核配置了 `CONFIG_DEBUG_ATOMIC_SLEEP` 才有效，否则 `__might_sleep()` 是一个空函数。

# kern.log

`/var/log/kern.log` 的一个主要问题是每行前面的 `%HOSTNAME%` 太长又没什么用，查了一下配置方法，实际上就是要在 `rsyslog.conf` 里定义一个 `$template`

```
$template SimpleFormat,"%timegenerated% %msg:::drop-last-lf%\n"
```

然后在 rsyslog.conf 的规则里加上这个 template

```
kern.* -/var/log/kern.log;SimpleFormat
```

注意，不要直接在 `/etc/rsyslog.conf` 里直接加上面这条规则，要加在 `/etc/rsyslog.d/50-default.conf`， 否则会输出两遍。

最后，就是要重启 rsyslog 服务生效。

内核的 log 实际上都是写入一个 ring buffer 里的，暴露给用户的接口是 `/proc/kmsg` 和 `/dev/kmsg`, rsyslog 服务也是通过这些接口，重新处理 log 后写入 `/var/log/kern.log` 的。下面的操作可以让 `dmesg` 多一条 log

```shell
echo "<3>HELLO" > /dev/kmsg
```

# References

- [https://access.redhat.com/solutions/5914171](https://access.redhat.com/solutions/5914171)
- [Dynamic Debug Howto](https://www.kernel.org/doc/html/v4.14/admin-guide/dynamic-debug-howto.html)
- [How to bind a template in rsyslog.conf](https://www.rsyslog.com/how-to-bind-a-template/)
