---
title: Building Linux Kernel
date: 2022-11-06 11:38:43
tags: [build]
categories: linux
---

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
| No rule to make target 'debian/canonical-certs.pem'            | scripts/config --disable SYSTEM_TRUSTED_KEYS           |
| No rule to make target 'debian/canonical-revoked-certs.pem'    | scripts/config --disable SYSTEM_REVOCATION_KEYS        |

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
- 制作 initramdisk (或者叫 initramfs) 初始系统镜像，它里面会包含 modules 
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
