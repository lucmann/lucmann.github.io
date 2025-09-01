---
title: Linux Kernel Debugging
date: 2023-05-19 11:38:43
tags: kernel
categories: linux
---

# CONFIG_DEBUG_FS

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

# CONFIG_TRACING

CONFIG_TRACING 是内核 2.6.27 (2008-10-09)* 引入的一个**不可配置的(not configurable)** 内核选项，所谓**不可配置**的意思是这个 CONFIG **不能直接**由用户打开或关闭，它的开启与否只能由其它依赖它的选项的状态决定。

它对应的 **tracefs** 也需要挂载

```shell
mount -t tracefs none /sys/kernel/tracing
```
像 **ftrace**, **trace-cmd** 都需要挂载这个目录。

## tracers

要使用 **ftrace**, 不光要挂载 tracefs, 内核相应的追踪器 (tracer) 也要编译进内核，可用的内核追踪器可以通过 `cat /sys/kernel/tracing/available_tracers` 查看。

- CONFIG_FUNCTION_TRACER
- CONFIG_FUNCTION_GRAPH_TRACER
- CONFIG_HWLAT_TRACER
- CONFIG_IRQSOFF_TRACER
- CONFIG_OSNOISE_TRACER
- CONFIG_PREEMPT_TRACER
- CONFIG_SCHED_TRACER
- CONFIG_STACK_TRACER
- CONFIG_CONTEXT_SWITCH_TRACER
- CONFIG_NOP_TRACER

## ftrace

## trace-cmd

**trace-cmd** 实际上是为了方便使用 **ftrace**, 比方下面的 trace-cmd 命令与直接 `cat /sys/kernel/tracing/available_events` 是等价的

```shell
trace-cmd list -e
```

# CONFIG_IKCONFIG_PROC

这个配置决定 `/proc/config.gz` 是否存在。Ubuntu 似乎默认关闭此配置，而是将内核 config 保存在 `/boot/config-$(uname -r)`

# CONFIG_DYNAMIC_DEBUG

打开 **Dynamic Debug** 有好几种，但前提都是 `CONFIG_DYNAMIC_DEBUG=y`

## 方法一：运行时打开

Dynamic Debug 就通过 `/sys/kernel/debug/dynamic_debug/control` 文件打开或关闭特定文件的某行或函数里的打印，但它只对使用

- `pr_debug()`
- `dev_dbg()`
- `print_hex_dump_debug()`
- `print_hex_dump_bytes()`

这 4 个函数的打印来有用。

## 方法二：内核启动参数

- 按 `file` 打开 dynamic debug

<span style="background-color: yellow; padding: 4px;">dyndbg=\"file arch/x86/pci/* +p\"</span>

这个启动参数会打开 `arch/x86/pci` 目录下所有 dynamic debug 打印，如果通过修改 `/etc/default/grub` 里的 `GRUB_CMDLINE_LINUX`, 需要对双引号进行转义

```
GRUB_CMDLINE_LINUX="dyndbg=\"file arch/x86/pci/* +p\""
```

- 按 `module` 打开 dynamic debug

<span style="background-color: yellow; padding: 4px;">dyndbg=\"module i915 +p\"</span>

如果要同时将 `arch/x86/pci/` 目录下所有文件和内核模块 `i915` 的 dynamic debug 都打开

<span style="background-color: yellow; padding: 4px;">dyndbg=\"file arch/x86/pci/* +p;module i915 +p\"</span>

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

`/var/log/kern.log` 的一个主要问题是每行前面的 `%HOSTNAME%` 太长又没什么用，`%timegenerated%` 和内核原始打印 `%msg%` 里的时间戳实际上有一个就可以了。查了一下配置方法，实际上就是要在 `/etc/rsyslog.conf` 里定义一个 `$template`, 并指定为默认的模板

```
$template SimpleFormat,"%timegenerated:::date-rfc3339% %msg:::drop-last-lf%\n"
$ActionFileDefaultTemplate SimpleFormat
```

或在 `/etc/rsyslog.d/50-default.conf` 里的 `kern.*` 规则中加上这个 template

```
kern.* -/var/log/kern.log;SimpleFormat
```

注意，不要直接在 `/etc/rsyslog.conf` 里直接加上面这条规则，要加在 `/etc/rsyslog.d/50-default.conf`， 否则会输出两遍。

最后，就是要重启 rsyslog 服务生效。

内核的 log 实际上都是写入一个 ring buffer 里的，暴露给用户的接口是 `/proc/kmsg` 和 `/dev/kmsg`, rsyslog 服务也是通过这些接口，重新处理 log 后写入 `/var/log/kern.log` 的。下面的操作可以让 `dmesg` 多一条 log

```shell
echo "<3>HELLO" > /dev/kmsg
```

# `sysctl` 和内核参数

运行时内核参数大多在 `/proc/sys/` 目录下，二进制程序 `/usr/sbin/sysctl` 可用来查看，修改这些内核参数。

- 查看某个内核参数的值
	- `sysctl kernel.perf_event_paranoid`
- 列出所有内核参数的值 (有些需要 root 权限)
	- `sysctl -a`
- 修改某个内核参数的值
	- `sysctl kernel.perf_event_paranoid=-1`

# References

- [https://access.redhat.com/solutions/5914171](https://access.redhat.com/solutions/5914171)
- [Dynamic Debug Howto](https://www.kernel.org/doc/html/v4.14/admin-guide/dynamic-debug-howto.html)
- [How to bind a template in rsyslog.conf](https://www.rsyslog.com/how-to-bind-a-template/)
- [Rsyslog properties](https://www.rsyslog.com/doc/configuration/properties.html#message-properties)
