---
title: Notes for Mesa
date: 2020-2-4
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
    - libx11-xcb-dev
    - libxcb-fixes0-dev
    - libxext-dev

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
        
        Shared-glapi:    no

Build targets in project: 83
Found ninja-1.9.0 at /usr/bin/ninja
```

Once dependencies check passed you can compile and install:

``` bash
ninja -C build && ninja -C build install
```

So far you should have at least a single static library, say libsoftpipe.a, generated although only with this can you not get your OpenGL application worked on the WSL.

#### libGL.so is not built until glx option is enabled in **meson_options.txt**.

As mentioned above, with the minimal dependencies, you will get nothing but libsoftpipe.a. Only with essential build-time dependencies for X11 installed and glx option configured is libGL.so built. The configuration is as follow:

```
Message: Configuration summary:
        
        prefix:          /home/luc/github/mesa/build/install
        libdir:          lib/x86_64-linux-gnu
        includedir:      include
        
        OpenGL:          yes (ES1: no ES2: no)
        
        OSMesa:          libOSMesa
        
        GLX:             Xlib-based (Gallium)
        
        EGL:             no
        GBM:             no
        EGL/Vulkan/VL platforms:   x11
        
        Vulkan drivers:  no
        
        llvm:            yes
        llvm-version:    9.0.0
        
        Gallium drivers: nouveau swrast
        Gallium st:      mesa
        HUD lmsensors:   no
        
        Shared-glapi:    no

Build targets in project: 91
```

NOTE: if you have remodified the **meson_options.txt** and are about to reconfigure, you need to run:

``` bash
meson setup --wipe build
```

After compilation and installed as follow:

```
lrwxrwxrwx 1 luc luc        10 Feb  5 16:18 libGL.so -> libGL.so.1*
lrwxrwxrwx 1 luc luc        14 Feb  5 16:18 libGL.so.1 -> libGL.so.1.5.0*
-rwxrwxrwx 1 luc luc 112504456 Feb  5 16:18 libGL.so.1.5.0*
lrwxrwxrwx 1 luc luc        14 Feb  5 16:18 libOSMesa.so -> libOSMesa.so.8*
lrwxrwxrwx 1 luc luc        18 Feb  5 16:18 libOSMesa.so.8 -> libOSMesa.so.8.0.0*
-rwxrwxrwx 1 luc luc  53012360 Feb  5 16:18 libOSMesa.so.8.0.0*
```

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

DRI and Gallium seem to be respectively different underlying implementation in Mesa. Moreover in term of swrast and i915, you have to choose either of both as you can read the following code snippet in meson.build.

``` meson
if with_dri_swrast and (with_gallium_softpipe or with_gallium_swr)
  error('Only one swrast provider can be built')
endif
if with_dri_i915 and with_gallium_i915
  error('Only one i915 provider can be built')
endif
```

#### What problems are encountered when you build mesa on the WSL?

- dri based GLX requires shared-glapi
- Gallium-xlib based GLX requires softpipe or llvmpipe
