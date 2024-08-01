---
title: dri3_alloc_render_buffer
date: 2024-07-30 21:21:27
tags: DRI
categories: graphics
---

[`mesa/src/loader/loader_dri3_helper.c::dri3_alloc_render_buffer()`](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/loader/loader_dri3_helper.c#L1435), 这个函数不长，但却涉及到了DRI框架的许多重要概念，buffer共享(DMABUF), GPU offload (PRIME), buffer同步(X client和server)，送显(present), modifiers等等，所以非常值得深入分析。

<!--more-->

```
static struct loader_dri3_buffer *
dri3_alloc_render_buffer(struct loader_dri3_drawable *draw,
                         unsigned int fourcc,
                         int width, int height, int depth);
```

# 同步

- 当我们谈论 X client 和 server 之间的 Buffer 同步时是在说什么？

在DRI3扩展下, render buffer (BO作为GPU 的render target) 是一开始由X client (例如一个 3D App)创建的(可能不止一个), render buffer 创建好后随即会通过 __DRIimageExtension 的 `queryImage()` 查询到该buffer 的 FD (drmPrimeHandleToFD, 后面会将该 FD 传送给 X server), 而在 X 的 compositor, 拿到 GPU 的渲染结果实际上就是通过该 FD (drmPrimeFDToHandle) 将 render buffer `gbm_bo_import()` 到 X server 进程, 并创建X 的 Pixmap (Pixmap 的Backing BO就是当初App进程创建的)后读取渲染结果进行合成。

该过程通过 X client 和 server 进程间的 buffer 共享实现了 render buffer 的零拷贝。
而同步问题也在这个过程中产生，当 render buffer 被 server 进程导入后用于合成时，渲染结果什么时候被读取完毕(render buffer IDLE 状态)，需要告知client 进程(client不能在上一帧数据未读取完毕前同时再渲染到同一个render buffer)。同样client 也须在 server 读取当前帧之前告知server 渲染是否已经完成。

这样 X client 和 server 之间的buffer 同步问题就产生了。

# 导入/导出

render buffer 的导入/导出操作是Linux 下[Buffer 共享和同步](https://www.kernel.org/doc/html/latest/driver-api/dma-buf.html)的一个标准流程，不仅仅是在 DRM 子系统使用，在Linux的其它子系统也广泛使用，如Video4Linux, Networking。这里仅仅将 mesa 中的实现与DMABUF 机制中的角色对应一下，作为一个DMABUF的应用案例分析。

- 导出(exporter·mesa)
     - [`image->queryImage(image, __DRI_IMAGE_ATTRIB_FD, &buffer_fds[i])`](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/loader/loader_dri3_helper.c#L1602)
        - [`dri2_query_image(image, attrib, *value)`](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/gallium/frontends/dri/dri2.c#L1476)
            - [`dri2_query_image_by_resource_handle(image, attrib, *value)`](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/gallium/frontends/dri/dri2.c#L1350)
                - [`pipe_screen->resource_get_handle(pscreen, NULL, image->texture, &whandle, usage)`](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/gallium/drivers/r600/r600_texture.c#L556)
                    - [`radeon_winsys_bo_get_handle(rws, buffer, *whandle)`](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/gallium/winsys/radeon/drm/radeon_drm_bo.c#L1331)
                        - drmPrimeHandleToFD()

    (以上调到pipe_screen后以AMD Radeon驱动为例)

- 传送(FD transfer·xcb)

    ```c
    static const xcb_protocol_request_t xcb_req = {
        .count = 2,
        .ext = &xcb_dri3_id,
        .opcode = XCB_DRI3_PIXMAP_FROM_BUFFERS,
        .isvoid = 1
    };
    ```

    - [`xcb_dri3_pixmap_from_buffers()`](https://gist.github.com/lucmann/2a6e24338cdae55ac359af3d25ddf2da#file-dri3-c-L676)
        - [`xcb_send_request_with_fds()`](https://gitlab.freedesktop.org/xorg/lib/libxcb/-/blob/master/src/xcb_out.c#L225)
            - [`send_fds(xcb_connection_t *, fds, num_fds)`](https://gitlab.freedesktop.org/xorg/lib/libxcb/-/blob/master/src/xcb_out.c#L190)

- 导入(importer·xserver)
    - [`proc_dri3_pixmap_from_buffers(ClientPtr client)`](https://gitlab.freedesktop.org/xorg/xserver/-/blob/master/dri3/dri3_request.c#L490)
        - [`dri3_pixmap_from_fds()`](https://gitlab.freedesktop.org/xorg/xserver/-/blob/master/dri3/dri3_screen.c#L63)
            - [`glamor_pixmap_from_fds()`](https://gitlab.freedesktop.org/xorg/xserver/-/blob/master/glamor/glamor_egl.c#L612)
                - [`gbm_bo_import(gbm_device *, GBM_BO_IMPORT_FD_MODIFIER, &import_data, 0)` (libgbm.so)](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/gbm/main/gbm.c#L583)
                    - [`image->createImageFromDmaBufs()` (libgbm.so)](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/gbm/backends/dri/gbm_dri.c#L801)

    (不难看出如果驱动支持DRI3, 则有以下依赖关系 **Xserver->Glamor->GBM**)

# 送显

如果平台的窗口系统(Winsys)是X11, 则送显主要是通过 Present 扩展完成的。这个与Xserver 的交互过程是通过 [`present_event`](https://gitlab.freedesktop.org/xorg/xserver/-/blob/master/present/present_priv.h#L226) 完成的

```c
typedef struct present_event {
    present_event_ptr next;
    ClientPtr client;
    WindowPtr window;
    XID id;
    int mask;
} present_event_rec;
```

- 注册事件
    - [`dri3_setup_present_event(struct loader_dri3_drawable *draw)`](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/loader/loader_dri3_helper.c#L1752)
        - [`xcb_present_select_input()`](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/loader/loader_dri3_helper.c#L1762): 告诉Xserver，client 正在监听这几个事件
        - [`xcb_register_for_special_xge()`](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/loader/loader_dri3_helper.c#L1779): 创建一个 XGE 事件队列，实质上是初始化了一个 pthread_cond_t

    (XGE指 [X Generic Event extension](https://www.x.org/wiki/Development/Documentation/XGE/))

- 接收事件
- 处理事件

