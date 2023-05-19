---
title: Linux Kernel Debugging
date: 2023-05-19 11:38:43
tags: linux
categories: kernel
---

# debugfs

## 如何在 WSL2 上启用 debugfs

确认下内核是否开启了 debugfs

```
zcat /proc/config.gz | grep CONFIG_DEBUG_FS
```

要想使用 debugfs，首先要挂载它到 `/sys/kernel/debug`

```
mount -t debugfs none /sys/kernel/debug/
```

如果想让挂载在系统重启后自动挂载，在 `/etc/fstab` 加下面一行

```
debugfs     /sys/kernel/debug       debugfs defaults        0 0
```

在添加后，执行 `mount -a` 命令将看到 `/sys/kernel/debug` 文件系统的内容

```
acpi   btt              clk               dma_buf          extfrag             kprobes        ras         split_huge_pages  usb
bdi    ceph             device_component  dri              fault_around_bytes  kvm            sched       swiotlb           virtio-ports
block  clear_warn_once  devices_deferred  error_injection  hid                 ramdisk_pages  sleep_time  tracing           x86
```

`/sys/kernel/debug` 目录的权限是 700, 只有 root 用户才能进入，sudo 也不行

# References

[1] https://access.redhat.com/solutions/5914171

