---
title: Linux Kernel Debugging
date: 2023-05-19 11:38:43
tags: kernel
categories: linux
---

| Distro      | Ubuntu                        | Arch Linux                    |
|:------------|:------------------------------|:------------------------------|
| source      |[git://git.launchpad.net/~ubuntu-kernel/ubuntu/+source/linux/+git/jammy](git://git.launchpad.net/~ubuntu-kernel/ubuntu/+source/linux/+git/jammy) |[git@github.com:archlinux/linux.git](git@github.com:archlinux/linux.git) |
| initrd      | `sudo mkinitramfs -o /boot/initrd.img-KERNELVERSION KERNELVERSION` | `sudo mkinitcpio --generate /boot/initramfs-KERNELVERSION.img --kernel KERNELVERSION` |
| grub        | `sudo update-grub`            | `sudo grub-mkconfig -o /boot/grub/grub.cfg`            |

- `KERNELVERSION` 是 `make kernelrelease` 的输出，同时也是 `sudo make modules_install` 时在 `/lib/modules` 创建的目录名

<!--more-->

# 编译内核

- 配置内核最简单的方法是 `make olddefconfig`
- 内核配置保存在 .config
- 内核源码树里包含命令行修改 .config 的脚本 scripts/config
- 内核构建依赖 flex, bison 词法分析程序

## Ubuntu

- Environment

| configuration             | version                            |
|:--------------------------|:-----------------------------------|
| distro                    | Ubuntu 22.04 Jammy Jellyfish       |
| gcc                       | 11.2.0                             |
| original kernel           | 5.15.0-43-generic                  |
| building kernel           | v6.0                               |

- Compilation Errors 

|  Errors                                                        | Need to do                                             |
|:---------------------------------------------------------------|:-------------------------------------------------------|
| gelf.h: No such file or directory                              | apt install libelf-dev                                 |
| <openssl/opensslv.h>: No such file or directory                | apt install libssl-dev                                 |
| No rule to make target 'debian/canonical-certs.pem'            | ./scripts/config --set-str SYSTEM_TRUSTED_KEYS ""      |
| No rule to make target 'debian/canonical-revoked-certs.pem'    | ./scripts/config --set-str SYSTEM_REVOCATION_KEYS ""   |

## Windows Subsytem for Linux

- .config

```
make KCONFIG_CONFIG=Microsoft/config-wsl
```

- Compilation Errors
  [这是一个非常常见的依赖问题](https://blog.csdn.net/qq_36393978/article/details/124274364)

  ```
    LD      vmlinux.o
    MODPOST vmlinux.symvers
    MODINFO modules.builtin.modinfo
    GEN     modules.builtin
  BTF: .tmp_vmlinux.btf: pahole (pahole) is not available
  Failed to generate BTF for vmlinux
  Try to disable CONFIG_DEBUG_INFO_BTF
  make: *** [Makefile:1218: vmlinux] Error 1
  ```
# 安装内核

安装内核包括 4 部分：

- 安装 vmlinuz, 也就是压缩格式的内核 ELF 镜像 `make install`
    - `make install` 最终要么调用用户自定义的安装脚本，要么调用内核源码树里的 `install.sh` 脚本
      ![](/images/kernel-build/install.png)
- 安装 modules, `make modules_install`
- 制作 initramdisk (或者叫 initramfs) 初始化内存盘，它里面会包含必要的 modules 
- 更新 grub, 以便启动时可以选择新内核

## Ubuntu

## Windows Subsystem for Linux


- `mv arch/x86/boot/bzImage /mnt/c/Users/luc/`

- [编辑 /mnt/c/Users/luc/.wslconfig](https://falco.org/blog/falco-wsl2-custom-kernel/), 添加下面两行

```
[wsl2]
kernel=C:\\Users\\luc\\bzImage
```

> NOTE: 如果[以上两行添加进 /etc/wsl.conf 文件，不会有任何作用](https://devblogs.microsoft.com/commandline/automatically-configuring-wsl/), 因为 `/etc/wsl.conf` 不支持 `wsl2` Section


- 配置完成后，重启 WSL Ubuntu 20.04, 新编译的内核将生效。不要用 `wsl --terminate`.

```
wsl --shutdown Ubuntu-20.04
```
# 调试内核
## 与调试相关的内核配置

### CONFIG_DEBUG_FS

#### 如何在 WSL2 上启用 debugfs

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

在添加后，执行 `mount -a` 命令将看到 `/sys/kernel/debug` 文件系统的内容, `/sys/kernel/debug` 目录的权限是 700, 只有 root 用户才能进入，sudo 也不行

### CONFIG_TRACING

CONFIG_TRACING 是内核 2.6.27 (2008-10-09)* 引入的一个**不可配置的(not configurable)** 内核选项，所谓**不可配置**的意思是这个 CONFIG **不能直接**由用户打开或关闭，它的开启与否只能由其它依赖它的选项的状态决定。

它对应的 **tracefs** 也需要挂载

```shell
mount -t tracefs none /sys/kernel/tracing
```
像 **ftrace**, **trace-cmd** 都需要挂载这个目录。

#### tracers

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

#### ftrace

#### trace-cmd

**trace-cmd** 实际上是为了方便使用 **ftrace**, 比方下面的 trace-cmd 命令与直接 `cat /sys/kernel/tracing/available_events` 是等价的

```shell
trace-cmd list -e
```

### CONFIG_IKCONFIG_PROC

这个配置决定 `/proc/config.gz` 是否存在。Ubuntu 似乎默认关闭此配置，而是将内核 config 保存在 `/boot/config-$(uname -r)`

### CONFIG_DYNAMIC_DEBUG

打开 **Dynamic Debug** 有好几种，但前提都是 `CONFIG_DYNAMIC_DEBUG=y`

#### 方法一：运行时打开

Dynamic Debug 就通过 `/sys/kernel/debug/dynamic_debug/control` 文件打开或关闭特定文件的某行或函数里的打印，但它只对使用

- `pr_debug()`
- `dev_dbg()`
- `print_hex_dump_debug()`
- `print_hex_dump_bytes()`

这 4 个函数的打印来有用。

#### 方法二：内核启动参数

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

#### 方法三：/etc/modprobe.d/xxx.conf

由于 `dyndbg=` 内核参数一般在内核模块加载前就解析了，所以对于按模块打开 dynamic debug 来说，在 `/etc/modprobe.d/i915-debug.conf` 文件中设置更保险

```
options i915 dyndbg=+p
```

### `CONFIG_DRM_USE_DYNAMIC_DEBUG`

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

### `CONFIG_DEBUG_ATOMIC_SLEEP`

[`dma_fence_wait_timeout()`](https://www.kernel.org/doc/html/latest/driver-api/dma-buf.html?highlight=dma_fence_wait_timeout#c.dma_fence_wait_timeout) 会睡眠调用进程直到 fence 被 signaled 或者指定定时器超时。该函数中会调用 `might_sleep()` 来标识 (annotation) 调用进程可能进入睡眠状态，并打印源文件名和行号，帮助调试。 但只有内核配置了 `CONFIG_DEBUG_ATOMIC_SLEEP` 才有效，否则 `__might_sleep()` 是一个空函数。

## kern.log

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

## `sysctl` 和内核参数

运行时内核参数大多在 `/proc/sys/` 目录下，二进制程序 `/usr/sbin/sysctl` 可用来查看，修改这些内核参数。

- 查看某个内核参数的值
	- `sysctl kernel.perf_event_paranoid`
- 列出所有内核参数的值 (有些需要 root 权限)
	- `sysctl -a`
- 修改某个内核参数的值
	- `sysctl kernel.perf_event_paranoid=-1`

# 参考
- [https://wiki.ubuntu.com/Kernel/SourceCode](https://wiki.ubuntu.com/Kernel/SourceCode)
- [https://wiki.archlinux.org/title/Installation_guide](https://wiki.archlinux.org/title/Installation_guide)
- [https://access.redhat.com/solutions/5914171](https://access.redhat.com/solutions/5914171)
- [Dynamic Debug Howto](https://www.kernel.org/doc/html/v4.14/admin-guide/dynamic-debug-howto.html)
- [How to bind a template in rsyslog.conf](https://www.rsyslog.com/how-to-bind-a-template/)
- [Rsyslog properties](https://www.rsyslog.com/doc/configuration/properties.html#message-properties)
