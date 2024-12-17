---
title: x11perf
date: 2024-12-16 16:31:25
tags: [X11]
categories: graphics
---

# [x11perf](https://gitlab.freedesktop.org/xorg/test/x11perf)

[x11perf](https://gitlab.freedesktop.org/xorg/test/x11perf) 是一个 X11 Server 的性能测试程序

![x11perf -range -ftext,-crgbftext](/images/x11perf/x11perf-f24itext.png)

<!--more-->

# [Xlib](https://gitlab.freedesktop.org/xorg/lib/libx11)

[Xlib](https://gitlab.freedesktop.org/xorg/lib/libx11) 是 X11 核心客户端库，客户端程序基本上都是调用它提供的 API 来绘制图形的。

# ftext

```mermaid
flowchart TD
    subgraph one [InitText]
        A["XChangeGC()"]
    end
    subgraph two [DoText]
        B["XDrawString()"]
    end
    subgraph three [ClearTextWin]
        C["XClearWindow()"]
    end
    subgraph four [EndText]
        D["XFreeFont()"]
    end

    one --> two --> three --> four
```
