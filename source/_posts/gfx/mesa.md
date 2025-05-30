---
title: Notes for Mesa
date: 2020-02-04
tags: [Mesa]
categories: graphics
---

# Overview
Mesa 包含了各种 GPU/CPU 的 OpenGL, OpenCL, Vulkan 实现(Usermode Driver), 也包括 GLX, EGL, GBM 等协议的实现。

- Vulkan 驱动

| codename | directories                                  | platforms                       |
|:---------|:---------------------------------------------|:--------------------------------|
| anv      | src/intel/vulkan, src/intel/vulkan_hasvk     | Alder Lake-P                    |
| asahi    | src/asahi/vulkan                             | Apple M1, M2                    |
| lpv      | src/gallium/drivers/llvmpipe                 | CPU                             |
| panvk    | src/panfrost/vulkan                          | RK3399                          |
| v3dv     | src/broadcom/vulkan                          | Raspberry Pi                    |

# Build

## Build Dependencies (since mesa-25.0.0)

|  | dep                        | apt-get             | version required   | yet another install                                                                                            |
|:-|:---------------------------|:--------------------|:-------------------|:---------------------------------------------------------------------------------------------------------------|
|  | /usr/bin/glslangValidator  | glslang-tools       |                    | [https://github.com/KhronosGroup/glslang](https://github.com/KhronosGroup/glslang)                             |
|  | /usr/bin/rustc             | rustc               | 1.78.0 or newer    | `curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh`                                               |
|  | bindgen (rust package)     | cargo               | 0.65 or newer      | `cargo install bindgen-cli`                                                                                    |
|  | libclc-dev                 | libclc-17-dev       |                    |  not required if -Dmesa-clc=auto                                                                               |
|+ | libdrm                     | libdrm-dev          | 2.4.121(120 ok2.0) | [https://gitlab.freedesktop.org/mesa/drm](https://gitlab.freedesktop.org/mesa/drm)                             |
|  | llvm                       | llvm-17-dev         | (17.0.6 ok2.0)     | [https://github.com/llvm/llvm-project](https://github.com/llvm/llvm-project)                                   |
|  | libLLVMSPIRVLib.so.17      | llvm-spirv-17       | (17.0.0 ok2.0)     | [https://github.com/KhronosGroup/SPIRV-LLVM-Translator](https://github.com/KhronosGroup/SPIRV-LLVM-Translator) |
|+ | LLVMSPIRVLib.h             | llvmspirvlib-17-dev | (absent ok2.0)     | [https://github.com/KhronosGroup/SPIRV-LLVM-Translator](https://github.com/KhronosGroup/SPIRV-LLVM-Translator) |
|  | clang-cpp                  | libclang-cpp17-dev  | (17.0.6 ok2.0)     | not required if -Dmesa-clc=auto                                                                                |
|* | clang-dev                  | libclang-17-dev     |                    | [Debian package issue](https://gitlab.freedesktop.org/mesa/mesa/-/issues/10485)                                |
|  | xshmfence                  | libxshmfence-dev    | (1.3    ok2.0)     | required if -Dplatforms=x11                                                                                    |
|  | xxf86vm                    | libxxf86vm-dev      | (1.1.4  ok2.0)     | required since -Dglx-direct=true by default                                                                    |
|  | xrandr                     | libxrandr-dev       | (1.5.2  ok2.0)     | required since -Dxlib-lease=true                                                                               |
|  | cbindgen (rust package)    | cargo               | 0.28.0             | `cargo install cbindgen`; required if -Dgallium-drivers=nouveau                                                |

NOTE:
- (*) 表示本来不需要的依赖
- (+) 在 OpenKylin 2.0 的源里没有，需要源码构建

- Ubuntu 22.04

**书到用时方恨少，包要装时不好找**

```bash
sudo apt install -y cmake ninja-build bison flex g++ git pkg-config python3-setuptools python3-lz4 \
    python3-jinja2 libssl-dev libelf-dev libboost-dev libglm-dev libtinyobjloader-dev libstb-dev \
    libpng-dev wayland-protocols libwayland-dev libdecor-0-dev freeglut3-dev libexpat1-dev libglvnd-dev \
    libx11-dev libxext-dev libxshmfence-dev libxrandr-dev libxxf86vm-dev libxfixes-dev libx11-xcb-dev \
    libxcb1-dev libxcb-randr0-dev libxcb-glx0-dev libxcb-shm0-dev libxcb-dri3-dev libxcb-dri2-0-dev libxcb-present-dev \
    libselinux1-dev libvulkan-dev vulkan-tools mesa-utils
```

```bash
meson build -Dprefix=/usr/local -Dlibdir=lib/x86_64-linux-gnu -Dplatforms=x11 -Dgallium-drivers=nouveau
```

- `-Dprefix=/usr/local` 避免覆盖系统原来的 `libGL*`
- `-Dlibdir=lib/x86_64-linux-gnu` 设置库的安装路径为 `/etc/ld.so.conf.d/x86_64-linux-gnu.conf` 中搜索**第一顺位**的路径
- 其他默认就好， 出现的依赖在 OpenKylin 2.0 上 apt-get 基本都能解决

```console
Build targets in project: 437
WARNING: Project specifies a minimum meson_version '>= 1.1.0' but uses features which were added in newer versions:
 * 1.3.0: {'rust.proc_macro', 'rust_abi arg in static_library'}

mesa 25.0.0-devel

  Directories
    prefix                       : /usr/local
    libdir                       : lib/x86_64-linux-gnu
    includedir                   : include

  Common C and C++ arguments
    c_cpp_args                   : -mtls-dialect=gnu2

  OpenGL
    OpenGL                       : YES
    ES1                          : YES
    ES2                          : YES
    Shared glapi                 : YES
    GLVND                        : YES

  DRI
    Platform                     : drm
    Driver dir                   : /usr/local/lib/x86_64-linux-gnu/dri

  GLX
    Enabled                      : YES
    Provider                     : dri

  EGL
    Enabled                      : YES
    Drivers                      : builtin:egl_dri2 builtin:egl_dri3
    Platforms                    : x11 surfaceless drm xcb

  GBM
    Enabled                      : YES
    Backends path                : /usr/local/lib/x86_64-linux-gnu/gbm

  Vulkan
    Drivers                      : amd intel intel_hasvk nouveau swrast
    Platforms                    : x11 surfaceless drm xcb
    ICD dir                      : share/vulkan/icd.d
    Intel Ray tracing            : YES

  Video
    Codecs                       : av1dec av1enc vp9dec
    APIs                         : vulkan xa

  LLVM
    Enabled                      : YES
    Version                      : 17.0.6

  Gallium
    Enabled                      : YES
    Drivers                      : nouveau
    Platforms                    : x11 surfaceless drm xcb
    Frontends                    : mesa xa
    Off-screen rendering (OSMesa): NO
    HUD lm-sensors               : NO

  Perfetto
    Enabled                      : NO

  Teflon (TensorFlow Lite delegate)
    Enabled                      : NO

  Subprojects
    paste                        : YES
    proc-macro2                  : YES (from syn => quote)
    quote                        : YES (from syn)
    syn                          : YES
    unicode-ident                : YES (from syn)

  User defined options
    libdir                       : lib/x86_64-linux-gnu
    prefix                       : /usr/local
    gallium-drivers              : nouveau
    platforms                    : x11

Found ninja-1.11.1 at /usr/bin/ninja
WARNING: Running the setup command as `meson [options]` instead of `meson setup [options]` is ambiguous and deprecated.
```

<!--more-->

After compilation and installed as follow:

``` bash
ls -l /usr/local/lib/x86_64-linux-gnu
total 159360
lrwxrwxrwx 1 root root        10 Feb  7 17:13 libGL.so -> libGL.so.1
lrwxrwxrwx 1 root root        14 Feb  7 17:13 libGL.so.1 -> libGL.so.1.5.0
-rwxr-xr-x 1 root root 111044912 Feb  7 17:13 libGL.so.1.5.0
lrwxrwxrwx 1 root root        14 Feb  7 17:13 libOSMesa.so -> libOSMesa.so.8
lrwxrwxrwx 1 root root        18 Feb  7 17:13 libOSMesa.so.8 -> libOSMesa.so.8.0.0
-rwxr-xr-x 1 root root  51541176 Feb  7 17:13 libOSMesa.so.8.0.0
lrwxrwxrwx 1 root root        13 Feb  7 17:13 libglapi.so -> libglapi.so.0
lrwxrwxrwx 1 root root        17 Feb  7 17:13 libglapi.so.0 -> libglapi.so.0.0.0
-rwxr-xr-x 1 root root    337264 Feb  7 17:07 libglapi.so.0.0.0
drwxr-xr-x 1 root root       512 Feb  7 17:13 pkgconfig
```

NOTE:
- Mesa is installed in `/usr/local/lib/$(uname -p)-linux-gnu` by default. So you have to `ldconfig` so that your linker can find them.
- libsoftpipe.a will be built but not installed.
- meson build system will enable compiler's `-g` flag by default unless you are building on the release branch.
- if you have remodified the **meson_options.txt** and built once and now are about to reconfigure and rebuild, you need to run:

``` bash
meson setup --wipe build
```

# 与 mesa 相关的一些常用软件包

## mesa-utils

包含以下 glx demo 或 test program

- glxdemo
- glxgears
- glxheads
- glxinfo

## mesa-utils-extra

包含以下 egl demo 或 test program

- eglinfo
- es2_info
- es2gears_wayland
- es2gears_x11
- es2tri


# Off-screen Demos
Now that mesa have been built and installed we can give a try to run an OGL application. Similarly without window system supportd on the WSL, [off-screen rendering](https://mesa3d.org/osmesa.html) is my choice. We can clone the mesa [demos](https://gitlab.freedesktop.org/mesa/demos) which includes a lot of demos besides off-screen demos. 

## Requisite
We need some more libraries besides libOSMesa and libGL before you can get these off-screen demos worked. They are:
- [GLU](https://gitlab.freedesktop.org/mesa/glu)
- libm

To build these demos:
``` bash
gcc osdemo.c -o osdemo -g -I/home/luc/github/demos/src/util -lGL -lGLU -lOSMesa -lm
```

The executable osdemo saves the rendered pixels as the portable pixmap format. You need to covert it to image format e.g. jpg. You may do this with `pnmtojpeg output.ppm > output.jpg`.

<div align=center>{% asset_img "osdemo.jpg" "osdemo" %}</div>

## OSMesa Call Graphs
Mesa supports many features from software pipelines to hardware drivers. For example [Gallium](https://www.freedesktop.org/wiki/Software/gallium/), it features with several software or hardware implementations which include the two software pipelines, softpipe and [llvmpipe](https://www.mesa3d.org/llvmpipe.html). With the different pipes enabled will the calls walk in the different paths. 

## Three Different Build Configuration (reference to **meson_options.txt**)
| Option | *platform* | *glx* | *dri-drivers* | *gallium-drivers* | *llvm* | *osmesa* |
|-|-|-|-|-|-|-|
| **llvmpipe** | x11 | gallium-xlib | | swrast | true  | gallium |
| **softpipe** | x11 | gallium-xlib | | swrast | false | gallium |
| **tnl**      | x11 | gallium-xlib | | swrast | true  | classic |

## Three Different Call Paths
### Context
<div align=center>{% asset_img OSMesaCreateContextExt.png "context initialization" %}</div>

NOTE: As for softpipe and llvmpipe `gl_api` and `gl_context` are created respectively while both of them are created in one path for the classic osmesa.

### Draw
<div align=center>{% asset_img PopMatrix.png "draw command" %}</div>

# Gallium-Based GLX Demos
If you want to know the full graphic stack of an OpenGL demo, you can not get rid of the window system. That is why I will try some GLX demos. Evidently GLX demos must depend on X11. You can cope with this problem by installing [vcXsrv](https://sourceforge.net/projects/vcxsrv/) on the Windows 10 which hosts your WSL. 

<div align=center>{% asset_img glxgears.png "glx demo" %}</div>

This time I still choose the gallium-xlib with softpipe. The following call graph shows the path that GLX context is created.

<div align=center>{% asset_img glXCreateContext.png "glx context creation" %}</div>

As we know, Mesa is quite modularized and flexible. How does it take the path that `softpipe_create_context` rather than other pipe contexts? The [st_manager](https://gitlab.freedesktop.org/mesa/mesa/blob/master/src/gallium/include/state_tracker/st_api.h) is a key structure.

`struct pipe_screen` has a callback function that will be set to `softpipe_create_context`. The following calls will create `struct pipe_screen` that will be set to the `st_manager`.

<div align=center>{% asset_img glXChooseVisual.png "pipe_screen creation" %}</div>

To bind the intended gallium driver backend to Mesa there must be something done before `glXChooseVisual` is called. It's started by the library `init()` and prepare the global variables.

[xm_public.h](https://gitlab.freedesktop.org/mesa/mesa/blob/master/src/gallium/state_trackers/glx/xlib/xm_public.h)

``` c
/* This is the driver interface required by the glx/xlib state tracker. 
 */
struct xm_driver {
   struct pipe_screen *(*create_pipe_screen)( Display *display );
   struct st_api *(*create_st_api)( void );
};

extern void
xmesa_set_driver( const struct xm_driver *driver );
```

# DRI-Based GLX Demos

## Loading

- `__glXInitialize`
- `driOpenDriver`

This process of loading drivers works similarly with that of gallium-based glx. Compilation macros and environment variables make a difference. There are several approaches to load the specific drivers:

- `dri3_create_display`
- `dri2CreateDisplay`
- `driCreateDisplay`
- `driswCreateDisplay`
- `applegl_create_display`
- `driwindowsCreateDisplay`

Let's look into `driCreateDisplay`. Once it manages to attach to `driCreateScreen` which searches and matches the appropriate gallium driver the function `driOpenDriver` will open the **found** driver by its name like "i965", "radeon", "nouveau", etc. These drivers are supposed to be installed at **`LIBGL_DRIVERS_PATH`** or `LIBGL_DRIVERS_DIR`(deprecated) and named as `*_dri.so` by default.

Like Gallium-based GLX's `_init` routine with GCC **`constructor`** attribute, DRI-based GLX also defines a routine `megadriver_stub_init` with `constructor` attribute which allows to load the specific driver in a way of **`__DRIextension`**.

<div align=center>{% asset_img "gdb.svg" "osdemo" %}</div>

## Contexts
There are a variety of **contexts** in Mesa. They are designed as a framework of layers.

```
__________________
|                |
|   gl_contex    | --------------> standard & general
|________________|

__________________
|                |
|   st_contex    | --------------> adapter
|________________|

__________________
|                |
|  draw_contex   | --------------> driver-specific
|________________|

```

### gl_context
{% blockquote %}
This is the central context data structure for Mesa. Almost all OpenGL state is contained in this structure. Think of this as a base class from which device drivers will derive sub classes.
{% endblockquote %}
Apart from OpenGL state it contains several other contexts

- `swrast_context`
- `swsetup_context`
- `swtnl_context`
- `vbo_context`
- `st_context`

### st_context

### draw_context

### vbo_context
VBO is short for vertex buffer object. This context derives two kinds of vbo contexts, `vbo_exec_context` and `vbo_save_context` which `vbo_exec_context` is generic for core and compatible ogl and the other is specific for compatible ogl.

#### vbo_exec_vtx_init

- Allocate a `gl_buffer_object` which just is referenced.
- Initialize vbo attributes including size, type and active size.

#### vbo vs. vao

``` c
struct gl_buffer_object
{
   GLint RefCount;
   GLuint Name;
   GLchar *Label;       
   GLenum16 Usage;      
   GLbitfield StorageFlags; 
   GLsizeiptrARB Size;  
   GLubyte *Data;       
   GLboolean DeletePending;   
   GLboolean Written;   
   GLboolean Purgeable; 
   GLboolean Immutable; 
   gl_buffer_usage UsageHistory; 
   GLuint NumSubDataCalls;
   GLuint NumMapBufferWriteCalls;
   struct gl_buffer_mapping Mappings[MAP_COUNT];
   simple_mtx_t MinMaxCacheMutex;
   struct hash_table *MinMaxCache;
   unsigned MinMaxCacheHitIndices;
   unsigned MinMaxCacheMissIndices;
   bool MinMaxCacheDirty;
   bool HandleAllocated; 
};


struct gl_vertex_array_object
{
   GLuint Name;
   GLint RefCount;
   GLchar *Label;       
   GLboolean EverBound;
   bool SharedAndImmutable;
   struct gl_array_attributes VertexAttrib[VERT_ATTRIB_MAX];
   struct gl_vertex_buffer_binding BufferBinding[VERT_ATTRIB_MAX];
   GLbitfield VertexAttribBufferMask;
   GLbitfield Enabled;
   GLbitfield _EffEnabledVBO;
   gl_attribute_map_mode _AttributeMapMode;
   GLbitfield NewArrays;
   struct gl_buffer_object *IndexBufferObj;
};
```

## Dispatchers

- *`Exec`*: The current dispatch table for non-displaylist-saving execution, either BeginEnd or OutsideBeginEnd
- *`OutsideBeginEnd`*: The normal dispatch table for non-displaylist-saving, non-begin/end
- *`Save`*: The dispatch table used between glNewList() and glEndList()
- *`BeginEnd`*: The dispatch table used between glBegin() and glEnd() (outside of a display list). Only valid functions between those two are set, which is mostly just the set in a GLvertexformat struct.
- *`ContextLost`*: Dispatch table for when a graphics reset has happened.
- *`MarshalExec`*: Dispatch table used to marshal API calls from the client program to a separate server thread. NULL if API calls are not being marshalled to another thread.
- *`CurrentClientDispatch`*: Dispatch table currently in use for fielding API calls from the client program. If API calls are being marshalled to another thread, this refers to *`MarshalExec`*. Otherwise it refers to *`CurrentServerDispatch`*.
- *`CurrentServerDispatch`*: Dispatch table currently in use for performing API calls. It refers to *`Save`* or *`Exec`*.


## Modules

- draw module
- CSO module
- translate module
- VBO module
- TNL module(Transform & Light)

## draw_xxx_stage

- **`extern struct draw_stage *draw_unfilled_stage( struct draw_context *context );`**
- **`extern struct draw_stage *draw_twoside_stage( struct draw_context *context );`**
- **`extern struct draw_stage *draw_offset_stage( struct draw_context *context );`**
- **`extern struct draw_stage *draw_clip_stage( struct draw_context *context );`**
- **`extern struct draw_stage *draw_flatshade_stage( struct draw_context *context );`**
- **`extern struct draw_stage *draw_cull_stage( struct draw_context *context );`**
- **`extern struct draw_stage *draw_stipple_stage( struct draw_context *context );`**
- **`extern struct draw_stage *draw_wide_line_stage( struct draw_context *context );`**
- **`extern struct draw_stage *draw_wide_point_stage( struct draw_context *context );`**
- **`extern struct draw_stage *draw_validate_stage( struct draw_context *context );`**

## Auxiliary

- cso_cache
    The CSO cache is used to accelerate preparation of state by saving driver-specific state structure for later use.

- draw
    Draw is a software TCL pipeline for hardware that lacks vertex shaders or other essential parts of pre-rasterization vertex preparation.

- driver_ddebug
- driver_noop
- driver_rbug
- driver_trace
- gallivm
- hud
- indices
    Indices provides tools for translating or generating element indices for use with element-based rendering.

- nir
- os
    * memory allocation
    * simple message logging
    * obtaining run-time configuration option
    * threading primitives
    The OS module contains the abstraction for basic operating system services above. This is the bare minimum required to port Gallium to a new platform. It already provides the implementations of these abstractions for the most common platforms. When targeting an embedded platform no implementation will be provided - these must be provided separately.

- pipe-loader
- pipebuffer
- postprocess
- rbug
- renderonly
- rtasm
- target-helpers
- tgsi
- translate
- util
- vl

# Q&A

- When xlib creates pipe screen, *only* software rasterizers or pipes'screen are created. And llvmpipe, softpipe, virgl, swr, unexceptionally, are software rasterizers or virtual GPU. [Zink](https://www.collabora.com/news-and-blog/blog/2018/10/31/introducing-zink-opengl-implementation-vulkan/) is, in brief, a translator from OpenGL to Vulkan and implemented as Gallium driver. So why only software pipes?

The answer is **`sw_winsys`**. All of target helpers's parameter is a `sw_winsys`. Check mesa source directory: [mesa/src/gallium/winsys](https://gitlab.freedesktop.org/mesa/mesa/tree/master/src/gallium/winsys)

```
amdgpu
etnaviv
freedreno
i915
iris
kmsro
lima
nouveau
panfrost
radeon
svga
sw
tegra
v3d
vc4
virgl
```

To put it simply, specific driver corresponds to specific winsys. The `sw` is for software rasterizers. If you expect to create pipe screen for some driver else, you need to add another target helper with its winsys as parameter like: 

```
static inline struct pipe_screen *
i915_screen_create_named(struct i915_drm_winsys *winsys, const char *driver)
```

That means you have to declare a bunch of new interfaces from the top. So you'd better wrap the function to create specific driver's winsys so that it can take a sw_winsys as its parameter like:


```
#if defined(GALLIUM_VIRGL)
   if (screen == NULL && strcmp(driver, "virpipe") == 0) {
      struct virgl_winsys *vws;
      vws = virgl_vtest_winsys_wrap(winsys);
      screen = virgl_create_screen(vws, NULL);
   }
#endif

```


- libGL.so is not built until glx option is enabled in **meson_options.txt**.

Only with essential build-time dependencies for X11 installed and glx option configured is libGL.so built.

- What role do DRM, DRI and Gallium play in Mesa? 

``` meson
_libdrm_checks = [
  ['intel', with_dri_i915 or with_gallium_i915],
  ['amdgpu', with_amd_vk or with_gallium_radeonsi],
  ['radeon', (with_gallium_radeonsi or with_dri_r100 or with_dri_r200 or
              with_gallium_r300 or with_gallium_r600)],
  ['nouveau', (with_gallium_nouveau or with_dri_nouveau)],
]
```

DRI and Gallium seem to be respectively different underlying implementation in Mesa. Moreover in term of swrast and i915, you have to choose either of both as you can read the following code snippet in meson.build. In fact DRI is more complicated and staler but Gallium is more smaller and simpler.

``` meson
if with_dri_swrast and (with_gallium_softpipe or with_gallium_swr)
  error('Only one swrast provider can be built')
endif
if with_dri_i915 and with_gallium_i915
  error('Only one i915 provider can be built')
endif
```

- What problems are encountered when you build mesa on the WSL?

- dri based GLX requires shared-glapi
- Gallium-xlib based GLX requires softpipe or llvmpipe
    * means that `gallium-xlib` is supposed to only support software rasterizers(llvmpipe, softpipe) and virtual GPU(virgl, swr).
```
option(
  'glx',
  type : 'combo',
  value : 'xlib',
  choices : ['auto', 'disabled', 'dri', 'xlib', 'gallium-xlib'],
  description : 'Build support for GLX platform'
)
```
In Mesa, glx is implemented in three ways:

| *-based | backend | window system |
|-|-|-|
| dri-based | non-sw-pipes | *_drm_winsys |
| xlib | tnl | sw_winsys |
| gallium-based | softpipe/llvmpipe | sw_winsys |


- OSMesa gallium requires gallium softpipe or llvmpipe
    * means if `osmesa` is configured as `gallium`, `gallium-drivers` must include `swrast` but the `classic` osmesa uses the fixed-functioned TNL by default.
```
option(
  'osmesa',
  type : 'combo',
  value : 'gallium',
  choices : ['none', 'classic', 'gallium'],
  description : 'Build OSmesa.'
)
```
- Cannot build GLX support without X11 platform support and at least one OpenGL API
    * GLX, As the name suggests, is dedicated to X11 winsys.

- When `__glXInitialize` creates the `Display`, **only** `driswCreateDisplay` returns successfully. Both of `dri2CreateDisplay` and `driCreateDisplay` failed.

- env: WSL on Windows 10 and with vcXsrv installed on the host as X server

The cause of failure is that vcXsrv has no extensions with DRI or DRI2. This lack of X server extension fails `DRI2QueryExtension` and `XF86DRIQueryExtension` so that the loading of gallium driver is not invoked.

# Debug

## 环境变量

- MESA_VERBOSE=comma-separated-list
  - MESA_VERBOSE=api
- MESA_GLSL=comma-separated-list
  - `MESA_GLSL=log` 把所有 GLSL shader 保存到文件里， shader_X.vert 或 shader_X.frag

# References

- [mdoc: Shading Languages Environment Variables](https://docs.mesa3d.org/shading.html#environment-variables)
