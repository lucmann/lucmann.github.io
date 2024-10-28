---
title: perfetto
date: 2024-08-26 15:36:16+08:00
tags: perf
categories: linux
---

# [Perfetto](https://perfetto.dev/)

Perfetto 是一个用于 Linux 和 Android 系统上进行系统级和应用级的跟踪和性能分析的开源工具。Perfetto 捆绑了许多数据源，这些数据源能够从不同的系统接口收集详细的性能数据，包括 ftrace, /proc, /sys, native heap profiling 等。Perfetto 还提供了一个基于 Web 的用户界面，用于跟踪数据的可视化和分析。这里主要是对第一次使用 perfetto 做一下记录，方便以后继续学习和整理。

![perfetto](/images/perfetto/perfetto.png)

<!--more-->

# Perfetto in Mesa

第一次使用 Perfetto 是从 Mesa 开始的，自从 Mesa 有了 Perfetto 的支持，就一直想看看 Perfetto 是怎么玩的，正好最近想比较一下 Zink + LAVAPIPE 和直接的 LLVMPIPE 两者的性能差异，就在我的 WSL2 Ubuntu 23.04 上折腾了一番。下面有几点基本情况下说明一下:

- Perfetto 在 Mesa 仓库里是以一个 submodule 存在的，而且构建选项默认是关闭的 (-Dperfetto=false)
- 启动 perfetto 时需要配置文件，Mesa 仓库也有好几个[配置文件](https://gitlab.freedesktop.org/mesa/mesa/-/tree/main/src/tool/pps/cfg)
- perfetto 在 Linux 的主要数据源是 ftrace, /proc, /sys 这些，因为我是在 WSL2 上， 所以 tracefs 默认是没有挂载的，需要手动挂载一下
    - `sudo mount -t tracefs tracefs /sys/kernel/tracing`
    - WSL2 的内核有些东西是不启用的，还好 `CONFIG_FTRACE=y`, `CONFIG_TRACING=y`, 这就省得重新编译内核了
- perfetto 仓库自己提供了一些用于构建的 Python 脚本，只需按照它的 [quickstart guide](https://perfetto.dev/docs/quickstart/linux-tracing) 构建就行了，但有一点很怪，perfetto 要用 google 自己的 clang 工具链(它的构建脚本里并没有给用户提供使用系统已有的 Clang 工具链的选项)

实际上从构建到生成 `/tmp/trace.perfetto-trace` (上传到[ui.perfetto.dev](https://ui.perfetto.dev/) 即可)，关键步骤有以下几个:

- `cd mesa/subproject/perfetto && ./tools/install-build-deps`
    - 不要带 --no-toolchain, 一开始下载慢，环境上也有 clang, 就想不下载 clang, 结果发现不行
- `./tools/setup_all_configs.py --host-only`
    - 会在当前目录的 out 目录下生成多个目录，下一步指定给 `ninja -C`
- `ninja -C out/linux_clang_release`
    - 如果顺利的话，perfetto 就构建完成了
- `CONFIG=../../src/tool/pps/cfg/system.cfg OUT=out/linux_clang_release ./tools/tmux -n `
    - 配置文件可以通过 CONFIG 环境变量指定, 这里的我用 Mesa 自带的 system.cfg。这里的 tmux 是一个 shell 脚本，它会启一个 tmux 会话，你在另外一个终端启动你的应用程序后，切回这个会话 Enter, perfetto 就开始记录数据了，一般有一个默认时长，当记录结束后，你只需 `Ctrl-B D` 关闭这个会话，记录的数据会自动保存在 `/tmp/trace.perfetto-trace` 文件中。

从第一次使用 perfetto 的情况，感觉它比较依赖 KMD 的实现，像 GPU 的性能计数器，只有内核驱动实现了，才能通过 perfetto 收集到，不过 perfetto 提供的库和 UI 确实简化了性能数据采集的过程和分析的难度。

