---
title: Build Software on Linux
date: 2022-06-15 17:02:49
tags: build
categories: programming
---

动态链接库 (shared library) 无处不在。使用动态链接库基本上是通过链接器 (linker, generally a program suffixing with "ld")。本文主要回答以下问题:

<!--more-->

- 有哪些链接器?
- 如何查看一个可执行文件链接哪些动态库?
- 链接器从哪里找到动态库?

# `pkg-config` vs `ldconfig`

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
