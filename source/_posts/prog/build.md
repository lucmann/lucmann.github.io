---
title: Build your program from source code on Linux
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

## gcc 的优化级别

-O 和 -O1 是等价的。 -O 会对目标文件大小和执行时间进行优化，但不会进行非常耗时的优化，以下是 -O 打开的优化: (其中红色的是 -Og 关闭的)

- -fauto-inc-dec
- <span style="color:red">-fbranch-count-reg</span>
- -fcombine-stack-adjustments
- -fcompare-elim
- -fcprop-registers
- -fdce
- -fdefer-pop
- <span style="color:red">-fdelayed-branch</span>
- <span style="color:red">-fdse</span>
- -fforward-propagate
- -fguess-branch-probability
- <span style="color:red">-fif-conversion</span>
- <span style="color:red">-fif-conversion2</span>
- <span style="color:red">-finline-functions-called-once</span>
- -fipa-modref
- -fipa-profile
- -fipa-reference
- -fipa-reference-addressable
- -fmerge-constants
- <span style="color:red">-fmove-loop-invariants</span>
- <span style="color:red">-fmove-loop-stores</span>
- -fomit-frame-pointer
- -freorder-blocks
- -fshrink-wrap
- -fshrink-wrap-separate
- -fsplit-wide-types
- -fssa-backprop
- <span style="color:red">-fssa-phiopt</span>
- <span style="color:red">-ftree-bit-ccp</span>
- -ftree-ccp
- -ftree-ch
- -ftree-coalesce-vars
- -ftree-copy-prop
- -ftree-dce
- -ftree-dominator-opts
- <span style="color:red">-ftree-dse</span>
- -ftree-forwprop
- -ftree-fre
- -ftree-phiprop
- <span style="color:red">-ftree-pta</span>
- -ftree-scev-cprop
- -ftree-sink
- -ftree-slsr
- <span style="color:red">-ftree-sra</span>
- -ftree-ter
- -funit-at-a-time


# 链接器

- bfd (使用 Binary File Descriptor 库构建的 Linker)
- gold (Google linker, faster than bfd, but fewer features)
- mold ([A modern linker](https://github.com/rui314/mold))

动态链接库 (shared library) 无处不在。使用动态链接库基本上是通过链接器 (linker, generally a program suffixing with "ld")。本文主要回答以下问题:

- 有哪些链接器?
- 如何查看一个可执行文件链接哪些动态库?
- 链接器从哪里找到动态库?
- GCC 编译器不支持 `CXX_SUPPORTS_CUSTOM_LINKER`
  - 所以安装 mold 后，如果想在编译 LLVM 时使用 `-DLLVM_USE_LINKER=mold`, 就得把 `CC=clang CXX=clang++` 先设置了

# `pkg-config` vs `ldconfig`

## pkg-config

`pkg-config` 是二进制可执行程序 `/usr/bin/pkgconf` 的一个符号链接文件，它是 CMake, meson 等构建系统主要使用的系统动态库检测的工具。`pkg-config` 本质上是在解析 `*.pc` 文件。下面是常见的 Mesa OpenGL Library 的 .pc 文件。

```
➜  piglit git:(main) ✗ find /usr -name 'gl.pc' -ls
    78674      4 -rw-r--r--   1 root     root          205 Jan  3  2023 /usr/lib/x86_64-linux-gnu/pkgconfig/gl.pc
    72985      4 -rw-r--r--   1 root     root          362 Jun  7 18:16 /usr/local/lib/x86_64-linux-gnu/pkgconfig/gl.pc
➜  piglit git:(main) ✗ bat /usr/local/lib/x86_64-linux-gnu/pkgconfig/gl.pc
───────┬────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
       │ File: /usr/local/lib/x86_64-linux-gnu/pkgconfig/gl.pc
───────┼────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
   1   │ prefix=/usr/local
   2   │ includedir=${prefix}/include
   3   │ libdir=${prefix}/lib/x86_64-linux-gnu
   4   │
   5   │ glx_tls=yes
   6   │
   7   │ Name: gl
   8   │ Description: Mesa OpenGL Library
   9   │ Version: 24.2.0-devel
  10   │ Requires.private: x11, xext, xfixes, x11-xcb, xcb, xcb-glx >=  1.8.1, xcb-dri2 >=  1.8, xxf86vm, libdrm >=  2.4.75
  11   │ Libs: -L${libdir} -lGL
  12   │ Libs.private: -lpthread -pthread -lm
  13   │ Cflags: -I${includedir}
───────┴────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
➜  piglit git:(main) ✗ bat /usr/lib/x86_64-linux-gnu/pkgconfig/gl.pc
───────┬────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
       │ File: /usr/lib/x86_64-linux-gnu/pkgconfig/gl.pc
───────┼────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
   1   │ prefix=/usr
   2   │ includedir=${prefix}/include
   3   │ libdir=${prefix}/lib/x86_64-linux-gnu
   4   │
   5   │ Name: GL
   6   │ Description: Legacy OpenGL and GLX library and headers.
   7   │ Version: 1.2
   8   │ Libs: -L${libdir} -lGL
   9   │ Cflags: -I${includedir}
───────┴────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
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

### [-D buildtype](https://mesonbuild.com/Builtin-options.html#details-for-buildtype)

meson 的 buildtype 是用来设定编译优化级别 (optimization levels: -O0, -O1, -O2, -O3, -Os) 和是否有调试信息 (debug: -g)。 实际上，meson 提供两个分开的选项分别控制编译优化级别和调试信息

- -Doptimization (plain|0|2|3|s, plain 指不设置任何 optimization flags)
- -Ddebug (true|false)

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

# 编译 LLVM

## 主要的配置

```
cmake -S llvm -B build -G Ninja                                     \
  -DCMAKE_C_COMPILER=gcc-10                                         \
  -DCMAKE_CXX_COMPILER=g++-10                                       \
  -DCMAKE_BUILD_TYPE=Release                                        \
  -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR"                             \
  -DLLVM_LIBDIR_SUFFIX="64"                                         \
  -DLLVM_TARGETS_TO_BUILD="host;AMDGPU"                             \
  -DLLVM_BUILD_LLVM_DYLIB="ON"                                      \
  -DBUILD_SHARED_LIBS="ON"                                          \
  -DLLVM_USE_LINKER="gold"                                          \
  -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra;lld"              \
  -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi;libunwind"               \
  -DLLVM_PARALLEL_COMPILE_JOBS="1"                                  \
  -DLLVM_PARALLEL_LINK_JOBS="1"
```

## OOM 问题

如果机器内存不够在编译 LLVM 的时候可能会触发 OOM Killer (我在 WSL2 下就100%触发). 可以通过临时添加 swap 分区的方法避免这个问题 （[Linux Add a Swap File Tutorial](https://www.cyberciti.biz/faq/linux-add-a-swap-file-howto/)）


