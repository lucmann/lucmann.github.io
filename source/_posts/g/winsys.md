---
title: winsys in Mesa
date: 2021-09-12 11:10:57
tags: winsys
---

# winsys有什么用
winsys像一个桥梁，它要将GPU渲染的结果传输到窗口系统的framebuffer，由显示系统将其呈现在屏幕上。

<!--more-->

{% asset_img winsys-Page-2.drawio.png "winsys" %}


# mesa中的实现

mesa中winsys有两种实现: 一种是基于dri的，另一种是不基于dri的。两者的主要区别在以下几点:

- winsys提供的FBO(上图中的framebuffer)的来源

## 基于dri

- amdgpu_winsys
- radeon_winsys
- nouveau_winsys

基于dri的winsys的实现代码在`src/gallium/frontends/dri`目录

### 基于dri winsys的gallium driver的加载
所有使用基于dri的winsys的gallium drivers都会利用下面的宏声明和定义一个函数符号(除了swrast)

```
#define DEFINE_LOADER_DRM_ENTRYPOINT(drivername)                          \
const __DRIextension **__driDriverGetExtensions_##drivername(void);       \
PUBLIC const __DRIextension **__driDriverGetExtensions_##drivername(void) \
{                                                                         \
   globalDriverAPI = &galliumdrm_driver_api;                              \
   return galliumdrm_driver_extensions;                                   \
}
```

例如，`const __DRIextension **__driDriverGetExtensions_r600(void);`. 你可以在相应的动态库里查找到该符号。
上面宏中的`galliumdrm_driver_api`和`galliumdrm_driver_extensions`都是全局的。

- `galliumdrm_driver_api`

```
const struct __DriverAPIRec galliumdrm_driver_api = {
   .InitScreen = dri2_init_screen,
   .DestroyScreen = dri_destroy_screen,
   .CreateContext = dri_create_context,
   .DestroyContext = dri_destroy_context,
   .CreateBuffer = dri2_create_buffer,
   .DestroyBuffer = dri_destroy_buffer,
   .MakeCurrent = dri_make_current,
   .UnbindContext = dri_unbind_context,

   .AllocateBuffer = dri2_allocate_buffer,
   .ReleaseBuffer  = dri2_release_buffer,
};
```

- `galliumdrm_driver_extensions`

```
/* This is the table of extensions that the loader will dlsym() for. */
const __DRIextension *galliumdrm_driver_extensions[] = {
    &driCoreExtension.base,
    &driImageDriverExtension.base,
    &driDRI2Extension.base,
    &gallium_config_options.base,
    NULL
};
```

这里出现的4个数组元素都是`__DRIextension`的子类，同时他们都是全局的。例如`driImageDriverExtension`被定义在`src/mesa/drivers/dri/common/dri_util.c`文件中:

```
/** Image driver interface */
const __DRIimageDriverExtension driImageDriverExtension = {
    .base = { __DRI_IMAGE_DRIVER, 1 },

    .createNewScreen2           = driCreateNewScreen2,
    .createNewDrawable          = driCreateNewDrawable,
    .getAPIMask                 = driGetAPIMask,
    .createContextAttribs       = driCreateContextAttribs,
};
```

除了这个`driImageDriverExtension`，基于dri的winsys还定义了一个与`__DRIimage`有关的`__DRIextension`:

```
/* The image loader extension record for DRI3
 */
static const __DRIimageLoaderExtension imageLoaderExtension = {
   .base = { __DRI_IMAGE_LOADER, 3 },

   .getBuffers          = loader_dri3_get_buffers,
   .flushFrontBuffer    = dri3_flush_front_buffer,
   .flushSwapBuffers    = dri3_flush_swap_buffers,
};
```

这个所谓的image loader extension是在多GPU的场景下会用到，也就是当系统中同时存在两个GPU时，可以通过设置mesa提供的环境变量`DRI_PRIME`，启用PRIME模式，此时其中一个GPU作为Display GPU(server GPU), 用做X11显示，另外一个GPU作为Render GPU, 用做3D渲染，当我们申请`__DRI_IMAGE_BUFFER_FRONT`时，我们不能直接使用pixmap,
因为所有pixmaps都是被server GPU所有，这种pixmap的格式可能不能被render GPU所理解。这种情况下，在`loader_dri3_get_buffers`中，会在render GPU的VRAM里创建一个Fake Front Buffer, 最后`dri3_flush_front_buffer`会将Fake Front Buffer的内容同步到真正的Front Buffer，即display GPU的pixmap里。

### 基于dri winsys的gallium driver的winsys实现
winsys是一个桥梁，它主要要实现的就是将color buffer传输到窗口系统的framebuffer.那么基于dri的winsys是怎么实现这个桥梁的? 它主要依赖下面两个对象:

- DRIimage
    * `__DRIimageRec`
    * `__DRIimageExtensionRec`

DRIimage通过调用`pipe_screen.resource_create`函数创建，创建后还需要queryImage一些信息，这些信息为X11所需要。

- PIPE_RESOURCE_PARAM_NPLANES
- PIPE_RESOURCE_PARAM_STRIDE
- PIPE_RESOURCE_PARAM_OFFSET
- PIPE_RESOURCE_PARAM_MODIFIER
- PIPE_RESOURCE_PARAM_HANDLE_TYPE_FD

`PIPE_RESOURCE_PARAM_HANDLE_TYPE_FD`通过向kernel发送下面请求，由一个gem handle返回一个file descriptor. 有了这个FD, 不同的进程间就可以共享内存。比方，当xserver获取到这个FD后，它可以通过`DRM_IOCTL_PRIME_FD_TO_HANDLE`又重新获取到gem bo的handle.

```
int
panfrost_bo_export(struct panfrost_bo *bo)
{
        struct drm_prime_handle args = {
                .handle = bo->gem_handle,
                .flags = DRM_CLOEXEC,
        };

        int ret = drmIoctl(bo->dev->fd, DRM_IOCTL_PRIME_HANDLE_TO_FD, &args);
        if (ret == -1)
                return -1;

        bo->flags |= PAN_BO_SHARED;
        return args.fd;
}
```

- X11 present extension

## 非dri

- sw_winsys

非dri的winsys的实现代码在`src/gallium/winsys/sw/xlib`目录

不同的winsys实际上是不同的接口(函数)，下面以sw_winsys为例说明一下mesa里的winsys的接口与实现。

<table>
<thead>
<tr>
    <th>接口</th>
    <th>实现</th>
    <th>功能</th>
</tr>
</thead>
<tbody>
<tr>
    <td>void<br>(*destroy)(struct sw_winsys *ws);</td>
    <td>xlib_displaytarget_destroy</td>
    <td>销毁这个winsys</td>
</tr>
<tr>
    <td></td>
    <td></td>
    <td></td>
</tr>
<tr>
    <td></td>
    <td></td>
    <td></td>
</tr>
<tr>
    <td></td>
    <td></td>
    <td></td>
</tr>
<tr>
    <td></td>
    <td></td>
    <td></td>
</tr>
<tr>
    <td></td>
    <td></td>
    <td></td>
</tr>
<tr>
    <td></td>
    <td></td>
    <td></td>
</tr>
<tr>
    <td></td>
    <td></td>
    <td></td>
</tr>
<tr>
    <td></td>
    <td></td>
    <td></td>
</tr>
<tr>
    <td></td>
    <td></td>
    <td></td>
</tr>
<tr>
    <td></td>
    <td></td>
    <td></td>
</tr>
</tbody>
</table>
