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

### 基于 UEFI/GPT 的硬盘分区

- boot 分区 (EFI System Partition, ESP) 必须是 **FAT32** 文件系统
- boot 分区除了放 bootloader (GRUB) 的配置文件外，还会放 vmlinuz, initrd 这些，最好弄到 5G ~ 10G, 以容纳多个内核版本的 elf 和 initrd

### iwctl

### pacman

### arch-chroot

## GUI boot

### GNOME Desktop Environment

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

`yay` 主要是用来安装那些托管在 [AUR](https://aur.archlinux.org/packages/visual-studio-code-bin) (Archlinux User Repository) 的软件包的，作为 `pacman` 的补充。

```bash
yay -S visual-studio-code-bin oh-my-zsh-git autojump
```

### 中文字体及输入法


```bash 安装 fcitx5
pacman -S noto-fonts noto-fonts-cjk fcitx5 fcitx5-gtk fcitx5-chinese-addons fcitx5-configtool
```

```bash 安装 sogou
yay -S fcitx5-sogou
```

安装后启动 fcitx5-configtool，添加搜狗输入法即可

```bash All-In-One
pacman -S gnome gnome-extra noto-fonts noto-fonts-cjk fcitx5 fcitx5-gtk fcitx5-chinese-addons fcitx5-configtool tig htop autojump fzf
```

# 编译内核

要制作一个可启动的内核，涉及到 3 个步骤

- 编译内核，生成内核 ELF 可执行文件 vmlinuz (压缩格式)，安装内核模块
- 生成初始内存文件系统 initramfs
- 更新引导加载程序 bootloader 的配置，即 `/boot/grub/grub.cfg`
    
总的来说，在 Arch Linux 上制作一个可启动的新内核，让系统能够用新内核正常工作，是相对比较简单的，这也是我放弃 openKylin，转向 Arch Linux 的主要原因，在 openKylin 上，光是找到目前正在运行的内核源码都费半天劲儿，包括内核源码库，整个发行版的软件包源码仓库，软件版本管理都有点混乱。

## 配置内核

### CONFIG_MODULE_COMPRESS_ALL

控制 `make modules_install` 时是否压缩 `.ko` 文件，它依赖 `CONFIG_MODULE_COMPRESS`。

## 内核可执行文件

```bash
make
sudo make modules_install
sudo make install
```

- 内核配置 .config 可以通过 `zcat /proc/config.gz > .config` 得到一个基础配置，然后 `make menuconfig` 在此基础上修改保存。
- vmlinuz 的安装脚本有一个搜索顺序 `scripts/install.sh -> (1) ~/bin/installkernel; (2) /sbin/installkernel; (3) arch/x86/boot/install.sh`, 而且默认的安装名总是 `vmlinuz`, 为了更好地区分多个 `vmlinuz`, 最好将 `kernel version` 加到 `vmlinuz` 后面。如果使用第一搜索顺序，在 `sudo make install` 时，需要

```
sudo --preserve-env=HOME make install
```

## 制作初始内存文件系统

```bash
sudo mkinitcpio --generate /boot/initramfs-6.16.1-arch1.img --kernel 6.16.1-arch1
```

其中 `--kernel` 参数来自 `make kernelrelease`, 它也是 `sudo make modules_install` 后在 `/lib/modules` 下创建的目录名. 同时新内核安装重启后有 **`make kernelrelease ≡ uname -r`**

## 更新 bootloader 配置

```bash
sudo grub-mkconfig -o /boot/grub/grub.cfg
```

这一步在两种情况下需要执行：
- 修改了 bootloader 程序的配置，如修改了 `/etc/default/grub`
- 当 `/boot` 目录新增了 `vmlinuz` 和 `initramfs.img`， 如果只是重新编译了内核，而安装的 vmlinuz 和 initramfs.img 还是**原来的名字**, 就无需执行这一步 

# 截屏 Greeter

截屏系统登录界面(Login Greeter) 并不那么容易，因为它和用户登录之后的 Display Manager 会话通常是两个独立的 Session, 它们各自由一个独立的 Xorg 进程渲染(如果当前会话类型 `XDG_SESSION_TYPE` 是 *wayland*, 这两个 X 服务则变成 `Xwayland`), 而且通过在 GDM 环境下的观察，负责渲染 Greeter 的进程(Xorg 或 Xwayland)是反复创建和销毁的，当你切换到登录界面时，它们才被 `gdm-x-session`(或 `gdm-wayland-session`) 拉起(创建)，一旦登录后进入桌面，这个进程又会被很快销毁。

![](/images/arch/gdm-wayland-session.png)

如果不了解这一点，在你尝试[截屏 GDM login screen](https://ao2.it/en/blog/2016/10/01/capturing-screenshot-gdm-login-screen) 时，就会很不顺利。不过下面的截屏方法只在 x11 会话下有效

```bash
#!/bin/bash

sleep 6

sudo -u gdm DISPLAY=:0 XAUTHORITY=/run/user/120/gdm/Xauthority import -display :0 -win root /tmp/gdm-screenshot.png
```

## GNOME Display Manager

## [Observer Effect](https://en.wikipedia.org/wiki/Observer_effect_(physics))-style `/usr/lib/gdm-x-session`

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

# 更换 boot partition

需要使用 USB 启动盘和 **parted** 命令， **parted** 的只能改变原有分区的 end (分区的 start,end), 如果你的当前分区是这样的

```
Number  Start   End     Size    File system  Name  Flags
 1      17.4kB  1074MB  1074MB  fat32              boot, esp
 2      1074MB  1000GB  999GB   ext4
```

是无法直接扩大 boot 分区的，可行的方案是先缩小 root 分区，然后用释放的空间重新创建 boot 的分区，再将原 boot 分区的内容整个 copy 到新的 boot 分区

1. `resize2fs /dev/nvme0n1p2 990G`
2. `(parted) resizepart 2 991GiB`
3. `resizepart /dev/nvme0n1 2 1931503615 (in 512-bytes sectors)`
4. `(parted) mkpart "EFI system partition" ext4 991GiB 100%`

# 参考

- [Arch Linux 详细安装教程](https://zhuanlan.zhihu.com/p/596227524)
- [Arch Linux User Repository](https://aur.archlinux.org/)
- [GNOME Display Manager Reference Manual](https://help.gnome.org/admin/gdm/stable/)
- [Capturing a screenshot of GDM login screen](https://ao2.it/en/blog/2016/10/01/capturing-screenshot-gdm-login-screen)
