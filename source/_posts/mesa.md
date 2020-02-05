---
title: Notes for Mesa
date: 2020-2-5
tags: [OpenGL]
---
[Mesa](https://mesa3d.org/) is an open source implementation for OpenGL. Check [repository](https://gitlab.freedesktop.org/mesa/mesa) for its source code. There are something worth to learn about this project for a graphic driver developer.

## Build
It's good choice for exploring any project's source code to start with its build script. So here is the start.

### Questions

#### What dependencies are there when you build mesa on the WSL?

* Run-time dependency
    - libdrm_intel
    - libdrm_amdgpu
    - libdrm_radeon
    - libdrm_nouveau
    - libdrm
    - LLVM
    - libelf
    - valgrind
    - bison
    - flex
    - gettext

* Build-time dependency
    - wayland-scanner

These dependencies include from build-time headers to run-time tools. Nevertheless most of them are optional but not necessary. You can customize them in **meson_options.txt** by modifying the **value** field of each option. Then you can create a build directory and start to configure.

``` bash
meson --prefix="${PWD}/build/install" build
```
You can configure a minimal dependencies mesa given that WSL is not a desktop platform as the following my configuration result:

```
Message: Configuration summary:
        
        prefix:          /home/luc/github/mesa/build/install
        libdir:          lib/x86_64-linux-gnu
        includedir:      include
        
        OpenGL:          yes (ES1: no ES2: no)
        OSMesa:          no
        
        EGL:             no
        GBM:             no
        EGL/Vulkan/VL platforms:   drm
        
        Vulkan drivers:  no
        
        llvm:            no
        
        Gallium drivers: swrast
        Gallium st:      mesa
        HUD lmsensors:   no
        
        Shared-glapi:    yes

Build targets in project: 85
Found ninja-1.9.0 at /usr/bin/ninja
```
NOTE: if you remodify the **meson_options.txt** and are about to reconfigure, you need to run:

``` bash
meson setup --wipe build
```

Once dependencies check passed you can compile and install:

``` bash
ninja -C build && ninja -C build install
```
So far you should have at least a single static library, say libsoftpipe.a, generated although only with this are you unable to get your OpenGL application worked on the WSL.

#### What role do DRM, DRI and Gallium play in Mesa? 

``` meson
_libdrm_checks = [
  ['intel', with_dri_i915 or with_gallium_i915],
  ['amdgpu', with_amd_vk or with_gallium_radeonsi],
  ['radeon', (with_gallium_radeonsi or with_dri_r100 or with_dri_r200 or
              with_gallium_r300 or with_gallium_r600)],
  ['nouveau', (with_gallium_nouveau or with_dri_nouveau)],
]
```

DRI and Gallium seem to be respectively different implementation on DRM. Moreover in term of swrast and i915, you have to choose either of both as you can read the following code snippet in meson.build.

``` meson
if with_dri_swrast and (with_gallium_softpipe or with_gallium_swr)
  error('Only one swrast provider can be built')
endif
if with_dri_i915 and with_gallium_i915
  error('Only one i915 provider can be built')
endif
```
