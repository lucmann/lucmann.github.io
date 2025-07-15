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
  ...
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
        ...
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

为了确定这个问题的确是VISUAL惹的祸，我想摆脱glfw，在单纯的GLX环境下测试一下，正好[mesa/demos](https://gitlab.freedesktop.org/mesa/demos)这个工程下有一个**glxgears_fbconfig**，这个demo允许通过`GLX_FBCONFIG_ID`来指定你想要的 `GLXFBConfig`， 而 `GLXFBConfig` 和 `XVisualInfo` 虽然是不同组件里定义的描述 FB 配置的数据结构(前者是 **Mesa GLX** 里定义的，而后者是 **X11 Util** 里定义的), 但两者存在一一对应的关系。

![GLXFBConfig vs XVisualInfo](/images/kylin-alpha-issue/fbconfig.png)

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

![VisualID=0x23](/images/kylin-alpha-issue/visual_0x23.png)
![VisualID=0x5b](/images/kylin-alpha-issue/visual_0x5b.png)

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
