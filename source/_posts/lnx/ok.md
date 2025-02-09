---
title: openKylin 2.0
date: 2024-11-27 15:36:16+08:00
tags: [linux]
categories: linux
---

# [OpenKylin 2.0](https://www.openkylin.top/)

![ok2.0](/images/ok/ok2.0.png)

<!--more-->

# Install trace-cmd

还算顺利，大部分依赖包从系统 apt 源里都能安装，但也有一些问题

- `/usr/lib64/pkgconfig` 没有在 `pkg-config` 默认的 search paths 里
    - `export PKG_CONFIG_PTH=/usr/lib64/pkgconfig` 可解决构建问题
- `/usr/lib64` 没有在 `ld.so` 的默认的 search paths 里
    - `/etc/ld.so.conf.d/x86_64-linux-gnu.conf` 里增加 `/usr/lib64` 可解决运行时找不到 shared library 的问题
- 因为 xmlto 的依赖问题无法安装，幸好它只是用来构建 Documentation, 通过 `-Ddoc=false` 不构建就可以继续下去

| Dependency   | Apt Install | Problem       |
|:-------------|:-----------:|:--------------|
| meson        | √           |               |
| cmake        | √           |               |
| asciidoc     | √           |               |
| xmlto        | ∅           | docbook unmet |
| flex         | √           |               |
| bison        | √           |               |
| libaudit-dev | √           |               |

# Install apitrace

apitrace 不带 GUI 的话可以非常顺利的安装，带 GUI 的话因为依赖 Qt5/6, 稍微麻烦一点

```
if (ENABLE_GUI)
    if (NOT (ENABLE_GUI STREQUAL "AUTO"))
        set (REQUIRE_GUI REQUIRED)
    endif ()
    if (POLICY CMP0020)
        cmake_policy (SET CMP0020 NEW)
    endif()
    if (ENABLE_QT6)
        find_package (Qt6 COMPONENTS Widgets Network ${REQUIRE_GUI})
    else ()
        find_package (Qt5 5.15 COMPONENTS Widgets Network ${REQUIRE_GUI})
    endif ()
endif ()
```

## Install Qt

