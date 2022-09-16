---
title: 温故而知新: GNU AutoTools
date: 2022-09-16 07:42:04
tags: [build]
categories: utilities
---

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

# AutoTools 包含什么

- autoconf
- automake
- libtool
- gettext
