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