---
title: Linux Kernel Debugging
date: 2023-05-19 11:38:43
tags: linux
categories: linux
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

# References

[1] https://access.redhat.com/solutions/5914171

