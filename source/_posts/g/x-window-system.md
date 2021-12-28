---
title: X Window System
date: 2020-12-20 11:58:43
tags: [xorg]
categories: graphics
---

最近在看[X11 Server的代码](https://gitlab.freedesktop.org/xorg/xserver),顺手记录一些基本概念和想法。X11 Server的代码很古老了，有的是1993年写的，目前的X Server实现[X.org](https://zh.wikipedia.org/wiki/X.Org_Server)是从最早的一个X Server实现[XFree86](https://zh.wikipedia.org/wiki/XFree86) 4.4 RC2版本(2004年)派生出来的。

<!--more-->

X11 Server编译后会生成若干动态链接库形式的驱动程序和一个可执行程序Xorg. Xorg本质上是一个服务器程序，像其它所有服务器程序一样，它的main函数里有一个while循环，以保证在程序启动后，到程序退出前能够为客户端持续提供服务。Xorg的另一个特点是极具扩展性，所有动态链接库形式的DDX(Device Dependent X)驱动都是以Xorg Module的方式动态加载的，具体加载哪个DDX Driver由Xorg的Config文件配置。
