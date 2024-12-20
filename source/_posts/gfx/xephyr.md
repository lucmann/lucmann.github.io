---
title: Xephyr
date: 2024-12-20 15:35:43
tags: [X11]
categories: graphics
---

[Xephyr /'zefə/](https://gitlab.freedesktop.org/xorg/xserver/-/blob/master/hw/kdrive/ephyr/README?ref_type=heads) 是一个嵌套的 X Server, 作为 X 应用程序运行。

<!--more-->

Xephyr 比 Xnest 更强大， 因为 Xnest 只是 Host X Server 的一个代理，而 Xephyr 是一个真正的 X Server, 它使用 Host X Server 的一个窗口作为它的 "framebuffer"。Xephyr 对调试与 X Server 交互的应用程序非常有用。

# 参考

- [维基百科 - Xephyr](https://en.wikipedia.org/wiki/Xephyr)
- [Archlinux Wiki - Xephyr](https://wiki.archlinuxcn.org/wiki/Xephyr)
- [Xephyr 介绍与使用](https://blog.csdn.net/weixin_56291477/article/details/131856800)