一开始通过 [qt-online-installer-linux-x64-4.8.1.run](https://www.qt.io/download-qt-installer-oss) 安装 Qt 6.8, 安装后 cmake `find_package` 还是有问题，就只能源码安装 [qt 5.15.16](https://download.qt.io/archive/qt/5.15/)

- `mkdir -p /opt/Qt5.15`
- `mkdir -p ~/qt-build`
- `cd ~/qt-build`
- `~/qt-everywhere-src-5.15.16/configure -prefix /opt/Qt5.15`
- `make -j $(nproc)`
- `sudo make install`

Qt 5.15.16 安装完成后就可以继续安装 qapitrace 了

- `cd ~/apitrace`
- `cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_MODULE_PATH=/opt/Qt5.15/lib/pkgconfig`
- `sudo cmake --install build`

可惜安装后运行 `qapitrace glxgears.trace` 报段错误，难道是 Qt 的版本太高了?

```
UKUIStylePlugin.........
QObject::connect: No such slot UKUIStylePlugin::tableModeChanged(bool)
BlurHelper00000.............
m_cfgPath... "/usr/share/qt5-ukui-platformtheme/themeconfig/default.json"
段错误
```

将 `/opt/Qt5.15/lib` 添加到 ld.so.conf 后又报出下面的错误, 提示 Qt 缺少 **wayland, xcb platform plugin**

```
qt.qpa.plugin: Could not find the Qt platform plugin "wayland" in ""
qt.qpa.plugin: Could not find the Qt platform plugin "xcb" in ""
This application failed to start because no Qt platform plugin could be initialized. Reinstalling the application may fix this problem.

Available platform plugins are: eglfs, linuxfb, minimal, minimalegl, offscreen, vnc, webgl.

已放弃
```

 所以检查编译配置，发现 **QPA backends** 中的 EGLFS 虽然打开了，但 EGLFS details 里所有的后端一个也没有打开， 阅读 [EGLFS 的文档](https://doc.qt.io/qt-6/embedded-linux.html#eglfs) 后了解到 EGLFS 似乎是运行在 EGL 和 OpenGL ES 2.0 之上， 但 **Qt Gui** 里的 OpenGL ES 2.0/3.0/3.1/3.2 都没有开启， 还有 **XCB Xlib** 也没有开启。

```
Qt Gui:
  Accessibility .......................... yes
  FreeType ............................... yes
    Using system FreeType ................ yes
  HarfBuzz ............................... yes
    Using system HarfBuzz ................ no
  Fontconfig ............................. yes
  Image formats:
    GIF .................................. yes
    ICO .................................. yes
    JPEG ................................. yes
      Using system libjpeg ............... yes
    PNG .................................. yes
      Using system libpng ................ yes
  Text formats:
    HtmlParser ........................... yes
    CssParser ............................ yes
    OdfWriter ............................ yes
    MarkdownReader ....................... yes
      Using system libmd4c ............... no
    MarkdownWriter ....................... yes
  EGL .................................... yes
  OpenVG ................................. no
  OpenGL:
    Desktop OpenGL ....................... yes
    OpenGL ES 2.0 ........................ no
    OpenGL ES 3.0 ........................ no
    OpenGL ES 3.1 ........................ no
    OpenGL ES 3.2 ........................ no
  Vulkan ................................. no
  Session Management ..................... yes
Features used by QPA backends:
  evdev .................................. yes
  libinput ............................... no
  INTEGRITY HID .......................... no
  mtdev .................................. no
  tslib .................................. no
  xkbcommon .............................. no
  X11 specific:
    XLib ................................. yes
    XCB Xlib ............................. no
    EGL on X11 ........................... no
    xkbcommon-x11 ........................ no
QPA backends:
  DirectFB ............................... no
  EGLFS .................................. yes
  EGLFS details:
    EGLFS OpenWFD ........................ no
    EGLFS i.Mx6 .......................... no
    EGLFS i.Mx6 Wayland .................. no
    EGLFS RCAR ........................... no
    EGLFS EGLDevice ...................... no
    EGLFS GBM ............................ no
    EGLFS VSP2 ........................... no
    EGLFS Mali ........................... no
    EGLFS Raspberry Pi ................... no
    EGLFS X11 ............................ no
  LinuxFB ................................ yes
  VNC .................................... yes
```

所以继续阅读 [Qt for X11 requirements](https://doc.qt.io/qt-6/linux-requirements.html) 文档, 安装文档中所有列出的依赖软件包后， 重新编译 Qt 5.15.16, 这次 qapitrace 运行正常，而且之前报出的缺少 **Qt Platform plugin "xcb" in ""** 也消失了

![qapitrace](/images/ok/apitrace.png)

## PAL

Qt Platform Abstraction (QPA) 是 Qt 中主要的**平台抽象层 (Platform Abstraction Layer)**, PAL 这个概念在 [AMD GPU 驱动](https://github.com/GPUOpen-Drivers/pal)中也有， 在 Android 里也有类似的 HAL (Hardware Abstraction Layer) 的概念，在系统软件设计中， PAL 的作用可谓是"承上启下"， 设计一个**接口稳定，易于扩展**的 PAL API 是整个用户态系统软件实现效率的关键。通过这次安装 Qt, 又多了一个学习的例子。

# [openKylin:linux-6.6-next](https://gitee.com/openkylin/linux/tree/linux-6.6-next)

主要关注了一下国产 GPU/DRM 驱动

```
./omapdrm
./nouveau
./mxsfb
./mwv207 # 景嘉微 0x0731
./msm
./mgag200
./meson
./mediatek
./mcde
./loongson # 龙芯 0x0014
./logicvc
./lima
./lib
./kmb
./ingenic
./imx
./i915
./i2c
./hyperv
./hisilicon
./gud
./gma500
./fsl-dcu
./exynos
./etnaviv
./display
./ci
./bridge
./atmel-hlcdc
./ast
./aspeed
./armada
./arm
./arise  # 格兰菲 0x6766
./amd
.
➜  drm git:(linux-6.6-next)
```

# [RedEclipse](https://github.com/redeclipse/base)

![RedEclipse on NV137](/images/ok/nv137-redeclipse.png)

# [Hyprland](git@github.com:hyprwm/Hyprland.git)

## Dependencies

|  | dep                        | apt-get             | version required     | yet another install                                                                                            |
|:-|:---------------------------|:--------------------|:---------------------|:---------------------------------------------------------------------------------------------------------------|
|  | wayland-protocols          | wayland-protocols   | 1.38-ok1             |                                                                                                                |
|  | libseat                    | libseat-dev         | 0.8.0-ok2            |                                                                                                                |
|  | libinput                   | libinput-dev        | 1.26.0(1.25.0-ok1.2) | [https://gitlab.freedesktop.org/libinput/libinput](https://gitlab.freedesktop.org/libinput/libinput)           |
|  | wayland                    | libwayland-dev      | 1.23.0-1ok3          |                                                                                                                |
|  | libdisplay-info            | libdisplay-info-dev | 0.1.1-ok1            |                                                                                                                |
|  | aquamarine                 |                     | >=0.4.5              | [https://github.com/hyprwm/aquamarine](https://github.com/hyprwm/aquamarine)                                   |
|^ | hyprwayland-scanner        |                     |                      | [https://github.com/hyprwm/hyprwayland-scanner](https://github.com/hyprwm/hyprwayland-scanner)                 |
|^ | pugixml                    | libpugixml-dev      | 1.14-ok1             | required by hyprwayland-scanner                                                                                |
|  | hyprutils                  |                     | >=0.2.3              | [https://github.com/hyprwm/hyprutils](https://github.com/hyprwm/hyprutils)                                     |


# Resources

- [KylinOS 的软件包 OpenKylin 基本都能用](https://archive.kylinos.cn/kylin/KYLIN-ALL/)
- [trace-cmd](https://trace-cmd.org/)
- [Platform plugins for Embedded Linux devices - EGLFS](https://doc.qt.io/qt-6/embedded-linux.html#embedded-eglfs)
- [Qt for X11 Requirements](https://doc.qt.io/qt-6/linux-requirements.html)

