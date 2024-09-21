---
title: X C Bindings 
date: 2024-09-21 15:35:43
tags: [X11]
categories: graphics
---

XCB 是应用与 X 服务器交互使用的 C 绑定函数集，它里面的一些 C 函数是通过 python3-xcbgen 工具生成的，不经过编译在它的[源码库](https://gitlab.freedesktop.org/xorg/lib/libxcb)是找不到的。而且 XCB 古老到仍然使用 autotools 那套构建系统，想要看到某些函数的“真容”，你还得“千呼万唤”。

<!--more-->

- 安装工具
    - `sudo apt install python3-xcbgen autoconf automake libtool xutils-dev xcb-proto`
    - **xutils-dev** 里包含 xorg-macros
    - **libxcb** 的版本与 ***xcb-proto** 软件包的版本有依赖关系，如果你安装的 xcb-proto 版本是 1.14-2, 那么最好将 libxcb 的 tag 也检出到 libxcb-1.14

- 构建
    - `./autogen.sh`
    - `make`
- 千呼万唤始出来
    - 在构建完之后，为了保存“成果”，需要使用一点 git 小技巧, 在 `src/.gitignore` 最后加上
        - `!*.c`
    - 现在可以好好看看 `xcb_dri3_pixmap_from_buffers()` 的实现了
        - [https://gitlab.freedesktop.org/lucmann/libxcb/-/tree/v1.14-where-you-will-find-xcb_dri3_pixmap_from_buffers](https://gitlab.freedesktop.org/lucmann/libxcb/-/tree/v1.14-where-you-will-find-xcb_dri3_pixmap_from_buffers)