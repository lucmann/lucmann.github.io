---
title: X Window System
date: 2020-12-20 11:58:43
tags: Window System
categories: graphics
---

最近在看[X11 Server的代码](https://gitlab.freedesktop.org/xorg/xserver),顺手记录一些基本概念和想法。X11 Server的代码很古老了，有的是1993年写的，目前的X Server实现[X.org](https://zh.wikipedia.org/wiki/X.Org_Server)是从最早的一个X Server实现[XFree86](https://zh.wikipedia.org/wiki/XFree86) 4.4 RC2版本(2004年)派生出来的。

<!--more-->

X11 Server编译后会生成若干动态链接库形式的驱动程序和一个可执行程序Xorg. Xorg本质上是一个服务器程序，像其它所有服务器程序一样，它的main函数里有一个while循环，以保证在程序启动后，到程序退出前能够为客户端持续提供服务。Xorg的另一个特点是极具扩展性，所有动态链接库形式的DDX(Device Dependent X)驱动都是以Xorg Module的方式动态加载的，具体加载哪个DDX Driver由Xorg的Config文件配置。

# 同一个使用glfw的OpenGL程序在Kylin V10系统上可以修改alpha通道改变窗口背景的透明度，而在Ubuntu Focal Fossa系统上却不能？

Alpha通道控制窗口的透明度(transparency)。而上X11系统上，alpha通道是否可用，和X11 Server的RENDER扩展有关，下面来自glfw的代码是自明的(self-explanatory)

```
GLFWbool _glfwIsVisualTransparentX11(Visual* visual)
{
    if (!_glfw.x11.xrender.available)
        return GLFW_FALSE;

    XRenderPictFormat* pf = XRenderFindVisualFormat(_glfw.x11.display, visual);
    return pf && pf->direct.alphaMask;
}
```

关于系统上的X11 Server的RENDER扩展的信息可以通过`xdpyinfo`命令查看(此命令输出很多), 而且RENDER扩展大多数情况下是可用的

```
xdpyinfo -ext RENDER
```

如果你的系统上的RENDER扩展可用，接下来的问题就简单了，你只需要查看一下你的程序(或者glfw)选取的VISUAL的**pict format**的`alphaMask`是否为`0`。如果是`0`, 则意味着glfw从你的系统获取的`GLFW_TRANSPARENT_FRAMEBUFFER`属性是`FALSE`, 也即在glfw创建的窗口中，你无法通过修改alpha通道来改变任何透明度。

```
RENDER version 0.11 opcode: 138, base error: 140
  Render formats :
  pict format:
        format id:    0x2c
        type:         Direct
        depth:        24
        alpha:         0 mask 0x0
        red:          16 mask 0xff
        green:         8 mask 0xff
        blue:          0 mask 0xff
```

很不幸，我的系统上(Ubuntu Focal Fossa)绝大部分VISUAL对应的都是这个ID为`0x2c`的**pict format**, 所以我不能通过修改alpha通道改变窗口背景或内容的透明度，除非我强制让glfw选择其它的VISUAL(对应的**pict format**不是`0x2c`)。
