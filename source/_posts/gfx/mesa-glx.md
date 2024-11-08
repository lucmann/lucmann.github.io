---
title: Mesa GLX 实现
date: 2021-03-11 14:44:56
tags: [mesa]
categories: graphics
---

# GLX

GLX 是 Mesa 中实现的三大支持平台(EGL, GLX, GBM)之一, 原来 GLX 在 Mesa 中有 *xlib*, *gallium-xlib* 和 *dri* 三种实现，自从 [Delete Mesa Classic](https://gitlab.freedesktop.org/mesa/mesa/-/merge_requests/10153) 后就只剩下 *xlib*，*dri* 两种实现，其中 *dri* 使用得比较多一些。

<!--more-->

```mermaid
flowchart TD
    A[xlib or gallium-xlib]
    A --> x11
    A --> xext
    A --> xcb
    B[dri]
    B --> x11
    B --> xext
    B --> xfixes
    B --> xcb-glx
    B --> xcb-shm
```

如果是xlib, 它的源码位于
- mesa/drivers/x11 

如果是gallium-xlib, 它的源码位于
- gallium/winsys/sw/xlib
- gallium/frontends/glx/xlib
- gallium/targets/libgl-xlib

# xlib

# dri

## glXMakeCurrent

```mermaid
sequenceDiagram
    autonumber
    participant GLX
    participant Gallium

    GLX     ->> GLX     : MakeContextCurrent()
    GLX     ->> Gallium : driUnbindContext()
    Gallium ->> Gallium : st_api_make_current()
    Gallium ->> Gallium : st_glFlush()
    opt not the first time MakeCurrent()
        GLX     ->> Gallium : driBindContext()
        Gallium ->> Gallium : st_api_make_current()
        Gallium ->> Gallium : st_glFlush()
        GLX     ->> Gallium : driDestroyContext()
        Gallium ->> Gallium : st_glFlush()
        Gallium ->> Gallium : st_context_destroy()
    end
```

# GLVND(Vendor Neutral GL dispatch library)

| Source Directory |  Shared libraray       | APT Package                            |
|:-----------------|:-----------------------|:---------------------------------------|
| GLDispatch       | libGLdispatch.so.0.0.0 | libglvnd0_1.3.2-1~kylin0.20.04.2_arm64 |
| GLX              | libGLX.so.0.0.0        | libglx0_1.3.2-1~kylin0.20.04.2_arm64   |
| EGL              | libEGL.so.1.1.0        | libegl1_1.3.2-1~kylin0.20.04.2_arm64   |
| OpenGL           | libOpenGL.so           | libopengl0_1.3.2-1~kylin0.20.04.2_arm64|
| GLESv1           | libGLESv1_CM.so        | libgles1_1.3.2-1~kylin0.20.04.2_arm64  |
| GLESv2           | libGLESv2.so           | libgles2_1.3.2-1~kylin0.20.04.2_arm64  |
| GL               | libGL.so.1.7.0         | libgl1_1.3.2-1~kylin0.20.04.2_arm64    |

上面这些包和动态库文件都来自同一个源码库 [glvnd/libglvnd](https://gitlab.freedesktop.org/glvnd/libglvnd), 但它们并不是真正的 OpenGL 实现 (驱动)， 它们只是一个 **Dispatch Layer**, 利用 [Dynamic Tag `DT_FILTER`](https://docs.oracle.com/cd/E19683-01/817-3677/6mj8mbtbr/index.html#chapter4-31738) 实现多个 Vendor 的 OpenGL 驱动能在同一个系统上共存。 libglvnd 向用户提供环境变量

`__GLX_VENDOR_LIBRARY_NAME`

来指定运行时到底是哪个 vendor 的驱动被调用。

# Mesa

| APT Package      |  Target              | Description                    |
|:-----------------|:---------------------|:-------------------------------|
| libglapi-mesa    | libglapi.so          | free implementation of GL API  |
| libgles2-mesa    |                      | transitional dummy package     |

# References
- [Dynamic Linking/Dynamic Section](https://docs.oracle.com/cd/E19683-01/817-3677/chapter6-42444/index.html)
