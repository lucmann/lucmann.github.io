---
title: Notes for Mesa
date: 2020-2-4
tags: [OpenGL]
---
[Mesa](https://mesa3d.org/) is an open source implementation for OpenGL. Check [repository](https://gitlab.freedesktop.org/mesa/mesa) for its source code. There are something worth to learn about this project for a graphic driver developer.

## Build
It's good choice for exploring any project's source code to start with its build script. So here is the start.

### Requisite
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

These dependencies include from build-time headers to run-time tools. Nevertheless most of them are optional but not necessary. You can customize them in **meson_options.txt** by modifying the **value** field of each option. With the listed above run and build time dependencies installed you can create a build directory and start to configure.

``` bash
meson build
```

You can configure a minimal dependencies mesa given that WSL is not a desktop platform as the following my configuration result:

```
Message: Configuration summary:
        
        prefix:          /usr/local
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
        
        Gallium drivers: swrast
        Gallium st:      mesa
        HUD lmsensors:   no
        
        Shared-glapi:    yes

Build targets in project: 90
WARNING: Project specifies a minimum meson_version '>= 0.46' but uses features which were added in newer versions:
 * 0.51.0: {'dep.get_variable'}
Found ninja-1.9.0 at /usr/bin/ninja
```
As you see this configuration supports quite a few components which are essential for off-screen rendering. Once dependencies check passed fully you can begin to compile and install:

``` bash
ninja -C build && sudo ninja -C build install
```

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

## Off-screen Demos
Now that mesa have been built and installed we can give a try to run an OGL application. Similarly without window system supportd on the WSL, [off-screen rendering](https://mesa3d.org/osmesa.html) is my choice. We can clone the mesa [demos](https://gitlab.freedesktop.org/mesa/demos) which includes a lot of demos besides off-screen demos. 

### Requisite
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

### Three Different Build Configuration (reference to **meson_options.txt**)
| Option | *platform* | *glx* | *dri-drivers* | *gallium-drivers* | *llvm* | *osmesa* |
|-|-|-|-|-|-|-|
| **llvmpipe** | x11 | gallium-xlib | | swrast | true  | gallium |
| **softpipe** | x11 | gallium-xlib | | swrast | false | gallium |
| **tnl**      | x11 | gallium-xlib | | swrast | true  | classic |

### Three Different Call Paths
#### Context
<div align=center>{% asset_img OSMesaCreateContextExt.png "context initialization" %}</div>

NOTE: As for softpipe and llvmpipe `gl_api` and `gl_context` are created respectively while both of them are created in one path for the classic osmesa.

#### Draw
<div align=center>{% asset_img PopMatrix.png "draw command" %}</div>

## GLX Demos
If you want to know the full runtime stack of an OpenGL demo, you can not get rid of the window system. That is why I will try some GLX demos. Evidently GLX demos must depend on X11. This time I still choose the gallium-xlib with softpipe. The following call graph shows the path that GLX context is created.

<div align=center>{% asset_img glXCreateContext.png "glx context creation" %}</div>

As we know, Mesa is quite modularized and flexible. How does it take the path that `softpipe_create_context` rather than other pipe contexts? The [st_manager](https://gitlab.freedesktop.org/mesa/mesa/blob/master/src/gallium/include/state_tracker/st_api.h) is a key structure.

`struct pipe_screen` has a callback function that will be set to `softpipe_create_context`. The following calls will create `struct pipe_screen` that will be set to the `st_manager`.

<div align=center>{% asset_img glXChooseVisual.png "pipe_screen creation" %}</div>

To bind the intended gallium driver backend to Mesa there must be something done before `glXChooseVisual` is called. It's started by the library `init()` and prepare the global variables.

[xlib.c](https://gitlab.freedesktop.org/mesa/mesa/blob/master/src/gallium/targets/libgl-xlib/xlib.c)

``` c
/* Helper function to build a subset of a driver stack consisting of
 * one of the software rasterizers (llvmpipe, softpipe) and the
 * xlib winsys.
 */
static struct pipe_screen *
swrast_xlib_create_screen( Display *display )
{
   struct sw_winsys *winsys;
   struct pipe_screen *screen = NULL;

   /* Create the underlying winsys, which performs presents to Xlib
    * drawables:
    */
   winsys = xlib_create_sw_winsys( display );
   if (winsys == NULL)
      return NULL;

   /* Create a software rasterizer on top of that winsys:
    */
   screen = sw_screen_create( winsys );
   if (screen == NULL)
      goto fail;

   /* Inject any wrapping layers we want to here:
    */
   return debug_screen_wrap( screen );

fail:
   if (winsys)
      winsys->destroy( winsys );

   return NULL;
}

static struct xm_driver xlib_driver = 
{
   .create_pipe_screen = swrast_xlib_create_screen,
   .create_st_api = st_gl_api_create,
};


/* Build the rendering stack.
 */
static void _init( void ) __attribute__((constructor));
static void _init( void )
{
   /* Initialize the xlib libgl code, pass in the winsys:
    */
   xmesa_set_driver( &xlib_driver );
}
```

[sw_helper.h](https://gitlab.freedesktop.org/mesa/mesa/blob/master/src/gallium/auxiliary/targets-helpers/sw_helper.h)

``` c
static inline struct pipe_screen *
sw_screen_create_named(struct sw_winsys *winsys, const char *driver)
{
   struct pipe_screen *screen = NULL;

#if defined(GALLIUM_LLVMPIPE)
   if (screen == NULL && strcmp(driver, "llvmpipe") == 0)
      screen = llvmpipe_create_screen(winsys);
#endif

#if defined(GALLIUM_VIRGL)
   if (screen == NULL && strcmp(driver, "virpipe") == 0) {
      struct virgl_winsys *vws;
      vws = virgl_vtest_winsys_wrap(winsys);
      screen = virgl_create_screen(vws, NULL);
   }
#endif

#if defined(GALLIUM_SOFTPIPE)
   if (screen == NULL && strcmp(driver, "softpipe") == 0)
      screen = softpipe_create_screen(winsys);
#endif

#if defined(GALLIUM_SWR)
   if (screen == NULL && strcmp(driver, "swr") == 0)
      screen = swr_create_screen(winsys);
#endif

#if defined(GALLIUM_ZINK)
   if (screen == NULL && strcmp(driver, "zink") == 0)
      screen = zink_create_screen(winsys);
#endif

   return screen;
}


struct pipe_screen *
sw_screen_create(struct sw_winsys *winsys)
{
   const char *default_driver;
   const char *driver;

#if defined(GALLIUM_LLVMPIPE)
   default_driver = "llvmpipe";
#elif defined(GALLIUM_SOFTPIPE)
   default_driver = "softpipe";
#elif defined(GALLIUM_SWR)
   default_driver = "swr";
#elif defined(GALLIUM_SWR)
   default_driver = "zink";
#else
   default_driver = "";
#endif

   driver = debug_get_option("GALLIUM_DRIVER", default_driver);
   return sw_screen_create_named(winsys, driver);
}
```

[xm_api.c](https://gitlab.freedesktop.org/mesa/mesa/blob/master/src/gallium/state_tracker/glx/xlib/xm_api.c)

``` c
/* Driver interface routines, set up by xlib backend on library
 * _init().  These are global in the same way that function names are
 * global.
 */
static struct xm_driver driver;
static struct st_api *stapi;

void xmesa_set_driver( const struct xm_driver *templ )
{
   driver = *templ;
   stapi = driver.create_st_api();

   xmesa_strict_invalidate =
      debug_get_bool_option("XMESA_STRICT_INVALIDATE", FALSE);
}
```

[xm_public.h](https://gitlab.freedesktop.org/mesa/mesa/blob/master/src/gallium/state_tracker/glx/xlib/xm_public.h)

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
As we see, glx library's `init()` will set `xlib_driver.create_pipe_screen` to `swrast_xlib_create_screen` that return a `pipe_screen` to be set to the `st_manager->screen`. Eventually those two helper functions decide which gallium driver backend will be used by compilation macros.

## Q&A
#### libGL.so is not built until glx option is enabled in **meson_options.txt**.

Only with essential build-time dependencies for X11 installed and glx option configured is libGL.so built.

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

DRI and Gallium seem to be respectively different underlying implementation in Mesa. Moreover in term of swrast and i915, you have to choose either of both as you can read the following code snippet in meson.build. In fact DRI is more complicated and staler but Gallium is more smaller and simpler.

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
    * This means `gallium-xlib` is supposed to be a software implementation of GLX.
```
option(
  'glx',
  type : 'combo',
  value : 'xlib',
  choices : ['auto', 'disabled', 'dri', 'xlib', 'gallium-xlib'],
  description : 'Build support for GLX platform'
)
```
- OSMesa gallium requires gallium softpipe or llvmpipe
    * This means if `osmesa` is configured as `gallium`, `gallium-drivers` must include `swrast` but the `classic` osmesa uses the fixed-functioned TNL by default.
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
