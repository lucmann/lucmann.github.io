---
title: X C Bindings 
date: 2024-09-21 15:35:43
tags: [X11]
categories: graphics
---

XCB 是应用与 X 服务器交互使用的 C 绑定函数集，它里面的一些 C 函数是通过 python3-xcbgen 工具生成的，不经过编译在它的[源码库](https://gitlab.freedesktop.org/xorg/lib/libxcb)是找不到的。而且 XCB 古老到仍然使用 autotools 那套构建系统，想要看到某些函数的“真容”，你还得“千呼万唤”。

<!--more-->

# Build
- 安装工具
    - `sudo apt install python3-xcbgen autoconf automake libtool xutils-dev xcb-proto`
    - **xutils-dev** 里包含 xorg-macros
    - **libxcb** 的版本与 ***xcb-proto** 软件包的版本有依赖关系，如果你安装的 xcb-proto 版本是 1.14-2, 那么最好将 libxcb 的 tag 也检出到 libxcb-1.14

- 构建
    - `./autogen.sh`
    - `make`
- 千呼万唤始出来
    - 在构建完之后，为了保存“成果”，需要使用一点 git 小技巧, 在 `src/.gitignore` 最后加上
        - `!*.c`
    - 现在可以好好看看 `xcb_dri3_pixmap_from_buffers()` 的实现了
        - [https://gitlab.freedesktop.org/lucmann/libxcb/-/tree/v1.14-where-you-will-find-xcb_dri3_pixmap_from_buffers](https://gitlab.freedesktop.org/lucmann/libxcb/-/tree/v1.14-where-you-will-find-xcb_dri3_pixmap_from_buffers)

# Bindings
## xcb_create_pixmap

```mermaid
sequenceDiagram
    autonumber
    participant Mesa
    participant X11

    Mesa-->>X11: xcb_generate_id()
    X11-->>Mesa: uint32_t pid (pixmap id)
    Mesa-->>X11: xcb_create_pixmap()
    X11->>X11: ProcCreatePixmap()
    note left of X11: CreatePixmapProcPtr 接口，有各种实现<br/>glamor, dri2, xwayland, xnest<br/>当初的 pixmap id 会<br/>记录在创建好的 Pixmap 的 drawable->id
    X11->>X11: glamor_create_pixmap(usage=0)
    X11->>X11: glamor_create_fbo()
    rect rgb(191, 223, 255)
    X11->>X11: _glamor_create_tex()
    X11-->>Mesa: glGenTextures()
    end
    Mesa-->>X11: xcb_dri3_buffers_from_pixmap(pid)
```

## xcb_dri3_buffers_from_pixmap

```mermaid
sequenceDiagram
    autonumber
    participant Mesa
    participant X11

    Mesa-->>X11: xcb_dri3_buffers_from_pixmap(pid)
    rect rgb(191, 223, 255)
    X11->>X11: proc_dri3_buffers_from_pixmap()
    X11->>X11: dixLookupResourceByType(pid)
    note left of X11: 通过 pid 找到当初的 PixmapPtr
    X11->>X11: dri3_fds_from_pixmap(PixmapPtr)
    rect rgb(200, 150, 255)
    note left of X11: X11 导出 FD
    X11->>X11: glamor_egl_fds_from_pixmap()
    X11->>X11: glamor_make_pixmap_exportable()
    X11->>X11: glamor_gbm_bo_from_pixmap_internal()
    note left of X11: 通过 PixmapPtr 获取 gbm_bo
    X11-->>Mesa: gbm_bo_get_fd()
    Mesa-->>X11: xcb_dri3_buffers_from_pixmap_reply()
    Mesa-->>X11: xcb_dri3_buffers_from_pixmap_reply_fds()
    note right of Mesa: 拿到导出的 FD 需要两步<br/> reply_fds() 才是真正拿到 FD
    end
    rect rgb(200, 150, 255)
    note right of Mesa: Mesa 导入 FD
    Mesa->>Mesa: dri2_from_dma_bufs2()
    Mesa->>Mesa: dri2_create_image_from_fd()
    Mesa->>Mesa: dri2_create_image_from_winsys(winsys_handle)
    loop Every FD
        Mesa->>Mesa: xxx_resource_from_handle()
        Mesa->>Mesa: xxx_bo_import()
        note right of Mesa: resource_from_handle() 返回的<br/>pipe_resource 给 __DRIimage.texture
    end
    end
    end
```

## glamor_make_pixmap_exportable

函数功能:

- 100 创建一个新的 Pixmap `exported` 用来 export
- 200 同时创建一个 gbm_bo, 将 `exported` 的 header 设置成刚创建的 gbm_bo
- 300 将原来的 Pixmap 的内容 `CopyArea()` 到 `exported`
- 400 `glamor_egl_exchange_buffers(pixmap, exported)`, 将 `exported` 里的 tex/gbm_bo/EGLImage 换到原来的 Pixmap 结构体中
- 500 销毁 `exported` Pixmap

步骤 300 暗藏 bug, 因为 `CopyArea()` 会调用 `glamor_copy_fbo_fbo_draw()` 将原 Pixmap 作为纹理采样到新的 Pixmap `exported`, 也就是会调入 `_mesa_DrawArrays()`, 进而导致这个即将被导出的 BO, 被重新设置成 drawcall 的 render target buffer。 但是 `glamor_copy_fbo_fbo_draw()` 在调用 `_mesa_DrawArrays()` 进行"拷贝"后，并**不会 `glFlush()`**, 只能等到 X11 定时触发 `Screen->BlockHandler()` 时 `glFlush()`。

另外一个进程导入 BO 后，同样会设置它为 render target buffer, 这就造成 X11 进程和 App 进程同时往同一个 render target buffer 里提交渲染命令，导致结果变得不确定。
