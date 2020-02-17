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
If you want to know the full graphic stack of an OpenGL demo, you can not get rid of the window system. That is why I will try some GLX demos. Evidently GLX demos must depend on X11. You can cope with this problem by installing [vcXsrv](https://sourceforge.net/projects/vcxsrv/) on the Windows 10 which hosts your WSL. This time I still choose the gallium-xlib with softpipe. The following call graph shows the path that GLX context is created.

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
#elif defined(GALLIUM_SWR)
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

## Gallium Genbu(customized)
To verify the analysis above we will try to add a customized gallium driver named **genbu** as a clone from the existing softpipe based GLX. All that we will do is three parts work below.

- key data structures: genbu_screen, genbu_context, the related interfaces and definitions 
- helper functions for loading software rasterizers for GLX
- build scripts

### Added or Modified Sources and Build Scripts
``` text
meson.build
meson_options.txt
src/gallium/auxiliary/target-helpers/inline_sw_helper.h
src/gallium/drivers/genbu/genbu_limits.h
src/gallium/drivers/genbu/genbu_public.h
src/gallium/drivers/genbu/genbu_screen.c
src/gallium/drivers/genbu/genbu_screen.h
src/gallium/drivers/genbu/genbu_texture.h
src/gallium/drivers/genbu/meson.build
src/gallium/targets/libgl-xlib/meson.build
src/gallium/meson.build
```

``` markdown
build/src/gallium/
├── auxiliary
│   ├── eb820e8@@gallium@sta
│   │   ├── cso_cache_cso_cache.c.o
│   │   ├── cso_cache_cso_context.c.o
│   │   ├── cso_cache_cso_hash.c.o
│   │   ├── draw_draw_context.c.o
│   │   ├── draw_draw_fs.c.o
│   │   ├── draw_draw_gs.c.o
│   │   ├── draw_draw_pipe.c.o
│   │   ├── draw_draw_pipe_aaline.c.o
│   │   ├── draw_draw_pipe_aapoint.c.o
│   │   ├── draw_draw_pipe_clip.c.o
│   │   ├── draw_draw_pipe_cull.c.o
│   │   ├── draw_draw_pipe_flatshade.c.o
│   │   ├── draw_draw_pipe_offset.c.o
│   │   ├── draw_draw_pipe_pstipple.c.o
│   │   ├── draw_draw_pipe_stipple.c.o
│   │   ├── draw_draw_pipe_twoside.c.o
│   │   ├── draw_draw_pipe_unfilled.c.o
│   │   ├── draw_draw_pipe_util.c.o
│   │   ├── draw_draw_pipe_validate.c.o
│   │   ├── draw_draw_pipe_vbuf.c.o
│   │   ├── draw_draw_pipe_wide_line.c.o
│   │   ├── draw_draw_pipe_wide_point.c.o
│   │   ├── draw_draw_prim_assembler.c.o
│   │   ├── draw_draw_pt.c.o
│   │   ├── draw_draw_pt_emit.c.o
│   │   ├── draw_draw_pt_fetch.c.o
│   │   ├── draw_draw_pt_fetch_emit.c.o
│   │   ├── draw_draw_pt_fetch_shade_emit.c.o
│   │   ├── draw_draw_pt_fetch_shade_pipeline.c.o
│   │   ├── draw_draw_pt_post_vs.c.o
│   │   ├── draw_draw_pt_so_emit.c.o
│   │   ├── draw_draw_pt_util.c.o
│   │   ├── draw_draw_pt_vsplit.c.o
│   │   ├── draw_draw_vertex.c.o
│   │   ├── draw_draw_vs.c.o
│   │   ├── draw_draw_vs_exec.c.o
│   │   ├── draw_draw_vs_variant.c.o
│   │   ├── driver_ddebug_dd_context.c.o
│   │   ├── driver_ddebug_dd_draw.c.o
│   │   ├── driver_ddebug_dd_screen.c.o
│   │   ├── driver_noop_noop_pipe.c.o
│   │   ├── driver_noop_noop_state.c.o
│   │   ├── driver_rbug_rbug_context.c.o
│   │   ├── driver_rbug_rbug_core.c.o
│   │   ├── driver_rbug_rbug_objects.c.o
│   │   ├── driver_rbug_rbug_screen.c.o
│   │   ├── driver_trace_tr_context.c.o
│   │   ├── driver_trace_tr_dump.c.o
│   │   ├── driver_trace_tr_dump_state.c.o
│   │   ├── driver_trace_tr_screen.c.o
│   │   ├── driver_trace_tr_texture.c.o
│   │   ├── hud_font.c.o
│   │   ├── hud_hud_context.c.o
│   │   ├── hud_hud_cpu.c.o
│   │   ├── hud_hud_cpufreq.c.o
│   │   ├── hud_hud_diskstat.c.o
│   │   ├── hud_hud_driver_query.c.o
│   │   ├── hud_hud_fps.c.o
│   │   ├── hud_hud_nic.c.o
│   │   ├── hud_hud_sensors_temp.c.o
│   │   ├── indices_u_primconvert.c.o
│   │   ├── meson-generated_.._u_indices_gen.c.o
│   │   ├── meson-generated_.._u_unfilled_gen.c.o
│   │   ├── nir_nir_draw_helpers.c.o
│   │   ├── nir_tgsi_to_nir.c.o
│   │   ├── os_os_process.c.o
│   │   ├── pipebuffer_pb_buffer_fenced.c.o
│   │   ├── pipebuffer_pb_bufmgr_cache.c.o
│   │   ├── pipebuffer_pb_bufmgr_debug.c.o
│   │   ├── pipebuffer_pb_bufmgr_mm.c.o
│   │   ├── pipebuffer_pb_bufmgr_slab.c.o
│   │   ├── pipebuffer_pb_cache.c.o
│   │   ├── pipebuffer_pb_slab.c.o
│   │   ├── pipebuffer_pb_validate.c.o
│   │   ├── postprocess_pp_celshade.c.o
│   │   ├── postprocess_pp_colors.c.o
│   │   ├── postprocess_pp_init.c.o
│   │   ├── postprocess_pp_mlaa.c.o
│   │   ├── postprocess_pp_program.c.o
│   │   ├── postprocess_pp_run.c.o
│   │   ├── rbug_rbug_connection.c.o
│   │   ├── rbug_rbug_context.c.o
│   │   ├── rbug_rbug_core.c.o
│   │   ├── rbug_rbug_demarshal.c.o
│   │   ├── rbug_rbug_shader.c.o
│   │   ├── rbug_rbug_texture.c.o
│   │   ├── renderonly_renderonly.c.o
│   │   ├── rtasm_rtasm_cpu.c.o
│   │   ├── rtasm_rtasm_execmem.c.o
│   │   ├── rtasm_rtasm_x86sse.c.o
│   │   ├── tgsi_tgsi_aa_point.c.o
│   │   ├── tgsi_tgsi_build.c.o
│   │   ├── tgsi_tgsi_dump.c.o
│   │   ├── tgsi_tgsi_emulate.c.o
│   │   ├── tgsi_tgsi_exec.c.o
│   │   ├── tgsi_tgsi_from_mesa.c.o
│   │   ├── tgsi_tgsi_info.c.o
│   │   ├── tgsi_tgsi_iterate.c.o
│   │   ├── tgsi_tgsi_lowering.c.o
│   │   ├── tgsi_tgsi_parse.c.o
│   │   ├── tgsi_tgsi_point_sprite.c.o
│   │   ├── tgsi_tgsi_sanity.c.o
│   │   ├── tgsi_tgsi_scan.c.o
│   │   ├── tgsi_tgsi_strings.c.o
│   │   ├── tgsi_tgsi_text.c.o
│   │   ├── tgsi_tgsi_transform.c.o
│   │   ├── tgsi_tgsi_two_side.c.o
│   │   ├── tgsi_tgsi_ureg.c.o
│   │   ├── tgsi_tgsi_util.c.o
│   │   ├── translate_translate.c.o
│   │   ├── translate_translate_cache.c.o
│   │   ├── translate_translate_generic.c.o
│   │   ├── translate_translate_sse.c.o
│   │   ├── util_u_async_debug.c.o
│   │   ├── util_u_bitmask.c.o
│   │   ├── util_u_blit.c.o
│   │   ├── util_u_blitter.c.o
│   │   ├── util_u_cache.c.o
│   │   ├── util_u_compute.c.o
│   │   ├── util_u_debug_describe.c.o
│   │   ├── util_u_debug_flush.c.o
│   │   ├── util_u_debug_gallium.c.o
│   │   ├── util_u_debug_image.c.o
│   │   ├── util_u_debug_memory.c.o
│   │   ├── util_u_debug_refcnt.c.o
│   │   ├── util_u_debug_stack.c.o
│   │   ├── util_u_debug_symbol.c.o
│   │   ├── util_u_dl.c.o
│   │   ├── util_u_draw.c.o
│   │   ├── util_u_draw_quad.c.o
│   │   ├── util_u_dump_defines.c.o
│   │   ├── util_u_dump_state.c.o
│   │   ├── util_u_framebuffer.c.o
│   │   ├── util_u_gen_mipmap.c.o
│   │   ├── util_u_handle_table.c.o
│   │   ├── util_u_hash_table.c.o
│   │   ├── util_u_helpers.c.o
│   │   ├── util_u_idalloc.c.o
│   │   ├── util_u_index_modify.c.o
│   │   ├── util_u_linear.c.o
│   │   ├── util_u_live_shader_cache.c.o
│   │   ├── util_u_log.c.o
│   │   ├── util_u_network.c.o
│   │   ├── util_u_prim.c.o
│   │   ├── util_u_prim_restart.c.o
│   │   ├── util_u_pstipple.c.o
│   │   ├── util_u_resource.c.o
│   │   ├── util_u_sampler.c.o
│   │   ├── util_u_screen.c.o
│   │   ├── util_u_simple_shaders.c.o
│   │   ├── util_u_split_draw.c.o
│   │   ├── util_u_suballoc.c.o
│   │   ├── util_u_surface.c.o
│   │   ├── util_u_surfaces.c.o
│   │   ├── util_u_tests.c.o
│   │   ├── util_u_texture.c.o
│   │   ├── util_u_threaded_context.c.o
│   │   ├── util_u_tile.c.o
│   │   ├── util_u_transfer.c.o
│   │   ├── util_u_transfer_helper.c.o
│   │   ├── util_u_upload_mgr.c.o
│   │   └── util_u_vbuf.c.o
│   ├── libgallium.a
│   ├── pipe-loader
│   ├── u_indices_gen.c
│   └── u_unfilled_gen.c
├── drivers
│   └── genbu
│       ├── 1e0fe92@@genbu@sta
│       │   ├── genbu_context.c.o
│       │   └── genbu_screen.c.o
│       └── libgenbu.a
├── state_trackers
│   └── glx
│       └── xlib
│           ├── 5534cc1@@xlib@sta
│           │   ├── glx_api.c.o
│           │   ├── glx_getproc.c.o
│           │   ├── glx_usefont.c.o
│           │   ├── xm_api.c.o
│           │   └── xm_st.c.o
│           └── libxlib.a
├── targets
│   └── libgl-xlib
│       ├── a6bea21@@GL@sha
│       │   └── xlib.c.o
│       ├── libGL.so -> libGL.so.1
│       ├── libGL.so.1 -> libGL.so.1.5.0
│       └── libGL.so.1.5.0
└── winsys
    └── sw
        ├── null
        ├── wrapper
        └── xlib
            ├── 5513208@@ws_xlib@sta
            │   └── xlib_sw_winsys.c.o
            └── libws_xlib.a
```

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
