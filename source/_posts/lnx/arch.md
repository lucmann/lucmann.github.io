---
title: Hello, Arch Linux
date: Sat Jul 26 17:36:06 CST 2025
tags: [distro]
categories: linux
---

![](/images/arch/gdm-screenshot.png)

<!--more-->

# 安装 Arch Linux

安装 Arch Linux 可以分为两个阶段：

- CLI：系统可以从 console 登录
- GUI：安装图形桌面环境，可以从 login greeter 登录
    * 这里就体现出 Arch Linux 的特点，你可以自由安装喜欢的桌面环境: KDE Plasma, GNOME
    * 桌面环境安装后，还有就是各种图形应用，包括 google-chrome, fcitx5-configtool, vscode 等等，有些没有在 pacman 的源里，需要使用 yay (Yet Another Yogurt, Yogurt 是早期的 AUR 助手) 安装
    * pacman 的源可以自己配成国内源，但 yay 的好多下载地址都XX了，所以这个阶段可能需要梯子，否则很难进行下去

## CLI boot

### iwctl

### pacman

### arch-chroot

## GUI boot

### DE

```bash
pacman -S gnome gnome-extra
```

### yay

```bash
git clone https://aur.archlinux.org/yay-git.git
pacman -S --needed base-devel
cd yay-git
makepkg -si
```

### 中文字体

```bash
pacman -S noto-fonts noto-fonts-cjk
```

### 中文输入法

```bash
pacman -S fcitx5 fcitx5-gtk fcitx5-chinese-addons fcitx5-configtool
yay -S fcitx5-sogou
```

安装后启动 fcitx5-configtool，添加搜狗输入法即可

# 编译内核

## vmlinuz

## initramfs

## mkinitcpio

# 截屏 Greeter

截屏系统登录界面(Login Greeter) 并不那么容易，因为它和用户登录之后的 Display Manager 会话通常是两个独立的 Session, 它们背后是两个 Xorg 进程(如果 `XDG_SESSION_TYPE` 是 *wayland*, 则是 Xwayland), 而且通过在 GDM 环境下的观察，负责渲染 Greeter 会话的 Xorg 进程是反复创建和销毁的，当你切换到登录界面时，这个 Xorg (或 Xwayland) 进程才被 `gdm-x-session`(或 `gdm-wayland-session`) 拉起，一旦登录后进入桌面，这个进程又会被很快销毁。

![](/images/arch/gdm-wayland-session.png)

如果不了解这一点，在你尝试[截屏 GDM login screen](https://ao2.it/en/blog/2016/10/01/capturing-screenshot-gdm-login-screen) 时，就会很不顺利。不过下面的截屏方法只在 x11 会话下有效

```bash
#!/bin/bash

sleep 6

sudo -u gdm DISPLAY=:0 XAUTHORITY=/run/user/120/gdm/Xauthority import -display :0 -win root /tmp/gdm-screenshot.png
```

## GNOME Display Manager

## "阅后即焚"式生成 Greeter

![](/images/arch/gdm-x-session-oneshot.png)

# 选 GPU

当 Linux 系统中同时存在多个 GPU 时，有多种方法可以指定使用哪一个。

## kernel boot parameter

通过内核启动参数，如 `modprobe.blacklist=i915`，将相应 GPU 的内核驱动屏蔽，但有时可能有副作用，例如我的 Dell Vostro 5390，如果将 `i915` 驱动屏蔽掉，Audio Subsystem 会出问题

```
00:1c.0 PCI bridge [0604]: Intel Corporation Cannon Point-LP PCI Express Root Port #5 [8086:9dbc] (rev f0)
00:1d.0 PCI bridge [0604]: Intel Corporation Cannon Point-LP PCI Express Root Port #13 [8086:9db4] (rev f0)
00:1f.3 Audio device [0403]: Intel Corporation Cannon Point-LP High Definition Audio Controller [8086:9dc8] (rev 30)
```

由于Vostro 使用的 Audio interface 就是 Intel HDA, 所以当 `i915` 被屏蔽后，由于某种原因，Intel HDA 设备没有初始化成功，而它又没有连接其它独显的音频接口(如 Nvidia HDA)，导致系统的声音不能正常工作

```
pci 0000:00:1f.3: deferred probe pending: snd_hda_intel: couldn't bind with audio component
nouveau 0000:01:00.0: Enabling HDA controller
```

基本上只有内核启动日志里有下面一行，intel audio 才能正常工作

```
snd_hda_intel 0000:00:1f.3: bound 0000:00:02.0 (ops intel_audio_component_bind_ops [i915])
```

更保险的启动参数是 `i915.modeset=0`，即不禁用整个 i915 驱动，只禁用 graphics 功能。但 [snd_hda_intel 和 i915 好像有些耦合问题](https://bbs.archlinux.org/viewtopic.php?id=292453)，必须再使用 `snd_hda_core.gpu_bind=0` 才能只禁用 Intel graphics, 不影响 Intel audio

# 参考

- [Arch Linux 详细安装教程](https://zhuanlan.zhihu.com/p/596227524)
- [Arch Linux User Repository](https://aur.archlinux.org/)
- [GNOME Display Manager Reference Manual](https://help.gnome.org/admin/gdm/stable/)
- [Capturing a screenshot of GDM login screen](https://ao2.it/en/blog/2016/10/01/capturing-screenshot-gdm-login-screen)
