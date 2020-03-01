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
    - libxdamage-dev (if glx option is dri)
    - libxfixes-dev (if glx option is dri)
    - libxcb-glx-dev (if glx option is dri)
    - libxcb-dri2-dev (if glx option is dri)
    - libxxf86vm-dev (if glx option is dri)

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

## Gallium-Based GLX Demos
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
 *
 * NOTE: The obsolete symbols _init and _fini
 *
 * signature:
 * void _init(void);
 * void _fini(void);
 *
 * The linker recoginizes special symbols _init and _fini. If a dynamic library
 * exports a routine named _init, then that code is executed after the loading,
 * before dlopen() returns. If the dynamic library exports a routine named _fini,
 * then that routine is called just before the library is unloaded. In case you
 * need to avoid linking against the system startup files,this can be done by
 * giving gcc the "-nostartfiles" parameter on the command line.
 * Using these routines, or the gcc -nostartfiles of -nostdlib options, is not
 * recommended. Their use may result in undesired behavior, since the constructor/
 * destructor routines will not be executed(unless special measures are taken).
 * Instead, libraries should export routines using the __attribute__((constructor))
 * and __attribute__((destructor)) function attributes. Constructor routines are
 * executed before dlopen() returns, and destructor routines ared executed before
 * dlclose() returns.
 */
static void _init( void ) __attribute__((constructor));
static void _init( void )
{
   /* Initialize the xlib libgl code, pass in the winsys:
    */
   xmesa_set_driver( &xlib_driver );
}
```

where define the `xlib_driver` and set by `_init()`.

[sw_helper.h](https://gitlab.freedesktop.org/mesa/mesa/blob/master/src/gallium/auxiliary/target-helpers/sw_helper.h)

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
#elif defined(GALLIUM_ZINK)
   default_driver = "zink";
#else
   default_driver = "";
#endif

   driver = debug_get_option("GALLIUM_DRIVER", default_driver);
   return sw_screen_create_named(winsys, driver);
}
```

