---
title: Building Linux Kernel
date: 2022-11-06 11:38:43
tags: [build]
categories: kernel
---

# Should Know Something About Kernel Building

- 配置内核最简单的方法是 `make olddefconfig`
- 内核配置保存在 .config
- 内核源码树里包含命令行修改 .config 的脚本 scripts/config
- 内核构建依赖 flex, bison 词法分析程序

# Build on Ubuntu 22.04

## Environment

| configuration             | version                            |
|:--------------------------|:-----------------------------------|
| distro                    | Ubuntu 22.04 Jammy Jellyfish       |
| gcc                       | 11.2.0                             |
| original kernel           | 5.15.0-43-generic                  |
| building kernel           | v6.0                               |

## Compilation Errors 

|  Errors                                                        | Resolution                                             |
|:---------------------------------------------------------------|:-------------------------------------------------------|
| gelf.h: No such file or directory                              | apt install libelf-dev                                 |
| <openssl/opensslv.h>: No such file or directory                | apt install libssl-dev                                 |
| No rule to make target 'debian/canonical-certs.pem'            | scripts/config --disable SYSTEM_TRUSTED_KEYS           |
| No rule to make target 'debian/canonical-revoked-certs.pem'    | scripts/config --disable SYSTEM_REVOCATION_KEYS        |

# Build on WSL2

## Build Config

```
make KCONFIG_CONFIG=Microsoft/config-wsl
```

## Compilation Errors

### `apt install -y dwarves`

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

## Direct WSL to the new vmlinux

### 新内核产生

```
  AS      arch/x86/boot/header.o
  CC      arch/x86/boot/version.o
  LD      arch/x86/boot/setup.elf
  OBJCOPY arch/x86/boot/setup.bin
  OBJCOPY arch/x86/boot/vmlinux.bin
  BUILD   arch/x86/boot/bzImage
Kernel: arch/x86/boot/bzImage is ready  (#3)
  MODPOST modules-only.symvers
  GEN     Module.symvers
```

### 新内核生效

编辑 [`/mnt/c/Users/luc/.wslconfig`](https://falco.org/blog/falco-wsl2-custom-kernel/), 添加下面两行

```
[wsl2]
kernel=C:\\Users\\luc\\bzImage
```

> NOTE: 不能通过向 [`/etc/wsl.conf`](https://devblogs.microsoft.com/commandline/automatically-configuring-wsl/), 添加下面两行
> 
> ```
> [wsl2]
> kernel=C:\\Users\\luc\\bzImage
> ```
>
> 来告诉 WSL 使用新内核，因为 `/etc/wsl.conf` 不支持 `wsl2` Section


配置完成后，重启 WSL Ubuntu 20.04

```
wsl --shutdown
```

