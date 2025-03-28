---
title: DRI Legacy
date: 2024-09-13 14:44:56
tags: [Mesa]
categories: graphics
---

# Just read it

[gallium/dril: Compatibility stub for the legacy DRI loader interface](https://gitlab.freedesktop.org/mesa/mesa/-/merge_requests/28378)

<!--more-->

# DRI extensions

```
#define __DRI2_BLOB                        "DRI2_Blob"
#define __DRI2_BUFFER_DAMAGE               "DRI2_BufferDamage"
#define __DRI2_CONFIG_QUERY                "DRI_CONFIG_QUERY"
#define __DRI2_FENCE                       "DRI2_Fence"
#define __DRI2_FLUSH                       "DRI2_Flush"
#define __DRI2_FLUSH_CONTROL               "DRI_FlushControl"
#define __DRI2_INTEROP                     "DRI2_Interop"
#define __DRI2_NO_ERROR                    "DRI_NoError"
#define __DRI2_RENDERER_QUERY              "DRI_RENDERER_QUERY"
#define __DRI2_ROBUSTNESS                  "DRI_Robustness"
#define __DRI2_THROTTLE                    "DRI2_Throttle"
#define __DRI_BACKGROUND_CALLABLE          "DRI_BackgroundCallable"
#define __DRI_CONFIG_OPTIONS               "DRI_ConfigOptions"
#define __DRI_COPY_SUB_BUFFER              "DRI_CopySubBuffer"
#define __DRI_CORE                         "DRI_Core"
#define __DRI_DRI2                         "DRI_DRI2"
#define __DRI_DRI2_LOADER                  "DRI_DRI2Loader"
#define __DRI_IMAGE                        "DRI_IMAGE"
#define __DRI_IMAGE_DRIVER                 "DRI_IMAGE_DRIVER"
#define __DRI_IMAGE_LOADER                 "DRI_IMAGE_LOADER"
#define __DRI_IMAGE_LOOKUP                 "DRI_IMAGE_LOOKUP"
#define __DRI_MEDIA_STREAM_COUNTER         "DRI_MediaStreamCounter"
#define __DRI_MUTABLE_RENDER_BUFFER_DRIVER "DRI_MutableRenderBufferDriver"
#define __DRI_MUTABLE_RENDER_BUFFER_LOADER "DRI_MutableRenderBufferLoader"
#define __DRI_READ_DRAWABLE                "DRI_ReadDrawable"
#define __DRI_SWAP_CONTROL                 "DRI_SwapControl"
#define __DRI_SWRAST                       "DRI_SWRast"
#define __DRI_SWRAST_LOADER                "DRI_SWRastLoader"
#define __DRI_SYSTEM_TIME                  "DRI_SystemTime"
#define __DRI_TEX_BUFFER                   "DRI_TexBuffer"
#define __DRI_USE_INVALIDATE               "DRI_UseInvalidate"
```
(extracted by `rg '^#define __DRI.* "DRI.*"' include/GL/internal/dri_interface.h | sort | awk '{printf("%s %-34s %s\n", $1, $2, $3)}'`)

这些扩展被定义在 `dri_interface.h`, 该文件有两个作用：
- 它是 mesa 内部的 libGL 与 libgallium_dri 之间的约定
- 它是 mesa 与外部 X.org 服务器家族的约定

# ~~LIBGL_DRIVERS_PATH~~

```
       14  armada-drm_dri.so -> libdril_dri.so
       14  exynos_dri.so -> libdril_dri.so
       14  gm12u320_dri.so -> libdril_dri.so
       14  hdlcd_dri.so -> libdril_dri.so
       14  hx8357d_dri.so -> libdril_dri.so
       14  ili9163_dri.so -> libdril_dri.so
       14  ili9225_dri.so -> libdril_dri.so
       14  ili9341_dri.so -> libdril_dri.so
       14  ili9486_dri.so -> libdril_dri.so
       14  imx-dcss_dri.so -> libdril_dri.so
       14  imx-drm_dri.so -> libdril_dri.so
       14  imx-lcdif_dri.so -> libdril_dri.so
       14  ingenic-drm_dri.so -> libdril_dri.so
       14  kirin_dri.so -> libdril_dri.so
       14  kms_swrast_dri.so -> libdril_dri.so
       14  komeda_dri.so -> libdril_dri.so
  1151904  libdril_dri.so
124969400  libgallium.so
       14  mali-dp_dri.so -> libdril_dri.so
       14  mcde_dri.so -> libdril_dri.so
       14  mediatek_dri.so -> libdril_dri.so
       14  meson_dri.so -> libdril_dri.so
       14  mi0283qt_dri.so -> libdril_dri.so
       14  mxsfb-drm_dri.so -> libdril_dri.so
       14  panel-mipi-dbi_dri.so -> libdril_dri.so
       14  panfrost_dri.so -> libdril_dri.so
       14  panthor_dri.so -> libdril_dri.so
       14  pl111_dri.so -> libdril_dri.so
       14  radeonsi_dri.so -> libdril_dri.so
       14  rcar-du_dri.so -> libdril_dri.so
       14  repaper_dri.so -> libdril_dri.so
       14  rockchip_dri.so -> libdril_dri.so
       14  rzg2l-du_dri.so -> libdril_dri.so
       14  ssd130x_dri.so -> libdril_dri.so
       14  st7586_dri.so -> libdril_dri.so
       14  st7735r_dri.so -> libdril_dri.so
       14  sti_dri.so -> libdril_dri.so
       14  stm_dri.so -> libdril_dri.so
       14  sun4i-drm_dri.so -> libdril_dri.so
       14  swrast_dri.so -> libdril_dri.so
       14  udl_dri.so -> libdril_dri.so
       14  vkms_dri.so -> libdril_dri.so
       14  zink_dri.so -> libdril_dri.so
       14  zynqmp-dpsub_dri.so -> libdril_dri.so
```
(generated by `ls -go --time-style=+ /home/luc/mesa-install/lib/x86_64-linux-gnu/dri | cut -c 14-`)

# GL Frontends

- GLX
- EGL
- GBM

在有了 DRIL 之后，这些 GL 前端库可以直接链接 `libgallium.so`
