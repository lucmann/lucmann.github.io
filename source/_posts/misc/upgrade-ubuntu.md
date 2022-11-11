---
title: Sogou IM on Ubuntu Jammy Jellyfish
date: 2022-07-31 20:47:35
tags: ubuntu
categories: misc
---

# Reminiscence

<!--more-->

# Display Drivers for NVIDIA GT 520M

![Before NVIDIA Display Driver Installed](before-install-driver.png)

## What Will Be Installed By This Installer?

```bash
$ find /usr/lib/modules -name '*nvidia*.ko' -exec ls -l --time-style=long-iso {} \;| awk '{print $(NF-2), substr($NF, 43)}' | sort

2022-07-12 drivers/i2c/busses/i2c-nvidia-gpu.ko
2022-07-12 drivers/platform/x86/nvidia-wmi-ec-backlight.ko
2022-07-12 drivers/usb/typec/altmodes/typec_nvidia.ko
2022-07-12 drivers/video/fbdev/nvidia/nvidiafb.ko
2022-10-13 drivers/i2c/busses/i2c-nvidia-gpu.ko
2022-10-13 drivers/platform/x86/nvidia-wmi-ec-backlight.ko
2022-10-13 drivers/usb/typec/altmodes/typec_nvidia.ko
2022-10-13 drivers/video/fbdev/nvidia/nvidiafb.ko
2022-11-11 drivers/video/nvidia-drm.ko
2022-11-11 drivers/video/nvidia.ko
2022-11-11 drivers/video/nvidia-modeset.ko
2022-11-11 drivers/video/nvidia-uvm.ko
```

此外，Installer 允许你备份原来的 Xorg.conf, 使用新的 nvidia.conf

```bash
$ find /etc -newercc /usr/lib/modules/5.15.0-52-generic/kernel/drivers/video/nvidia.ko

/etc/X11/xorg.conf
/etc/X11/xorg.conf.nvidia-xconfig-original
```

# Sogou IM

Sogou 输入法真心做得不错，支持主流的国产 Linux 操作系统。今天将系统升级到 Ubuntu 22.04.1 LTS 后之前安装的 Sogoupinyin 突然不工作了。刚开始没想到是缺少 `libqt5qml5` 这个依赖，一顿重装 fcitx, sogoupinyin 操作后，还是不行，google 后才发现原来 sogoupinyin 安装后，可执行程序都在

```
/opt/sogoupinyin/files/bin
```

通过运行这个目录下的 `sogoupinyin-service` 这个程序后才发现它无法加载 libqt5qml5.

另外，`sogoupinyin-configtool` 也在上面这个目录，而且 sogoupinyin 也支持五笔

![sogoupinyin config interface](sogoupinyin-configtool.png)

# Alacritty

[Alacritty](https://github.com/alacritty/alacritty)是一个 GPU 加速的 Terminal Emulator.

升级系统后，感觉系统默认的 `gnome-terminal` 的标题栏很碍眼，所以想换一个新的 Terminal Emulator 试试，就选择了之前在学习 Rust 时听说过的 Alacritty, 但目前 Jammy Jellyfish 的官方 PPA 里并没有它的安装包。下面这个 PPA 安装过程很丝滑, 而且它自动将安装好的 `/usr/bin/alacritty` 设置为默认的 `x-terminal-emulator`

```
sudo add-apt-repository ppa:aslatter/ppa
```

安装后，`Ctrl-Alt-T`，Good Looking

![Alacritty](alacritty-first-sight.png)
