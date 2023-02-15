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

[编辑 /mnt/c/Users/luc/.wslconfig](https://falco.org/blog/falco-wsl2-custom-kernel/), 添加下面两行

```
[wsl2]
kernel=C:\\Users\\luc\\bzImage
```

> NOTE: 如果[以上两行添加进 /etc/wsl.conf 文件，不会有任何作用](https://devblogs.microsoft.com/commandline/automatically-configuring-wsl/), 因为 `/etc/wsl.conf` 不支持 `wsl2` Section


配置完成后，重启 WSL Ubuntu 20.04, 新编译的内核将生效。

```
wsl --shutdown
```

## 打开 [CONFIG_DRM_VKMS](https://docs.kernel.org/gpu/vkms.html)

```
make nconfig
```

> NOTE: [在 WSL 上不能将配置项置为 'M', 而要 'Y'](https://unix.stackexchange.com/questions/594470/wsl-2-does-not-have-lib-modules)


配置更改后重新 make

## Testing With IGT on WSL2

```
➜  tests git:(master) sudo ./kms_writeback --device "sys:/sys/devices/platform/vkms"
IGT-Version: 1.27.1-g45da871d (x86_64) (Linux: 5.15.90.1-microsoft-standard-WSL2+ x86_64)
(kms_writeback:9671) igt_kms-WARNING: Output Writeback-1 could not be assigned to a pipe
Starting subtest: writeback-pixel-formats
Subtest writeback-pixel-formats: SUCCESS (0.000s)
Starting subtest: writeback-invalid-parameters
Subtest writeback-invalid-parameters: SUCCESS (0.000s)
Starting subtest: writeback-fb-id
Subtest writeback-fb-id: SUCCESS (0.018s)
Starting subtest: writeback-check-output
Subtest writeback-check-output: SUCCESS (0.124s)
```

