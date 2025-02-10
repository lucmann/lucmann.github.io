---
title: Windows Subsystem for Linux
date: 2024-08-13 09:20:28+08:00
tags: [virtualization]
categories: utilities
---

# WSL

[WSL (Windows Subsystem for Linux)](https://zh.wikipedia.org/zh-cn/%E9%80%82%E7%94%A8%E4%BA%8ELinux%E7%9A%84Windows%E5%AD%90%E7%B3%BB%E7%BB%9F)是一个能够在 Windows 系统(Windows 10及以上版本)上原生运行 Linux 可执行文件的兼容层。目前有两个版本: WSL, WSL2

<!--more-->

查看 Windows 系统上的 WSL 版本

```powershell
wsl -l -v
```


# WSLg

Microsoft在`Build 21364.co_release`版本内发布了[WSLg](https://github.com/microsoft/wslg)功能，该功能允许在**WSL**里运行**X11**和**Wayland**的客户端程序(GUI Application).

![xeyes on WSL2](/images/wsl/xeyes.gif)

如果你已经加入[Windows Insider Program](https://insider.windows.com/zh-cn/)计划并且也正在使用**WSL2**, 那么只需要如下操作即可激活**WSLg**功能。

```
PS C:\Windows\system32> wsl --update
正在检查更新...
正在下载更新...
正在安装更新...
此更改将在 WSL 下次完全重启时生效。若要强制重启，请运行“wsl --shutdown”。
内核版本： 5.10.60.1
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

![WSLg nautilus](/images/wsl/wslg-nautilus.png)

## glmark2

![WSLg glmark2](/images/wsl/wslg-glmark2.png)

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

# 关于 WSL2 需要知道的几件事

- WSL2 所有的 Distros 使用同一个内核，你可以在同一个 WSL2 安装 Ubuntu, 安装 Debian, 安装 Kali, 但不管安装多少个 Distros, 它们都用同一个 Linux 内核
- WSL2 中所有内核模块都是直接被编译进内核的
- WSL2 中没有内核线程，或者说你看不到它们
