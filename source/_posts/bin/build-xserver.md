---
title: Build Xserver from Source
date: 2020-10-19 17:47:39
tags: [X]
---

# Motivation
Given that a difficulty to add a custom X11 device driver or module or extension for Xserver, I try to find out how an Xserver is built and its dependent drivers and modules and extensions are orgnized together by building it from [source](https://gitlab.freedesktop.org/xorg/xserver.git).

<!--more-->

# Build
Fortunately the xserver has already an alternative build system support for [Meson](https://mesonbuild.com/) more than autotools before. Build configuration options are listed in the file `meson_options.txt` which makes easy configure the build. On the other hand, Meson does not permit in-source builds, a separate build directory makes it clearer for checking the output.

A few simple commands can get done with building:
```shell
meson build
ninja -C build
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

# Replacement
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

# Conclusion
Xserver/Xorg is built in a flexible and easy way. We can implement our own device-dependent functionality and add it to Xserver as its another driver or module.
