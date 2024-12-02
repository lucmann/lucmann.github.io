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

# Resources

- [KylinOS 的软件包 OpenKylin 基本都能用](https://archive.kylinos.cn/kylin/KYLIN-ALL/)
- [trace-cmd](https://trace-cmd.org/)
