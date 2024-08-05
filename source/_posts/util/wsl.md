---
title: Windows Subsystem for Linux
tags: [WSL]
categories: utilities
---

# WSL 是何物?

[WSL (Windows Subsystem for Linux)](https://zh.wikipedia.org/zh-cn/%E9%80%82%E7%94%A8%E4%BA%8ELinux%E7%9A%84Windows%E5%AD%90%E7%B3%BB%E7%BB%9F)是一个能够在 Windows 系统(Windows 10及以上版本)上原生运行 Linux 可执行文件的兼容层。目前有两个版本: WSL, WSL2

<!--more-->

查看 Windows 系统上的 WSL 版本

```powershell
wsl -l -v
```


# WSLg

Microsoft在`Build 21364.co_release`版本内发布了[WSLg](https://github.com/microsoft/wslg)功能，该功能允许在**WSL**里运行**X11**和**Wayland**的客户端程序(GUI Application).

![xeyes on WSL2](xeyes.gif)

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

![WSLg nautilus](wslg-nautilus.png)

## glmark2

![WSLg glmark2](wslg-glmark2.png)

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

# LKM in WSL2

WSL2 上没有可加载的内核模块，因为所有模块是被直接编进内核的。如果 `lsmod`, 不会输出任何模块。 但我们仍可以通过查看内核配置文件找到有哪些模块被编译进内核了。
