---
title: Kylin V10 窗口透明问题
date: 2020-12-20 11:58:43
tags: [X11]
categories: graphics
---

最近在看[X11 Server的代码](https://gitlab.freedesktop.org/xorg/xserver),顺手做下笔记。X11 Server的代码很古老了，有的是1993年写的，目前的X Server实现[X.org](https://zh.wikipedia.org/wiki/X.Org_Server)是从最早的一个X Server实现[XFree86](https://zh.wikipedia.org/wiki/XFree86) 4.4 RC2版本(2004年)派生出来的。

<!--more-->

X11 Server编译后会生成若干动态链接库形式的驱动程序和一个可执行程序Xorg. Xorg本质上是一个服务器程序，像其它所有服务器程序一样，它的main函数里有一个while循环，以保证在程序启动后，到程序退出前能够为客户端持续提供服务。Xorg的另一个特点是极具扩展性，所有动态链接库形式的DDX(Device Dependent X)驱动都是以Xorg Module的方式动态加载的，具体加载哪个DDX Driver由Xorg的Config文件配置。

# 同一个使用glfw的OpenGL程序在Kylin V10系统上可以修改alpha通道改变窗口背景的透明度，而在Ubuntu Focal Fossa系统上却不能？

Alpha通道控制窗口的透明度(transparency)。而X11窗口系统上，alpha通道是否可用，和X11 Server的RENDER扩展有关，下面来自glfw的代码是自明的(self-explanatory)

```
GLFWbool _glfwIsVisualTransparentX11(Visual* visual)
{
    if (!_glfw.x11.xrender.available)
        return GLFW_FALSE;

    XRenderPictFormat* pf = XRenderFindVisualFormat(_glfw.x11.display, visual);
    return pf && pf->direct.alphaMask;
}
```

关于系统上的 X11 Server 的 RENDER 扩展的信息可以通过 `xdpyinfo` 命令查看(此命令输出很多信息)

- Render formats

`xdpyinfo -ext RENDER | awk '/Render formats/,/Screen formats/'`

```
  Render formats :
  pict format:
	format id:    0x25
	type:         Direct
	depth:        1
	alpha:         0 mask 0x1
	red:           0 mask 0x0
	green:         0 mask 0x0
	blue:          0 mask 0x0
  pict format:
	format id:    0x26
	type:         Direct
	depth:        8
	alpha:         0 mask 0xff
	red:           0 mask 0x0
	green:         0 mask 0x0
	blue:          0 mask 0x0
  pict format:
	format id:    0x27
	type:         Direct
	depth:        4
	alpha:         0 mask 0xf
	red:           0 mask 0x0
	green:         0 mask 0x0
	blue:          0 mask 0x0
  pict format:
	format id:    0x28
	type:         Direct
	depth:        32
	alpha:        24 mask 0xff
	red:          16 mask 0xff
	green:         8 mask 0xff
	blue:          0 mask 0xff
  pict format:
	format id:    0x29
	type:         Direct
	depth:        32
	alpha:         0 mask 0x0
	red:          16 mask 0xff
	green:         8 mask 0xff
	blue:          0 mask 0xff
  pict format:
	format id:    0x2a
	type:         Direct
	depth:        32
	alpha:         0 mask 0xff
	red:           8 mask 0xff
	green:        16 mask 0xff
	blue:         24 mask 0xff
  pict format:
	format id:    0x2b
	type:         Direct
	depth:        32
	alpha:         0 mask 0x0
	red:           8 mask 0xff
	green:        16 mask 0xff
	blue:         24 mask 0xff
  pict format:
	format id:    0x2c
	type:         Direct
	depth:        24
	alpha:         0 mask 0x0
	red:          16 mask 0xff
	green:         8 mask 0xff
	blue:          0 mask 0xff
  pict format:
	format id:    0x2d
	type:         Direct
	depth:        24
	alpha:         0 mask 0x0
	red:           0 mask 0xff
	green:         8 mask 0xff
	blue:         16 mask 0xff
  pict format:
	format id:    0x2e
	type:         Direct
	depth:        15
	alpha:         0 mask 0x0
	red:           8 mask 0xf
	green:         4 mask 0xf
	blue:          0 mask 0xf
  pict format:
	format id:    0x2f
	type:         Direct
	depth:        15
	alpha:         0 mask 0x0
	red:           0 mask 0xf
	green:         4 mask 0xf
	blue:          8 mask 0xf
  pict format:
	format id:    0x30
	type:         Direct
	depth:        15
	alpha:         0 mask 0x0
	red:          10 mask 0x1f
	green:         5 mask 0x1f
	blue:          0 mask 0x1f
  pict format:
	format id:    0x31
	type:         Direct
	depth:        15
	alpha:         0 mask 0x0
	red:           0 mask 0x1f
	green:         5 mask 0x1f
	blue:         10 mask 0x1f
  pict format:
	format id:    0x32
	type:         Direct
	depth:        16
	alpha:         0 mask 0x0
	red:           8 mask 0xf
	green:         4 mask 0xf
	blue:          0 mask 0xf
  pict format:
	format id:    0x33
	type:         Direct
	depth:        16
	alpha:         0 mask 0x0
	red:           0 mask 0xf
	green:         4 mask 0xf
	blue:          8 mask 0xf
  pict format:
	format id:    0x34
	type:         Direct
	depth:        16
	alpha:         0 mask 0x0
	red:          10 mask 0x1f
	green:         5 mask 0x1f
	blue:          0 mask 0x1f
  pict format:
	format id:    0x35
	type:         Direct
	depth:        16
	alpha:         0 mask 0x0
	red:           0 mask 0x1f
	green:         5 mask 0x1f
	blue:         10 mask 0x1f
  pict format:
	format id:    0x36
	type:         Direct
	depth:        16
	alpha:        15 mask 0x1
	red:          10 mask 0x1f
	green:         5 mask 0x1f
	blue:          0 mask 0x1f
  pict format:
	format id:    0x37
	type:         Direct
	depth:        16
	alpha:        15 mask 0x1
	red:           0 mask 0x1f
	green:         5 mask 0x1f
	blue:         10 mask 0x1f
  pict format:
	format id:    0x38
	type:         Direct
	depth:        16
	alpha:         0 mask 0x0
	red:          11 mask 0x1f
	green:         5 mask 0x3f
	blue:          0 mask 0x1f
  pict format:
	format id:    0x39
	type:         Direct
	depth:        16
	alpha:         0 mask 0x0
	red:           0 mask 0x1f
	green:         5 mask 0x3f
	blue:         11 mask 0x1f
  pict format:
	format id:    0x3a
	type:         Direct
	depth:        16
	alpha:        12 mask 0xf
	red:           8 mask 0xf
	green:         4 mask 0xf
	blue:          0 mask 0xf
  pict format:
	format id:    0x3b
	type:         Direct
	depth:        16
	alpha:        12 mask 0xf
	red:           0 mask 0xf
	green:         4 mask 0xf
	blue:          8 mask 0xf
  pict format:
	format id:    0x3c
	type:         Direct
	depth:        32
	alpha:         0 mask 0x0
	red:           0 mask 0xff
	green:         8 mask 0xff
	blue:         16 mask 0xff
  pict format:
	format id:    0x3d
	type:         Direct
	depth:        32
	alpha:        30 mask 0x3
	red:          20 mask 0x3ff
	green:        10 mask 0x3ff
	blue:          0 mask 0x3ff
  pict format:
	format id:    0x3e
	type:         Direct
	depth:        32
	alpha:         0 mask 0x0
	red:          20 mask 0x3ff
	green:        10 mask 0x3ff
	blue:          0 mask 0x3ff
  pict format:
	format id:    0x3f
	type:         Direct
	depth:        32
	alpha:        30 mask 0x3
	red:           0 mask 0x3ff
	green:        10 mask 0x3ff
	blue:         20 mask 0x3ff
  pict format:
	format id:    0x40
	type:         Direct
	depth:        32
	alpha:         0 mask 0x0
	red:           0 mask 0x3ff
	green:        10 mask 0x3ff
	blue:         20 mask 0x3ff
  Screen formats :
```

- Screen formats

`xdpyinfo -ext RENDER | awk '/Screen formats/,/depth formats/'`

```
  Screen formats :
    Screen 0 (sub-pixel order Unknown)
      filters: nearest, bilinear, convolution, fast(nearest), good(bilinear), best(bilinear)
      visual format:
        visual id:      0x23
        pict format id: 0x2c
      visual format:
        visual id:      0x24
        pict format id: 0x2c
      visual format:
        visual id:      0x296
        pict format id: 0x2c
      visual format:
        visual id:      0x297
        pict format id: 0x2c
      visual format:
        visual id:      0x298
        pict format id: 0x2c
      visual format:
        visual id:      0x299
        pict format id: 0x2c
      visual format:
        visual id:      0x29a
        pict format id: 0x2c
      visual format:
        visual id:      0x29b
        pict format id: 0x2c
      visual format:
        visual id:      0x29c
        pict format id: 0x2c
      visual format:
        visual id:      0x29d
        pict format id: 0x2c
      visual format:
        visual id:      0x29e
        pict format id: 0x2c
      visual format:
        visual id:      0x29f
        pict format id: 0x2c
      visual format:
        visual id:      0x2a0
        pict format id: 0x2c
      visual format:
        visual id:      0x2a1
        pict format id: 0x2c
      visual format:
        visual id:      0x2a2
        pict format id: 0x2c
      visual format:
        visual id:      0x2a3
        pict format id: 0x2c
      visual format:
        visual id:      0x2a4
        pict format id: 0x2c
      visual format:
        visual id:      0x2a5
        pict format id: 0x2c
      visual format:
        visual id:      0x2a6
        pict format id: 0x2c
      visual format:
        visual id:      0x2a7
        pict format id: 0x2c
      visual format:
        visual id:      0x2a8
        pict format id: 0x2c
      visual format:
        visual id:      0x2a9
        pict format id: 0x2c
      visual format:
        visual id:      0x2aa
        pict format id: 0x2c
      visual format:
        visual id:      0x2ab
        pict format id: 0x2c
      visual format:
        visual id:      0x2ac
        pict format id: 0x2c
      visual format:
        visual id:      0x2ad
        pict format id: 0x2c
      visual format:
        visual id:      0x2ae
        pict format id: 0x2c
      visual format:
        visual id:      0x2af
        pict format id: 0x2c
      visual format:
        visual id:      0x2b0
        pict format id: 0x2c
      visual format:
        visual id:      0x2b1
        pict format id: 0x2c
      visual format:
        visual id:      0x2b2
        pict format id: 0x2c
      visual format:
        visual id:      0x2b3
        pict format id: 0x2c
      visual format:
        visual id:      0x2b4
        pict format id: 0x2c
      visual format:
        visual id:      0x2b5
        pict format id: 0x2c
      visual format:
        visual id:      0x2b6
        pict format id: 0x2c
      visual format:
        visual id:      0x2b7
        pict format id: 0x2c
      visual format:
        visual id:      0x2b8
        pict format id: 0x2c
      visual format:
        visual id:      0x2b9
        pict format id: 0x2c
      visual format:
        visual id:      0x2ba
        pict format id: 0x2c
      visual format:
        visual id:      0x2bb
        pict format id: 0x2c
      visual format:
        visual id:      0x2bc
        pict format id: 0x2c
      visual format:
        visual id:      0x2bd
        pict format id: 0x2c
      visual format:
        visual id:      0x2be
        pict format id: 0x2c
      visual format:
        visual id:      0x2bf
        pict format id: 0x2c
      visual format:
        visual id:      0x2c0
        pict format id: 0x2c
      visual format:
        visual id:      0x2c1
        pict format id: 0x2c
      visual format:
        visual id:      0x2c2
        pict format id: 0x2c
      visual format:
        visual id:      0x2c3
        pict format id: 0x2c
      visual format:
        visual id:      0x2c4
        pict format id: 0x2c
      visual format:
        visual id:      0x2c5
        pict format id: 0x2c
      visual format:
        visual id:      0x2c6
        pict format id: 0x2c
      visual format:
        visual id:      0x2c7
        pict format id: 0x2c
      visual format:
        visual id:      0x2c8
        pict format id: 0x2c
      visual format:
        visual id:      0x2c9
        pict format id: 0x2c
      visual format:
        visual id:      0x2ca
        pict format id: 0x2c
      visual format:
        visual id:      0x2cb
        pict format id: 0x2c
      visual format:
        visual id:      0x2cc
        pict format id: 0x2c
      visual format:
        visual id:      0x2cd
        pict format id: 0x2c
      visual format:
        visual id:      0x2ce
        pict format id: 0x2c
      visual format:
        visual id:      0x2cf
        pict format id: 0x2c
      visual format:
        visual id:      0x2d0
        pict format id: 0x2c
      visual format:
        visual id:      0x2d1
        pict format id: 0x2c
      visual format:
        visual id:      0x2d2
        pict format id: 0x2c
      visual format:
        visual id:      0x2d3
        pict format id: 0x2c
      visual format:
        visual id:      0x2d4
        pict format id: 0x2c
      visual format:
        visual id:      0x2d5
        pict format id: 0x2c
      visual format:
        visual id:      0x2d6
        pict format id: 0x2c
      visual format:
        visual id:      0x2d7
        pict format id: 0x2c
      visual format:
        visual id:      0x2d8
        pict format id: 0x2c
      visual format:
        visual id:      0x2d9
        pict format id: 0x2c
      visual format:
        visual id:      0x2da
        pict format id: 0x2c
      visual format:
        visual id:      0x2db
        pict format id: 0x2c
      visual format:
        visual id:      0x2dc
        pict format id: 0x2c
      visual format:
        visual id:      0x2dd
        pict format id: 0x2c
      visual format:
        visual id:      0x2de
        pict format id: 0x2c
      visual format:
        visual id:      0x2df
        pict format id: 0x2c
      visual format:
        visual id:      0x2e0
        pict format id: 0x2c
      visual format:
        visual id:      0x2e1
        pict format id: 0x2c
      visual format:
        visual id:      0x2e2
        pict format id: 0x2c
      visual format:
        visual id:      0x2e3
        pict format id: 0x2c
      visual format:
        visual id:      0x2e4
        pict format id: 0x2c
      visual format:
        visual id:      0x2e5
        pict format id: 0x2c
      visual format:
        visual id:      0x2e6
        pict format id: 0x2c
      visual format:
        visual id:      0x2e7
        pict format id: 0x2c
      visual format:
        visual id:      0x2e8
        pict format id: 0x2c
      visual format:
        visual id:      0x2e9
        pict format id: 0x2c
      visual format:
        visual id:      0x2ea
        pict format id: 0x2c
      visual format:
        visual id:      0x2eb
        pict format id: 0x2c
      visual format:
        visual id:      0x2ec
        pict format id: 0x2c
      visual format:
        visual id:      0x2ed
        pict format id: 0x2c
      visual format:
        visual id:      0x2ee
        pict format id: 0x2c
      visual format:
        visual id:      0x2ef
        pict format id: 0x2c
      visual format:
        visual id:      0x2f0
        pict format id: 0x2c
      visual format:
        visual id:      0x2f1
        pict format id: 0x2c
      visual format:
        visual id:      0x2f2
        pict format id: 0x2c
      visual format:
        visual id:      0x2f3
        pict format id: 0x2c
      visual format:
        visual id:      0x2f4
        pict format id: 0x2c
      visual format:
        visual id:      0x2f5
        pict format id: 0x2c
      visual format:
        visual id:      0x2f6
        pict format id: 0x2c
      visual format:
        visual id:      0x2f7
        pict format id: 0x2c
      visual format:
        visual id:      0x2f8
        pict format id: 0x2c
      visual format:
        visual id:      0x2f9
        pict format id: 0x2c
      visual format:
        visual id:      0x2fa
        pict format id: 0x2c
      visual format:
        visual id:      0x2fb
        pict format id: 0x2c
      visual format:
        visual id:      0x2fc
        pict format id: 0x2c
      visual format:
        visual id:      0x2fd
        pict format id: 0x2c
      visual format:
        visual id:      0x2fe
        pict format id: 0x2c
      visual format:
        visual id:      0x2ff
        pict format id: 0x2c
      visual format:
        visual id:      0x300
        pict format id: 0x2c
      visual format:
        visual id:      0x301
        pict format id: 0x2c
      visual format:
        visual id:      0x302
        pict format id: 0x2c
      visual format:
        visual id:      0x303
        pict format id: 0x2c
      visual format:
        visual id:      0x304
        pict format id: 0x2c
      visual format:
        visual id:      0x305
        pict format id: 0x2c
      visual format:
        visual id:      0x306
        pict format id: 0x2c
      visual format:
        visual id:      0x307
        pict format id: 0x2c
      visual format:
        visual id:      0x308
        pict format id: 0x2c
      visual format:
        visual id:      0x309
        pict format id: 0x2c
      visual format:
        visual id:      0x30a
        pict format id: 0x2c
      visual format:
        visual id:      0x30b
        pict format id: 0x2c
      visual format:
        visual id:      0x30c
        pict format id: 0x2c
      visual format:
        visual id:      0x30d
        pict format id: 0x2c
      visual format:
        visual id:      0x30e
        pict format id: 0x2c
      visual format:
        visual id:      0x30f
        pict format id: 0x2c
      visual format:
        visual id:      0x310
        pict format id: 0x2c
      visual format:
        visual id:      0x311
        pict format id: 0x2c
      visual format:
        visual id:      0x312
        pict format id: 0x2c
      visual format:
        visual id:      0x313
        pict format id: 0x2c
      visual format:
        visual id:      0x314
        pict format id: 0x2c
      visual format:
        visual id:      0x315
        pict format id: 0x2c
      visual format:
        visual id:      0x316
        pict format id: 0x2c
      visual format:
        visual id:      0x317
        pict format id: 0x2c
      visual format:
        visual id:      0x318
        pict format id: 0x2c
      visual format:
        visual id:      0x319
        pict format id: 0x2c
      visual format:
        visual id:      0x31a
        pict format id: 0x2c
      visual format:
        visual id:      0x31b
        pict format id: 0x2c
      visual format:
        visual id:      0x31c
        pict format id: 0x2c
      visual format:
        visual id:      0x31d
        pict format id: 0x2c
      visual format:
        visual id:      0x31e
        pict format id: 0x2c
      visual format:
        visual id:      0x31f
        pict format id: 0x2c
      visual format:
        visual id:      0x320
        pict format id: 0x2c
      visual format:
        visual id:      0x321
        pict format id: 0x2c
      visual format:
        visual id:      0x322
        pict format id: 0x2c
      visual format:
        visual id:      0x323
        pict format id: 0x2c
      visual format:
        visual id:      0x324
        pict format id: 0x2c
      visual format:
        visual id:      0x325
        pict format id: 0x2c
      visual format:
        visual id:      0x326
        pict format id: 0x2c
      visual format:
        visual id:      0x327
        pict format id: 0x2c
      visual format:
        visual id:      0x328
        pict format id: 0x2c
      visual format:
        visual id:      0x329
        pict format id: 0x2c
      visual format:
        visual id:      0x32a
        pict format id: 0x2c
      visual format:
        visual id:      0x32b
        pict format id: 0x2c
      visual format:
        visual id:      0x32c
        pict format id: 0x2c
      visual format:
        visual id:      0x32d
        pict format id: 0x2c
      visual format:
        visual id:      0x32e
        pict format id: 0x2c
      visual format:
        visual id:      0x32f
        pict format id: 0x2c
      visual format:
        visual id:      0x330
        pict format id: 0x2c
      visual format:
        visual id:      0x331
        pict format id: 0x2c
      visual format:
        visual id:      0x332
        pict format id: 0x2c
      visual format:
        visual id:      0x333
        pict format id: 0x2c
      visual format:
        visual id:      0x334
        pict format id: 0x2c
      visual format:
        visual id:      0x335
        pict format id: 0x2c
      visual format:
        visual id:      0x336
        pict format id: 0x2c
      visual format:
        visual id:      0x337
        pict format id: 0x2c
      visual format:
        visual id:      0x338
        pict format id: 0x2c
      visual format:
        visual id:      0x339
        pict format id: 0x2c
      visual format:
        visual id:      0x33a
        pict format id: 0x2c
      visual format:
        visual id:      0x33b
        pict format id: 0x2c
      visual format:
        visual id:      0x33c
        pict format id: 0x2c
      visual format:
        visual id:      0x33d
        pict format id: 0x2c
      visual format:
        visual id:      0x33e
        pict format id: 0x2c
      visual format:
        visual id:      0x33f
        pict format id: 0x2c
      visual format:
        visual id:      0x340
        pict format id: 0x2c
      visual format:
        visual id:      0x341
        pict format id: 0x2c
      visual format:
        visual id:      0x342
        pict format id: 0x2c
      visual format:
        visual id:      0x343
        pict format id: 0x2c
      visual format:
        visual id:      0x344
        pict format id: 0x2c
      visual format:
        visual id:      0x345
        pict format id: 0x2c
      visual format:
        visual id:      0x346
        pict format id: 0x2c
      visual format:
        visual id:      0x347
        pict format id: 0x2c
      visual format:
        visual id:      0x348
        pict format id: 0x2c
      visual format:
        visual id:      0x349
        pict format id: 0x2c
      visual format:
        visual id:      0x34a
        pict format id: 0x2c
      visual format:
        visual id:      0x34b
        pict format id: 0x2c
      visual format:
        visual id:      0x34c
        pict format id: 0x2c
      visual format:
        visual id:      0x34d
        pict format id: 0x2c
      visual format:
        visual id:      0x34e
        pict format id: 0x2c
      visual format:
        visual id:      0x34f
        pict format id: 0x2c
      visual format:
        visual id:      0x350
        pict format id: 0x2c
      visual format:
        visual id:      0x351
        pict format id: 0x2c
      visual format:
        visual id:      0x352
        pict format id: 0x2c
      visual format:
        visual id:      0x353
        pict format id: 0x2c
      visual format:
        visual id:      0x354
        pict format id: 0x2c
      visual format:
        visual id:      0x355
        pict format id: 0x2c
      visual format:
        visual id:      0x356
        pict format id: 0x2c
      visual format:
        visual id:      0x357
        pict format id: 0x2c
      visual format:
        visual id:      0x358
        pict format id: 0x2c
      visual format:
        visual id:      0x359
        pict format id: 0x2c
      visual format:
        visual id:      0x35a
        pict format id: 0x2c
      visual format:
        visual id:      0x35b
        pict format id: 0x2c
      visual format:
        visual id:      0x35c
        pict format id: 0x2c
      visual format:
        visual id:      0x35d
        pict format id: 0x2c
      visual format:
        visual id:      0x35e
        pict format id: 0x2c
      visual format:
        visual id:      0x35f
        pict format id: 0x2c
      visual format:
        visual id:      0x360
        pict format id: 0x2c
      visual format:
        visual id:      0x361
        pict format id: 0x2c
      visual format:
        visual id:      0x362
        pict format id: 0x2c
      visual format:
        visual id:      0x363
        pict format id: 0x2c
      visual format:
        visual id:      0x364
        pict format id: 0x2c
      visual format:
        visual id:      0x365
        pict format id: 0x2c
      visual format:
        visual id:      0x366
        pict format id: 0x2c
      visual format:
        visual id:      0x367
        pict format id: 0x2c
      visual format:
        visual id:      0x368
        pict format id: 0x2c
      visual format:
        visual id:      0x369
        pict format id: 0x2c
      visual format:
        visual id:      0x36a
        pict format id: 0x2c
      visual format:
        visual id:      0x36b
        pict format id: 0x2c
      visual format:
        visual id:      0x36c
        pict format id: 0x2c
      visual format:
        visual id:      0x36d
        pict format id: 0x2c
      visual format:
        visual id:      0x36e
        pict format id: 0x2c
      visual format:
        visual id:      0x36f
        pict format id: 0x2c
      visual format:
        visual id:      0x370
        pict format id: 0x2c
      visual format:
        visual id:      0x371
        pict format id: 0x2c
      visual format:
        visual id:      0x372
        pict format id: 0x2c
      visual format:
        visual id:      0x373
        pict format id: 0x2c
      visual format:
        visual id:      0x374
        pict format id: 0x2c
      visual format:
        visual id:      0x375
        pict format id: 0x2c
      visual format:
        visual id:      0x376
        pict format id: 0x2c
      visual format:
        visual id:      0x377
        pict format id: 0x2c
      visual format:
        visual id:      0x378
        pict format id: 0x2c
      visual format:
        visual id:      0x379
        pict format id: 0x2c
      visual format:
        visual id:      0x37a
        pict format id: 0x2c
      visual format:
        visual id:      0x37b
        pict format id: 0x2c
      visual format:
        visual id:      0x37c
        pict format id: 0x2c
      visual format:
        visual id:      0x37d
        pict format id: 0x2c
      visual format:
        visual id:      0x37e
        pict format id: 0x2c
      visual format:
        visual id:      0x37f
        pict format id: 0x2c
      visual format:
        visual id:      0x380
        pict format id: 0x2c
      visual format:
        visual id:      0x381
        pict format id: 0x2c
      visual format:
        visual id:      0x382
        pict format id: 0x2c
      visual format:
        visual id:      0x383
        pict format id: 0x2c
      visual format:
        visual id:      0x5b
        pict format id: 0x28
      visual format:
        visual id:      0x384
        pict format id: 0x28
      visual format:
        visual id:      0x385
        pict format id: 0x28
      visual format:
        visual id:      0x386
        pict format id: 0x28
      visual format:
        visual id:      0x387
        pict format id: 0x28
      visual format:
        visual id:      0x388
        pict format id: 0x28
      visual format:
        visual id:      0x389
        pict format id: 0x28
      visual format:
        visual id:      0x38a
        pict format id: 0x28
      visual format:
        visual id:      0x38b
        pict format id: 0x28
      visual format:
        visual id:      0x38c
        pict format id: 0x28
      visual format:
        visual id:      0x38d
        pict format id: 0x28
      visual format:
        visual id:      0x38e
        pict format id: 0x28
      visual format:
        visual id:      0x38f
        pict format id: 0x28
      visual format:
        visual id:      0x390
        pict format id: 0x28
      visual format:
        visual id:      0x391
        pict format id: 0x28
      visual format:
        visual id:      0x392
        pict format id: 0x28
      visual format:
        visual id:      0x393
        pict format id: 0x28
      visual format:
        visual id:      0x394
        pict format id: 0x28
      visual format:
        visual id:      0x395
        pict format id: 0x28
      visual format:
        visual id:      0x396
        pict format id: 0x28
      visual format:
        visual id:      0x397
        pict format id: 0x28
      visual format:
        visual id:      0x398
        pict format id: 0x28
      visual format:
        visual id:      0x399
        pict format id: 0x28
      visual format:
        visual id:      0x39a
        pict format id: 0x28
      visual format:
        visual id:      0x39b
        pict format id: 0x28
      visual format:
        visual id:      0x39c
        pict format id: 0x28
      visual format:
        visual id:      0x39d
        pict format id: 0x28
      visual format:
        visual id:      0x39e
        pict format id: 0x28
      visual format:
        visual id:      0x39f
        pict format id: 0x28
      visual format:
        visual id:      0x3a0
        pict format id: 0x28
     depth formats:
```

上面使用 `xdpyinfo` 命令得到两张表：

- **Render formats** 表列出了所有 RENDER 扩展支持的颜色格式的信息，包括它们的位深 (depth), 以及每个 Component 的位深 (mask)
- **Screen formats** 表实际上列出的是所有 X Visual 与 Render formats 的绑定关系

接下来只需要查看一下程序(或者 GLFW )选取的 X Visual 的 Render format 的 Alpha 分量的位深是否为 0。 0 意味着 GLFW 获取的 `GLFW_TRANSPARENT_FRAMEBUFFER` 属性是`FALSE`, 也就意味着 GLFW 创建的这个窗口中，无法通过修改颜色的 Alpha 分量来改变窗口的透明度, 因为应用的窗口和桌面背景的融合(你是否能透过窗口看到桌面)是 X11 控制的。

我系统上(WSL Ubuntu 20.04)绝大部分VISUAL对应的都是这个ID为`0x2c`的**pict format**, 所以我不能通过修改alpha通道改变窗口背景或内容的透明度，除非我强制让glfw选择其它的VISUAL(对应的**pict format**不是`0x2c`)。

为了确定这个问题的确是VISUAL惹的祸，我想摆脱glfw，在单纯的GLX环境下测试一下，正好[mesa/demos](https://gitlab.freedesktop.org/mesa/demos)这个工程下有一个**glxgears_fbconfig**，这个demo允许通过`GLX_VISUAL_ID`来指定你想要的VISUAL，我分别使用下面两个VISUAL进行测试

- visual id: 0x5b, pict format id 0x28

    ```
      pict format:
            format id:    0x28
            type:         Direct
            depth:        32
            alpha:        24 mask 0xff
            red:          16 mask 0xff
            green:         8 mask 0xff
            blue:          0 mask 0xff
    ```

- visual id: 0x23, pict format id 0x2c

注意，为了更清楚地看出不同，我使用`glClearBufferfv`将背景设置为半透明

```diff
diff --git a/src/xdemos/glxgears_fbconfig.c b/src/xdemos/glxgears_fbconfig.c
index 8806dd1c..7d6cbd29 100644
--- a/src/xdemos/glxgears_fbconfig.c
+++ b/src/xdemos/glxgears_fbconfig.c
@@ -35,0 +36 @@
+#define GL_GLEXT_PROTOTYPES
@@ -242,0 +244 @@ draw(void)
+    static const GLfloat bg[] = {0.0, 0.0, 0.0, 0.5};
+    @@ -243,0 +246 @@ draw(void)
+    +   glClearBufferfv(GL_COLOR, 0, bg);
+    @@ -439,0 +443 @@ make_window( Display *dpy, const char *name,
+    +            GLX_VISUAL_ID,     0x23,)
```

测试结果如下:

![VisualID=0x23](/images/X/kylin-alpha-issue/visual_0x23.png)
![VisualID=0x5b](/images/X/kylin-alpha-issue/visual_0x5b.png)

从测试结果看，在`VisualID=0x23`的情况下，这个VISUAL对应的`pict format`没有alpha通道，所以修改`bg`的alpha值不会对窗口背景的透明度有任何影响。

# glx visual 和 fbconfig 不是一回事

所以 **GLX_VISUAL_ID** 和 **GLX_FBCONFIG_ID** 也不是一回事， 虽然它俩都是 `XID`, 但 mesa 的实现虽然将 glx visual 和 fbconfig 统一用 `glx_config` 来表示, 但

```c
_GLX_PUBLIC XVisualInfo *
glXChooseVisual(Display * dpy, int screen, int *attribList);

_GLX_PUBLIC GLXFBConfig *
glXChooseFBConfig(Display * dpy, int screen, const int *attribList, int *nitems)
```

是完全不同的实现，而且 mesa 的 `glXChooseVisual()` 实际上无法通过指定 **GLX_VISUAL_ID** 来选择某个特定的 X Visual (这可能是一个 Bug)。
