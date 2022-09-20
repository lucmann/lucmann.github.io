---
title: Mesa Gallium 驱动框架
date: 2021-04-08 08:07:49
tags: [mesa]
categories: graphics
---

# Gallium Framework

## `-Dgallium-drivers` 与 `-Ddri-drivers` 的区别，以及它们与 `-Degl` 的关系 

从构建的角度看， `-Dgallium-drivers` 与 `-Ddri-drivers` 是无关的。

```
        DRI platform:    drm
        DRI drivers:     no
        DRI driver dir:  /usr/lib/x86_64-linux-gnu/dri
```

```
        EGL:             yes
        EGL drivers:     builtin:egl_dri2 builtin:egl_dri3
        EGL/Vulkan/VL platforms:   x11 surfaceless drm xcb
```

```
        Gallium drivers: swrast panfrost
        Gallium st:      mesa
        HUD lmsensors:   no
```

```
build/src/gallium/targets/dri/libgallium_dri.so
build/src/gallium/targets/dri/8381c20@@gallium_dri@sha/target.c.o
build/src/gallium/targets/dri/8381c20@@gallium_dri@sha/megadriver_stub.c.o
```

```plantuml
@startuml
class pipe_loader_device

class pipe_loader_sw_device {
    sw_driver_descriptor * dd
    .. !GALLIUM_STATIC_TARGETS ..
    util_dl_library * lib
    ..
    sw_winsys * ws
    int fd
}

class pipe_loader_drm_device {
    drm_driver_descriptor * dd
    .. !GALLIUM_STATIC_TARGETS ..
    util_dl_library * lib
    ..
    int fd
}

pipe_loader_device <|-- pipe_loader_drm_device
pipe_loader_device <|-- pipe_loader_sw_device
@enduml
```

