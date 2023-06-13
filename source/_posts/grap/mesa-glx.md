---
title: Mesa 的 GLX 实现
date: 2021-03-11 14:44:56
tags: [mesa]
categories: graphics
---

# GLX

{% blockquote %}
GLX (Initialism for "OpenGL Extension to the X Window System") is an extension to the X Window System core protocol providing an interface between OpenGL and the X Window System as well as extensions to OpenGL itself. (From WiKi)
{% endblockquote %}

Mesa的GLX实现有3种
- xlib
- gallium-xlib
- dri

```mermaid
graph TD
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