[xm_api.c](https://gitlab.freedesktop.org/mesa/mesa/blob/master/src/gallium/state_trackers/glx/xlib/xm_api.c)

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

static XMesaDisplay
xmesa_init_display( Display *display )
{
   static mtx_t init_mutex = _MTX_INITIALIZER_NP;
   XMesaDisplay xmdpy;
   XMesaExtDisplayInfo *info;

   if (display == NULL) {
      return NULL;
   }

   mtx_lock(&init_mutex);

   /* Look for XMesaDisplay which corresponds to this display */
   info = MesaExtInfo.head;
   while(info) {
      if (info->display == display) {
         /* Found it */
         mtx_unlock(&init_mutex);
         return  &info->mesaDisplay;
      }
      info = info->next;
   }

   /* Not found.  Create new XMesaDisplay */
   /* first allocate X-related resources and hook destroy callback */

   /* allocate mesa display info */
   info = (XMesaExtDisplayInfo *) Xmalloc(sizeof(XMesaExtDisplayInfo));
   if (info == NULL) {
      mtx_unlock(&init_mutex);
      return NULL;
   }
   info->display = display;

   xmdpy = &info->mesaDisplay; /* to be filled out below */
   xmdpy->display = display;
   xmdpy->pipe = NULL;

   xmdpy->smapi = CALLOC_STRUCT(st_manager);
   if (!xmdpy->smapi) {
      Xfree(info);
      mtx_unlock(&init_mutex);
      return NULL;
   }

   xmdpy->screen = driver.create_pipe_screen(display);
   if (!xmdpy->screen) {
      free(xmdpy->smapi);
      Xfree(info);
      mtx_unlock(&init_mutex);
      return NULL;
   }

   /* At this point, both smapi and screen are known to be valid */
   xmdpy->smapi->screen = xmdpy->screen;
   xmdpy->smapi->get_param = xmesa_get_param;
   (void) mtx_init(&xmdpy->mutex, mtx_plain);

   /* chain to the list of displays */
   _XLockMutex(_Xglobal_lock);
   info->next = MesaExtInfo.head;
   MesaExtInfo.head = info;
   MesaExtInfo.ndisplays++;
   _XUnlockMutex(_Xglobal_lock);

   mtx_unlock(&init_mutex);

   return xmdpy;
}
```

where `driver.create_pipe_screen(display)` is instantiated as `xlib_driver.swrast_xlib_create_screen`. As we see, the dynamic library routine `_init()` will set `xlib_driver.create_pipe_screen` to `swrast_xlib_create_screen` that return a `pipe_screen` to be set to the `st_manager->screen`. Eventually those two helper functions decide which gallium driver backend will be used by compilation macros.

## Gallium Genbu
To verify the analysis above we will try to add a customized gallium driver named **genbu** as a clone from the existing softpipe based GLX. All that we will do is three parts work below.

- key data structures: genbu_screen, genbu_context, the related callbacks and definitions 
- helper functions for loading software rasterizers for GLX
- build scripts

### [Added or Modified Sources and Build Scripts](https://gitlab.freedesktop.org/lucmaa/mesa/tree/gallium-gb-1.0)

### Trace

| *belong* | *hook* | *callback* | *caller* | *X/GL API* |
|-|-|-|-|-|
| pipe_screen | resource_create | softpipe_resource_create | bufferobj_data | |
| pipe_context | transfer_map | softpipe_transfer_map | | |
| pipe_context | create_surface | softpipe_create_surface | st_framebuffer_validate | |
| pipe_context | flush | softpipe_flush_wrapped | st_context_flush | glXSwapBuffers |
| pipe_context | clear | softpipe_clear | st_Clear | glClear |
| pipe_context | draw_vbo | softpipe_draw_vbo | st_draw_vbo | glCallList |
| pipe_context | draw_vbo | softpipe_draw_vbo | st_draw_vbo | glCallList |


## DRI-Based GLX Demos

### Loading

- `__glXInitialize`
- `driOpenDriver`

``` c
#if defined(GLX_DIRECT_RENDERING) && !defined(GLX_USE_APPLEGL)
   glx_direct = !env_var_as_boolean("LIBGL_ALWAYS_INDIRECT", false);
   glx_accel = !env_var_as_boolean("LIBGL_ALWAYS_SOFTWARE", false);

   dpyPriv->drawHash = __glxHashCreate();

   /*
    ** Initialize the direct rendering per display data and functions.
    ** Note: This _must_ be done before calling any other DRI routines
    ** (e.g., those called in AllocAndFetchScreenConfigs).
    */
#if defined(GLX_USE_DRM)
   if (glx_direct && glx_accel) {
#if defined(HAVE_DRI3)
      if (!env_var_as_boolean("LIBGL_DRI3_DISABLE", false))
         dpyPriv->dri3Display = dri3_create_display(dpy);
#endif /* HAVE_DRI3 */
      dpyPriv->dri2Display = dri2CreateDisplay(dpy);
      dpyPriv->driDisplay = driCreateDisplay(dpy);
   }
#endif /* GLX_USE_DRM */
   if (glx_direct)
      dpyPriv->driswDisplay = driswCreateDisplay(dpy);
#endif /* GLX_DIRECT_RENDERING && !GLX_USE_APPLEGL */

#ifdef GLX_USE_APPLEGL
   if (!applegl_create_display(dpyPriv)) {
      free(dpyPriv);
      return NULL;
   }
#endif

#ifdef GLX_USE_WINDOWSGL
   if (glx_direct && glx_accel)
      dpyPriv->windowsdriDisplay = driwindowsCreateDisplay(dpy);
#endif

   if (!AllocAndFetchScreenConfigs(dpy, dpyPriv)) {
      free(dpyPriv);
      return NULL;
   }
```

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


``` c
/**
 * This is a constructor function for the megadriver dynamic library.
 *
 * When the driver is dlopen'ed, this function will run. It will
 * search for the name of the foo_dri.so file that was opened using
 * the dladdr function.
 *
 * After finding foo's name, it will call __driDriverGetExtensions_foo
 * and use the return to update __driDriverExtensions to enable
 * compatibility with older DRI driver loaders.
 */
__attribute__((constructor)) static void
megadriver_stub_init(void);
```

``` c
/*
 * Allocate, initialize and return a __DRIdisplayPrivate object.
 * This is called from __glXInitialize() when we are given a new
 * display pointer.
 */
_X_HIDDEN __GLXDRIdisplay *
driCreateDisplay(Display * dpy)
{
   struct dri_display *pdpyp;
   int eventBase, errorBase;
   int major, minor, patch;

   if (!XF86DRIQueryExtension(dpy, &eventBase, &errorBase)) {
      return NULL;
   }

   if (!XF86DRIQueryVersion(dpy, &major, &minor, &patch)) {
      return NULL;
   }

   pdpyp = malloc(sizeof *pdpyp);
   if (!pdpyp) {
      return NULL;
   }

   pdpyp->driMajor = major;
   pdpyp->driMinor = minor;
   pdpyp->driPatch = patch;

   pdpyp->base.destroyDisplay = driDestroyDisplay;
   pdpyp->base.createScreen = driCreateScreen;

   return &pdpyp->base;
}
```

## Contexts
There are a variety of **contexts** in Mesa. The biggest one, **`gl_context`**, is commented as
{% blockquote %}
This is the central context data structure for Mesa. Almost all OpenGL state is contained in this structure. Think of this as a base class from which device drivers will derive sub classes.
{% endblockquote %}
Apart from OpenGL state it contains several other contexts

- `swrast_context`
- `swsetup_context`
- `swtnl_context`
- `vbo_context`
- `st_context`

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

## Q&A
#### When xlib creates pipe screen, *only* software rasterizers or pipes'screen are created. And llvmpipe, softpipe, virgl, swr, unexceptionally, are software rasterizers or virtual GPU. [Zink](https://www.collabora.com/news-and-blog/blog/2018/10/31/introducing-zink-opengl-implementation-vulkan/) is, in brief, a translator from OpenGL to Vulkan and implemented as Gallium driver. So why only software pipes?

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

#### When `__glXInitialize` creates the `Display`, **only** `driswCreateDisplay` returns successfully. Both of `dri2CreateDisplay` and `driCreateDisplay` failed.

- env: WSL on Windows 10 and with vcXsrv installed on the host as X server

The cause of failure is that vcXsrv has no extensions with DRI or DRI2. This lack of X server extension fails `DRI2QueryExtension` and `XF86DRIQueryExtension` so that the loading of gallium driver is not invoked.

``` c
/*
 * XextAddDisplay - add a display to this extension
 */
XExtDisplayInfo *XextAddDisplay (
    XExtensionInfo *extinfo,
    Display *dpy,
    _Xconst char *ext_name,
    XExtensionHooks *hooks,
    int nevents,
    XPointer data)
{
    XExtDisplayInfo *dpyinfo;

    dpyinfo = (XExtDisplayInfo *) Xmalloc (sizeof (XExtDisplayInfo));
    if (!dpyinfo) return NULL;
    dpyinfo->display = dpy;
    dpyinfo->data = data;
    dpyinfo->codes = XInitExtension (dpy, ext_name);

    /*
     * if the server has the extension, then we can initialize the
     * appropriate function vectors
     */
    if (dpyinfo->codes) {
	int i, j;

	for (i = 0, j = dpyinfo->codes->first_event; i < nevents; i++, j++) {
	    XESetWireToEvent (dpy, j, hooks->wire_to_event);
	    XESetEventToWire (dpy, j, hooks->event_to_wire);
	}

        /* register extension for XGE */
        if (strcmp(ext_name, GE_NAME))
            xgeExtRegister(dpy, dpyinfo->codes->major_opcode, hooks);

	if (hooks->create_gc)
	  XESetCreateGC (dpy, dpyinfo->codes->extension, hooks->create_gc);
	if (hooks->copy_gc)
	  XESetCopyGC (dpy, dpyinfo->codes->extension, hooks->copy_gc);
	if (hooks->flush_gc)
	  XESetFlushGC (dpy, dpyinfo->codes->extension, hooks->flush_gc);
	if (hooks->free_gc)
	  XESetFreeGC (dpy, dpyinfo->codes->extension, hooks->free_gc);
	if (hooks->create_font)
	  XESetCreateFont (dpy, dpyinfo->codes->extension, hooks->create_font);
	if (hooks->free_font)
	  XESetFreeFont (dpy, dpyinfo->codes->extension, hooks->free_font);
	if (hooks->close_display)
	  XESetCloseDisplay (dpy, dpyinfo->codes->extension,
			     hooks->close_display);
	if (hooks->error)
	  XESetError (dpy, dpyinfo->codes->extension, hooks->error);
	if (hooks->error_string)
	  XESetErrorString (dpy, dpyinfo->codes->extension,
			    hooks->error_string);
    } else if (hooks->close_display) {
	/* The server doesn't have this extension.
	 * Use a private Xlib-internal extension to hang the close_display
	 * hook on so that the "cache" (extinfo->cur) is properly cleaned.
	 * (XBUG 7955)
	 */
	XExtCodes *codes = XAddExtension(dpy);
	if (!codes) {
	    XFree(dpyinfo);
	    return NULL;
	}
	XESetCloseDisplay (dpy, codes->extension, hooks->close_display);
    }

    /*
     * now, chain it onto the list
     */
    _XLockMutex(_Xglobal_lock);
    dpyinfo->next = extinfo->head;
    extinfo->head = dpyinfo;
    extinfo->cur = dpyinfo;
    extinfo->ndisplays++;
    _XUnlockMutex(_Xglobal_lock);
    return dpyinfo;
}
```

