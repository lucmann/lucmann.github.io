---
title: mesa中的winsys层
date: 2021-09-12 11:10:57
tags: Window System
categories: graphics
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

- `xcb_dri3_pixmap_from_buffer`

这个函数是由`c_client.py`自动生成的。

```
xcb_void_cookie_t
xcb_dri3_pixmap_from_buffer (xcb_connection_t *c,
                             xcb_pixmap_t      pixmap,
                             xcb_drawable_t    drawable,
                             uint32_t          size,
                             uint16_t          width,
                             uint16_t          height,
                             uint16_t          stride,
                             uint8_t           depth,
                             uint8_t           bpp,
                             int32_t           pixmap_fd)
{
    static const xcb_protocol_request_t xcb_req = {
        .count = 2,
        .ext = &xcb_dri3_id,
        .opcode = XCB_DRI3_PIXMAP_FROM_BUFFER,
        .isvoid = 1
    };

    struct iovec xcb_parts[4];
    xcb_void_cookie_t xcb_ret;
    xcb_dri3_pixmap_from_buffer_request_t xcb_out;
    int fds[1];
    int fd_index = 0;

    xcb_out.pixmap = pixmap;
    xcb_out.drawable = drawable;
    xcb_out.size = size;
    xcb_out.width = width;
    xcb_out.height = height;
    xcb_out.stride = stride;
    xcb_out.depth = depth;
    xcb_out.bpp = bpp;

    xcb_parts[2].iov_base = (char *) &xcb_out;
    xcb_parts[2].iov_len = sizeof(xcb_out);
    xcb_parts[3].iov_base = 0;
    xcb_parts[3].iov_len = -xcb_parts[2].iov_len & 3;

    fds[fd_index++] = pixmap_fd;
    xcb_ret.sequence = xcb_send_request_with_fds(c, 0, xcb_parts + 2, &xcb_req, 1, fds);
    return xcb_ret;
}
```

而X Server收到这个request后的处理函数如下:

```
int
dri3_pixmap_from_fds(PixmapPtr *ppixmap, ScreenPtr screen,
                     CARD8 num_fds, const int *fds,
                     CARD16 width, CARD16 height,
                     const CARD32 *strides, const CARD32 *offsets,
                     CARD8 depth, CARD8 bpp, CARD64 modifier)
{
    dri3_screen_priv_ptr        ds = dri3_screen_priv(screen);
    const dri3_screen_info_rec *info = ds->info;
    PixmapPtr                   pixmap;

    if (!info)
        return BadImplementation;

    if (info->version >= 2 && info->pixmap_from_fds != NULL) {
        pixmap = (*info->pixmap_from_fds) (screen, num_fds, fds, width, height,
                                           strides, offsets, depth, bpp, modifier);
    } else if (info->pixmap_from_fd != NULL && num_fds == 1) {
        pixmap = (*info->pixmap_from_fd) (screen, fds[0], width, height,
                                          strides[0], depth, bpp);
    } else {
        return BadImplementation;
    }

    if (!pixmap)
        return BadAlloc;

    *ppixmap = pixmap;
    return Success;
}
```

可以看到最终是调用了`pixmap_from_fds`或`pixmap_from_fd`, 接下来的工作由display GPU的kmd完成，最后当display GPU `gbm_bo_import`调用了`drmPrimeFDToHandle`函数后，X Server进程进入内核态，接下来内核DRM子系统的ioctl会调用`drm_prime_fd_to_handle_ioctl`, 它会调用display GPU的`prime_fd_to_handle` callback. 整个过程其实是将显存(buffer, 确切说是back buffer)抽象成dma-buf(dma-buf实际上是一个文件，所以它有fd, 可供在进程间传递)来实现用户应用进程与X Server进程间的Buffer共享。

{% asset_img winsys-Page-9.drawio.png "dma-buf" %}

下面是内核函数`dma_buf_get`的实现，从这个小函数的实现我们可以清楚地看到fd -> file -> dma_buf的转换

```c
/**
 * dma_buf_get - returns the struct dma_buf related to an fd
 * @fd:	[in]	fd associated with the struct dma_buf to be returned
 *
 * On success, returns the struct dma_buf associated with an fd; uses
 * file's refcounting done by fget to increase refcount. returns ERR_PTR
 * otherwise.
 */
struct dma_buf *dma_buf_get(int fd)
{
	struct file *file;

	file = fget(fd);

	if (!file)
		return ERR_PTR(-EBADF);

	if (!is_dma_buf_file(file)) {
		fput(file);
		return ERR_PTR(-EINVAL);
	}

	return file->private_data;
}
EXPORT_SYMBOL_GPL(dma_buf_get);
```

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
