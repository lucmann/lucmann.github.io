---
title: Build X Server (Xorg) from Source
date: 2020-10-19 17:47:39
tags: [X11]
categories: graphics
---

# Motivation
Given that a difficulty to add a custom X11 device driver or module or extension for Xserver, I try to find out how an Xserver is built and its dependent drivers and modules and extensions are orgnized together by building it from [source](https://gitlab.freedesktop.org/xorg/xserver.git).

<!--more-->

# Build

Xorg 的源码库克隆下来后，checkout 到目标分支或 tag, 假设我们想编译 xorg-server-1.20.4 这个版本

```
git checkout xorg-server-1.20.4 -b 1.20.4  // 将这个 tag 检出后命名为 1.20.4 分支
```

X Server 的源码库中除了 Xorg，还包含 modesetting_drv.so, xnest, xwin, xvfb 等，这些组件并不全都需要，如果全都编译，可能会有更多的依赖。
另外，Xorg 本身安装在 /usr/bin/ 目录, 其它相关 DDX 驱动安装在 /usr/lib/xorg/modules 目录，为了让编译好的 Xorg 和相关驱动安装后能够覆盖原来的，需要通过指定 meson 的 `prefix` 和 `libdir` 构建变量

```
meson build -Dprefix=/usr -Dlibdir=lib -Dxnest=false -Dxwin=false -Dxvfb=false
```

NOTES: 默认情况下，`prefix=/usr/local`, `libdir=lib/x86_64-linux-gnu`

构建配置完成后，进行编译

```
ninja -C build
```

编译完成后，使用 sudo 进行安装

```
sudo ninja install -C build
```

安装成功后，使用 systemd 重启 X Server 服务 （假设使用的窗口管理器是 lightdm)

```
sudo systemctl restart lightdm
```

It is easy to find what are built with `find build -type f -perm /a+x -regex '.*[^0-9h]$'`
<pre>
./build/meson-private/sanitycheckc.exe
./build/test/tests
./build/test/simple-xinit
./build/hw/xnest/Xnest
./build/hw/vfb/Xvfb
./build/hw/xfree86/gtf
./build/hw/xfree86/vgahw/libvgahw.so
./build/hw/xfree86/libxorgserver.so
./build/hw/xfree86/fbdevhw/libfbdevhw.so
./build/hw/xfree86/drivers/modesetting/modesetting_drv.so
./build/hw/xfree86/dixmods/libglx.so
./build/hw/xfree86/dixmods/libwfb.so
./build/hw/xfree86/dixmods/libshadow.so
./build/hw/xfree86/shadowfb/libshadowfb.so
./build/hw/xfree86/cvt
./build/hw/xfree86/exa/libexa.so
./build/hw/xfree86/Xorg
./build/hw/xfree86/int10/libint10.so
./build/hw/xfree86/loader/xorg_symbol_test
</pre>

where `./build/hw/xfree86/Xorg` is newly built Xserver.

## Replacement
Now that we have an Xorg out there, we can try it. I have an *xrdp* installed on my WSL as its desktop environment. We can replace `xorgxrdp` with this new one for testing. To do it, we need to modify xrdp's init configuration file `/etc/xrdp/sesman.ini`

<pre>
[Xorg]
; Specify the path of non-suid Xorg executable. It might differ depending
; on your distribution and version. The typical path is shown as follows:
;
; Fedora 26 or later    :  param=/usr/libexec/Xorg
; Debian 9 or later     :  param=/usr/lib/xorg/Xorg
; Ubuntu 16.04 or later :  param=/usr/lib/xorg/Xorg
; Arch Linux            :  param=/usr/lib/xorg-server/Xorg
; CentOS 7              :  param=/usr/bin/Xorg or param=Xorg
;
;param=/usr/lib/xorg/Xorg
;
param=/home/luc/github/xserver/build/hw/xfree86/Xorg
</pre>

But it failed on restart xrdp.service. We check the log and `/etc/X11/xrdp/xorg.conf`. It turns out some drivers related xrdp do not exist. It is xrdp that customizes Xorg.

```shell
grep '(EE) Failed to' ~/.xorgxrdp.25.log
```
<pre>
[ 22862.100] (EE) Failed to load module "glx" (module does not exist, 0)
[ 22862.100] (EE) Failed to load module "int10" (module does not exist, 0)
[ 22862.100] (EE) Failed to load module "vbe" (module does not exist, 0)
[ 22862.100] (EE) Failed to load module "glamoregl" (module does not exist, 0)
[ 22862.100] (EE) Failed to load module "xorgxrdp" (module does not exist, 0)
[ 22862.100] (EE) Failed to load module "xrdpdev" (module does not exist, 0)
[ 22862.100] (EE) Failed to load module "xrdpmouse" (module does not exist, 0)
[ 22862.100] (EE) Failed to load module "xrdpkeyb" (module does not exist, 0)
</pre>

```shell
grep -E '(Driver | Load)' /etc/X11/xrdp/xorg.conf
```
<pre>
    Load "dbe"
    Load "ddc"
    Load "extmod"
    Load "glx"
    Load "int10"
    Load "record"
    Load "vbe"
    Load "glamoregl"
    Load "xorgxrdp"
    Load "fb"
    Driver "xrdpkeyb"
    Driver "xrdpmouse"
    Driver "xrdpdev"
</pre>

## Conclusion

Xserver/Xorg is built in a flexible and easy way. We can implement our own device-dependent functionality and add it to Xserver as its another driver or module.

# Xserver 相关工具

## xinit

```bash
➜  ~ dpkg --contents /var/cache/apt/archives/xinit_1.4.1-0ubuntu2_amd64.deb | awk '{print $NF}' | rg -v '/$'
./etc/X11/xinit/xinitrc
./etc/X11/xinit/xserverrc
./usr/bin/startx
./usr/bin/xinit
./usr/share/doc/xinit/NEWS.Debian.gz
./usr/share/doc/xinit/changelog.Debian.gz
./usr/share/doc/xinit/copyright
./usr/share/man/man1/startx.1.gz
./usr/share/man/man1/xinit.1.gz
```
