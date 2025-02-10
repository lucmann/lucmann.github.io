---
title: EGLImage & Direct Texture
date: 2020-11-12 15:00:56
tags: [EGL, Mesa]
categories: graphics
---

# Overview
EGLImage的存在是为了更方便地在EGL client API之间共享2D Image Data. 那什么是EGL client API呢？OpenGL, OpenGL ES, OpenVG都是EGL client API. EGLImage与下面3个EGL Extensions有关:
- EGL_KHR_image
- EGL_KHR_image_base
- EGL_KHR_image_pixmap

<!--more-->

后两个扩展其实是从第一个开离出来的。涉及EGLImage新增的Token和API都是在后两个扩展里定义的。

# EGL_KHR_image_base
它定义了一个新的Opaque类型EGLImageKHR
```c
typedef void * EGLImageKHR;
```

两者它定义了2个新API
```c
EGLImageKHR eglCreateImageKHR(
EGLDisplay dpy,
EGLContext ctx,
EGLenum target,
EGLClientBuffer buffer,
const EGLint *attrib_list
);

EGLBoolean eglDestroyImageKHR(
EGLDisplay dpy,
EGLImageKHR image
);
```

其中，EGLClientBuffer也是Opaque类型
```c
typedef void * EGLClientBuffer;
```

- `eglCreateImageKHR()` 接受的 **targets**:

| target                             | Notes                                             |
|:-----------------------------------|:--------------------------------------------------|
| EGL_GL_TEXTURE_2D                  | Used for GL 2D texture images                     |
| EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_X | Used for the +X face of GL cubemap texture images |
| EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_X | Used for the -X face of GL cubemap texture images |
| EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Y | Used for the +Y face of GL cubemap texture images |
| EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Y | Used for the -Y face of GL cubemap texture images |
| EGL_GL_TEXTURE_CUBE_MAP_POSITIVE_Z | Used for the +Z face of GL cubemap texture images |
| EGL_GL_TEXTURE_CUBE_MAP_NEGATIVE_Z | Used for the -Z face of GL cubemap texture images |
| EGL_GL_TEXTURE_3D                  | Used for GL 3D texture images                     |
| EGL_GL_RENDERBUFFER                | Used for GL renderbuffer images                   |
| EGL_NATIVE_PIXMAP_KHR              | Used for X11 Pixmap (added EGL_KHR_image_pixmap)  |

- `eglCreateImageKHR()` 接受的 **attributes**:

| attribute                          | Notes                                             |
|:-----------------------------------|:--------------------------------------------------|
| EGL_WIDTH                          |                                                   |
| EGL_HEIGHT                         |                                                   |
| EGL_LINUX_DRM_FOURCC_EXT           |                                                   |
| EGL_DMA_BUF_PLANE0_FD_EXT          |                 0x3272                            |
| EGL_DMA_BUF_PLANE0_OFFSET_EXT      |                 0x3273                            |
| EGL_DMA_BUF_PLANE0_PITCH_EXT       |                 0x3274                            |
| EGL_DMA_BUF_PLANE1_FD_EXT          |                 0x3275                            |
| EGL_DMA_BUF_PLANE1_OFFSET_EXT      |                 0x3276                            |
| EGL_DMA_BUF_PLANE1_PITCH_EXT       |                 0x3277                            |
| EGL_DMA_BUF_PLANE2_FD_EXT          |                 0x3278                            |
| EGL_DMA_BUF_PLANE2_OFFSET_EXT      |                 0x3279                            |
| EGL_DMA_BUF_PLANE2_PITCH_EXT       |                 0x327A                            |

- `eglDestroyImageKHR()`



## EGLImageKHR in [kylin-wlcom](https://gitee.com/openkylin/kylin-wayland-compositor)

```mermaid
sequenceDiagram
    autonumber

    participant app
    participant compositor
    participant libwlroots
    participant mesa
    participant kernel

    app ->> compositor : REQUEST:create_prime_buffer <- fd
    compositor -->> compositor : drm_handle_create_prime_buffer(..., fd, ...)
    compositor ->> libwlroots  : wlr_buffer_init(..., &buffer_impl, ...)
    compositor -->> compositor : API:buffer_get_dmabuf(struct wlr_buffer *, struct wlr_dmabuf_attributes *)
    compositor ->> libwlroots  : wlr_buffer_get_dmabuf(struct wlr_buffer *, struct wlr_dmabuf_attributes *)
    compositor -->> compositor : gl_texture_from_dmabuf_buffer()
    compositor -->> compositor : gl_texture_from_dmabuf()
    compositor -->> compositor : ky_egl_create_image_from_dmabuf() -> EGLImageKHR
    rect rgb(191, 223, 255)
        compositor ->> mesa        : eglCreateImageKHR() -> EGLImageKHR
        mesa -->> mesa : dri2_create_image()
        mesa -->> mesa : dri2_create_image_dma_buf()
        mesa -->> mesa : xxx_resource_from_handle()
        mesa -->> mesa : xxx_bo_import(..., fd)
        mesa ->> kernel: drmPrimeFDToHandle(..., fd) -> gem_handle
    end
```

# EGL_KHR_image_pixmap
