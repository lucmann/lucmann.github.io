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

# 参考

- [Arch Linux 详细安装教程](https://zhuanlan.zhihu.com/p/596227524)
- [Arch Linux User Repository](https://aur.archlinux.org/)
- [GNOME Display Manager Reference Manual](https://help.gnome.org/admin/gdm/stable/)
- [Capturing a screenshot of GDM login screen](https://ao2.it/en/blog/2016/10/01/capturing-screenshot-gdm-login-screen)
