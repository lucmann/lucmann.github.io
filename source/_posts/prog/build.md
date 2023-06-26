---
title: 关于 Linux 上的程序构建
date: 2022-09-18 17:02:49
tags: build
categories: programming
---

# 编译器

- gcc 
- g++ (GNU, latest version 12 ~ 13)
- clang
- clang++ (LLVM, latest version 16 ~ 17)

<!--more-->

# 链接器

- bfd (使用 Binary File Descriptor 库构建的 Linker)
- gold (Google linker, faster than bfd, but fewer features)
- mold ([A modern linker](https://github.com/rui314/mold))

动态链接库 (shared library) 无处不在。使用动态链接库基本上是通过链接器 (linker, generally a program suffixing with "ld")。本文主要回答以下问题:

- 有哪些链接器?
- 如何查看一个可执行文件链接哪些动态库?
- 链接器从哪里找到动态库?

# `pkg-config` vs `ldconfig`

## pkg-config

`dpkg-deb --contents /var/cache/apt/archives/pkg-config_0.29.1-0ubuntu4_amd64.deb`

```
drwxr-xr-x root/root         0 2020-02-07 17:04 ./
drwxr-xr-x root/root         0 2020-02-07 17:04 ./etc/
drwxr-xr-x root/root         0 2020-02-07 17:04 ./etc/dpkg/
drwxr-xr-x root/root         0 2020-02-07 17:04 ./etc/dpkg/dpkg.cfg.d/
-rw-r--r-- root/root       204 2020-02-07 17:04 ./etc/dpkg/dpkg.cfg.d/pkg-config-hook-config
drwxr-xr-x root/root         0 2020-02-07 17:04 ./usr/
drwxr-xr-x root/root         0 2020-02-07 17:04 ./usr/bin/
-rwxr-xr-x root/root     55552 2020-02-07 17:04 ./usr/bin/pkg-config
hrwxr-xr-x root/root         0 2020-02-07 17:04 ./usr/bin/x86_64-pc-linux-gnu-pkg-config link to ./usr/bin/pkg-config
drwxr-xr-x root/root         0 2020-02-07 17:04 ./usr/lib/
-rw-r--r-- root/root        17 2020-02-07 17:04 ./usr/lib/pkg-config.multiarch
drwxr-xr-x root/root         0 2020-02-07 17:04 ./usr/lib/pkgconfig/
drwxr-xr-x root/root         0 2020-02-07 17:04 ./usr/share/
drwxr-xr-x root/root         0 2020-02-07 17:04 ./usr/share/aclocal/
-rw-r--r-- root/root     10249 2020-02-07 17:04 ./usr/share/aclocal/pkg.m4
drwxr-xr-x root/root         0 2020-02-07 17:04 ./usr/share/doc/
drwxr-xr-x root/root         0 2020-02-07 17:04 ./usr/share/doc/pkg-config/
-rw-r--r-- root/root       359 2014-02-09 01:57 ./usr/share/doc/pkg-config/AUTHORS
-rw-r--r-- root/root      5320 2016-03-02 03:40 ./usr/share/doc/pkg-config/NEWS.gz
-rw-r--r-- root/root      2599 2014-02-09 01:57 ./usr/share/doc/pkg-config/README
-rw-r--r-- root/root      1376 2020-02-07 17:04 ./usr/share/doc/pkg-config/changelog.Debian.gz
-rw-r--r-- root/root       772 2020-02-07 17:04 ./usr/share/doc/pkg-config/copyright
-rw-r--r-- root/root     17832 2020-02-07 17:04 ./usr/share/doc/pkg-config/pkg-config-guide.html
drwxr-xr-x root/root         0 2020-02-07 17:04 ./usr/share/man/
drwxr-xr-x root/root         0 2020-02-07 17:04 ./usr/share/man/man1/
-rw-r--r-- root/root      7556 2020-02-07 17:04 ./usr/share/man/man1/pkg-config.1.gz
-rwxr-xr-x root/root      2235 2020-02-07 17:04 ./usr/share/pkg-config-crosswrapper
-rwxr-xr-x root/root      1826 2020-02-07 17:04 ./usr/share/pkg-config-dpkghook
drwxr-xr-x root/root         0 2020-02-07 17:04 ./usr/share/pkgconfig/
```

# `lib*-dev` 与 `lib*` 的区别

```
$ dpkg -L libxcb1
/.
/usr
/usr/lib
/usr/lib/x86_64-linux-gnu
/usr/lib/x86_64-linux-gnu/libxcb.so.1.1.0
/usr/share
/usr/share/doc
/usr/share/doc/libxcb1
/usr/share/doc/libxcb1/changelog.Debian.gz
/usr/share/doc/libxcb1/copyright
/usr/lib/x86_64-linux-gnu/libxcb.so.1
```

```
$ dpkg -L libxcb1-dev
/.
/usr
/usr/include
/usr/include/xcb
/usr/include/xcb/bigreq.h
/usr/include/xcb/xc_misc.h
/usr/include/xcb/xcb.h
/usr/include/xcb/xcbext.h
/usr/include/xcb/xproto.h
/usr/lib
/usr/lib/x86_64-linux-gnu
/usr/lib/x86_64-linux-gnu/libxcb.a
/usr/lib/x86_64-linux-gnu/pkgconfig
/usr/lib/x86_64-linux-gnu/pkgconfig/xcb.pc
/usr/share
/usr/share/doc
/usr/share/doc/libxcb1-dev
/usr/share/doc/libxcb1-dev/copyright
/usr/lib/x86_64-linux-gnu/libxcb.so
/usr/share/doc/libxcb1-dev/changelog.Debian.gz
```

## `/usr/sbin/ldconfig`

Configure Dynamic Linker Run Time Bindings

```sh
#!/bin/sh

if  test $# = 0							\
    && test x"$LDCONFIG_NOTRIGGER" = x				\
 && test x"$DPKG_MAINTSCRIPT_PACKAGE" != x			\
 && dpkg-trigger --check-supported 2>/dev/null
then
	if dpkg-trigger --no-await ldconfig; then
		if test x"$LDCONFIG_TRIGGER_DEBUG" != x; then
			echo "ldconfig: wrapper deferring update (trigger activated)"
		fi
		exit 0
	fi	
fi

exec /sbin/ldconfig.real "$@"
```

# build system

## autotools

最近在 Ubuntu 上构建一个软件时遇到以下问题:

```bash
pkg-config --cflags egl
# Nothing
```

<!--more-->

打开 `pkg-config` 的调试模式

```bash
pkg-config --cflags egl --debug
  line>prefix=/usr
 Variable declaration, 'prefix' has value '/usr'
  line>libdir=${prefix}/lib/x86_64-linux-gnu
 Variable declaration, 'libdir' has value '/usr/lib/x86_64-linux-gnu'
  line>includedir=${prefix}/include
 Variable declaration, 'includedir' has value '/usr/include'
  line>
  line>Name: EGL
  line>Description: EGL library and headers
  line>Version: 1.5
  line>Libs: -L${libdir} -lEGL
  line>Cflags: -I${includedir}
Path position of 'egl' is 4
Adding 'egl' to list of known packages
Package egl has -I/usr/include in Cflags
Removing -I/usr/include from cflags for egl
Package egl has -L /usr/lib/x86_64-linux-gnu in Libs
Removing -L /usr/lib/x86_64-linux-gnu from libs for egl
  pre-remove: egl
 post-remove: egl
adding CFLAGS_OTHER string ""
  pre-remove: egl
 post-remove: egl
 original: egl
   sorted: egl
adding CFLAGS_I string ""
returning flags string ""
```

其中的 `Package egl has ...` 和 `Removing ... from cflags for ...` 让我十分纳闷。所以想从 [`pkg-config` 的源码](https://gitlab.freedesktop.org/pkg-config/pkg-config)一探究竟。当我打开它时，发现它的构建系统还是“古老的” AutoTools.

```
➜  pg-config git:(master) ✗ ./autogen.sh
+ PROJECT=pkg-config
+ dirname ./autogen.sh
+ srcdir=.
+ test -z .
+ pwd
+ ORIGDIR=/home/luc/gh/pkg-config
+ cd .
+ autoreconf -iv
./autogen.sh: 13: autoreconf: not found
+ exit 127
```

### autotools 包含什么

- autoconf
- automake
- libtool
- gettext

## meson

### use `gold`

```bash
meson build --prefix=/usr -D{c,cpp}_args=-fuse-ld=gold -Dflavors=x11-gl,x11-glesv2
```

## cmake

### use `gold`

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=On -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=gold
```

# 编译 mold

``` under WSL2 Ubuntu 20.04 hosted Windows 11 Dell OptiPlex 3090
$ cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_EXPORT_COMPILE_COMMANDS=On
$ time cmake --build build -j 8
...
[100%] Linking CXX executable mold
[100%] Built target mold
cmake --build build -j 8  844.42s user 94.17s system 674% cpu 2:19.20 total
```

# 编译 mesa

```
➜  forked git:(23.1.3) meson build -Dprefix=/usr/local -Dbuildtype=debug -Dplatforms=x11 -Dvulkan-drivers=swrast -Dgallium-drivers=swrast,radeonsi,panfrost -Dglx=dri -Dllvm=enabled -Dcpp_rtti=false
➜  forked git:(23.1.3) time ninja -C build
ninja: Entering directory `build'
[1559/1559] Generating src/gallium/targets/dri/devenv_panfrost_dri.so with a custom command
ninja -C build  1214.38s user 240.13s system 710% cpu 3:24.62 total
```

```
➜  forked git:(23.1.3) ✗ CC=clang CC_LD=mold meson build -Dprefix=/usr/local -Dbuildtype=debug -Dplatforms=x11 -Dvulkan-drivers=swrast -Dgallium-drivers=swrast,radeonsi,panfrost -Dglx=dri -Dllvm=enabled -Dcpp_rtti=false
➜  forked git:(23.1.3) ✗ time ninja -C build
2 warnings generated.
[1559/1559] Generating src/gallium/targets/dri/devenv_mcde_dri.so with a custom command
ninja -C build  12971.24s user 158.32s system 1146% cpu 19:04.81 total
```
