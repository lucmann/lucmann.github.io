---
title: LLVM
date: 2022-09-09 15:58:26
tags: LLVM
categories: programming
---

# LLVM & llvm-project

LLVM 是 Low-Level Virtual Machine 的简写，但事实上它与虚拟机关系不大。我们更熟悉它是一套工具链，包括 clang `/'klæŋ/`, lld, lldb 等等。接触 LLVM 是因为 Mesa llvmpipe 使用 LLVM, 还有 AMDGPU 和 Radeon 的编译器后端都使用 LLVM IR，所以要编译 Mesa 的 `-Dgallium-drivers=llvmpipe,radeonsi` 都依赖于 LLVM 的诸多组件, 构建 Linux 内核的 [eBPF](https://massoudasadiblog.blogspot.com/2024/07/ebpf-on-wsl2-kernel-version-6x-ubuntu.html) 程序也依赖 LLVM, 构建 [perfetto](https://perfetto.dev/docs/quickstart/linux-tracing#building-from-source) 也依赖 LLVM。这里主要记录 LLVM 的构建和使用的一些问题。

<!--more-->

llvm-project 是 2003 年开源的。2022 年初，llvm-project 的源码库和 bug tracker 被移到了 [GitHub](https://github.com/llvm/llvm-project). 一开始，LLVM 的社区交流和项目沟通主要方式是 Mailing Lists 和 [IRC](https://lucmann.github.io/misc/english/), 在 2019 年，LLVM 社区转向 [Discourse](https://www.discourse.org/) 这个开源社区交流平台。

# Build LLVM

- `cmake -S llvm -B build`
    - llvm 是 llvm-project 根目录下的一个子目录，是我们的编译对象，像 lld, 还有 c++ 运行时库这里不需要构建
- `-G "Ninja"`
    - 使用 ninja 构建系统
- `-DCMAKE_BUILD_TYPE=Debug`
    - 默认是 `-DCMAKE_BUILD_TYPE=Release`
- `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`
    - 生成 compile_commands.json 索引数据库(导入 IDE)
- `-DCMAKE_INSTALL_PREFIX=~/.local/llvm`
    - 自定义安装路径，默认是 /usr/local
    - 安装后可通过 `llvm-config --ldflags` 获取
- `-DBUILD_SHARED_LIBS=ON`
    - `BUILD_SHARED_LIBS` 是一个 CMake 选项，当它开启时，那些没有指明 STATIC, SHARED, MODULE 的构建目标都会被构建成 SHARED 库。这个一般要开启，否则会导致在链接生成 llvm-lto2 时(因为链接的都是静态库) 内存压力巨大，很容易被 OOM-Killed 
- `-DLLVM_BUILD_LLVM_DYLIB=OFF`
    - 如果 ON，将所有 LLVM 组件生成一个单一的共享库 libLLVM, 适用于将 LLVM 嵌入到其它工具中的情况
- `-DLLVM_ENABLE_PROJECTS="clang;lld"`
    - 只构建 clang 和 lld (LLVM Linker) 子项目
- `-DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi"`
    - 一般不配置
- `-DLLVM_TARGETS_TO_BUILD="BPF;AMDGPU;host"`
    - 指定支持哪些后端(ISA), 这里如果是为了编译 Mesa Radeon/AMDGPU, `AMDGPU` target 必须指定。`host`是指当前编译机器。另外指定的越多，编译越耗时
    - 安装后可通过 `llvm-config --targets-built` 获取
- `-DLLVM_PARALLEL_COMPILE_JOBS=1 -DLLVM_PARALLEL_LINK_JOBS=1`
    - 在编译机配置不高的时候，减小 `cc1plus` 和 `ld` 被 OOM-Killed 的风险
- `-DLLVM_USE_LINKER=gold`
    - 将默认链接器 `ld` 换成 `gold`(可能会链接得快一点)
    - 实际上一般 Linux 系统 (e.g. Ubuntu 20.04), 默认安装有两个链接器
    ```
    lrwxrwxrwx 1   19 Oct 20  2021 /usr/bin/ld -> x86_64-linux-gnu-ld
    lrwxrwxrwx 1   23 Oct 20  2021 /usr/bin/ld.bfd -> x86_64-linux-gnu-ld.bfd
    lrwxrwxrwx 1   24 Oct 20  2021 /usr/bin/ld.gold -> x86_64-linux-gnu-ld.gold
    ```
    - `-DLLVM_USE_LINKER=bfd`
    ![ld-liblto_plugin.so](ld-liblto_plugin.so.gif)

安装后，我们需要将 `bin` 目录加入 `PATH`, 并创建 `/etc/ld.so.conf.d/llvm.conf` 包含下面一行

```
/home/luc/.local/llvm/lib64
```

# Building llvmpipe

```
llvm-config found: YES (/home/luc/.local/llvm/bin/llvm-config) 15.0.0
Run-time dependency LLVM (modules: bitwriter, core, engine, executionengine, instcombine, mcdisassembler, mcjit, native, scalaropts, transformutils, coroutines) found: YES 15.0.0
Run-time dependency valgrind found: NO (tried pkgconfig)
Program bison found: YES (/usr/bin/bison)
Program bison found: YES (/usr/bin/bison)
Program flex found: YES (/usr/bin/flex)
Run-time dependency libunwind found: NO (tried pkgconfig and cmake)
Run-time dependency OpenMP found: YES 4.5
Run-time dependency x11 found: YES 1.6.9
Run-time dependency xext found: YES 1.3.4
Run-time dependency xfixes found: YES 5.0.3
Run-time dependency xcb-glx found: NO (tried pkgconfig and cmake)
Message: Configuration summary:

        prefix:          /usr
        libdir:          lib/x86_64-linux-gnu
        includedir:      include

        OpenGL:          yes (ES1: yes ES2: yes)
        OSMesa:          no

        DRI platform:    drm
        DRI drivers:     no
        DRI driver dir:  /usr/lib/x86_64-linux-gnu/dri

        GLX:             DRI-based

        EGL:             yes
        EGL drivers:     builtin:egl_dri2 builtin:egl_dri3
        EGL/Vulkan/VL platforms:   x11 surfaceless drm xcb
        GBM:             yes
        GBM backends path: /usr/lib/x86_64-linux-gnu/gbm

        Vulkan drivers:  swrast
        Vulkan ICD dir:  share/vulkan/icd.d

        llvm:            yes
        llvm-version:    15.0.0

        Gallium drivers: swrast
        Gallium st:      mesa
        HUD lmsensors:   no

        Shared-glapi:    yes

        Perfetto:        no
        Perfetto ds:     auto

Build targets in project: 174
```

当编译 Mesa 时，遇到的第一个编译错误是 `No such file or directory`

```
../src/gallium/auxiliary/gallivm/lp_bld_init.c:52:10: fatal error: llvm-c/Transforms/Coroutines.h: No such file or directory                                                                                                                       52 | #include <llvm-c/Transforms/Coroutines.h>                                                                             |          ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                                                                       compilation terminated.
```

这个问题似乎不好解决，我转向构建 LLVM 14.0.6

```bash
➜  llvm-project git:(14.0.6) cmake -S llvm -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=On -DLLVM_ENABLE_PROJECTS="clang;lld" -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi" -DCMAKE_INSTALL_PREFIX=~/.local/llvm-14 -DLLVM_LIBDIR_SUFFIX="64" -DLLVM_TARGETS_TO_BUILD="host" -DLLVM_BUILD_LLVM_DYLIB=On -DBUILD_SHARED_LIBS=On -DLLVM_PARALLEL_COMPILE_JOBS=1 -DLLVM_PARALLEL_LINK_JOBS=1 -DLLVM_USE_LINKER=gold
```

但又遇到下面的错误

```
CMake Error at /home/luc/gh/llvm-project/libcxx/CMakeLists.txt:880 (message):                                             LIBCXX_ABI_NAMESPACE '__1' is reserved for use by libc++.                                                                                                                                                                                                                                                                                                             -- Configuring incomplete, errors occurred!
```

查看 `libcxx/CMakeLists.txt` 发现 `LIBCXX_ABI_NAMESPACE` 是一个 CMake 变量, 而且被缓存了，难怪第一次构建时没有报这个错误，所以在编译新的 LLVM 之前一次记得将原来的构建目录删除。

```
set(LIBCXX_ABI_NAMESPACE "" CACHE STRING "The inline ABI namespace used by libc++. It defaults to __n where `n` is the  current ABI version.")
```
