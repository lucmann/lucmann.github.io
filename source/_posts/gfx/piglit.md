---
title: Piglit
date: 2025-02-07 14:44:56
tags: [OpenGL]
categories: graphics
---


```mermaid
flowchart TB
  subgraph p [Main Thread]
    subgraph p0 ["piglit_drm_create_dma_buf()"]
      p.a["drv->create(w, h, fourcc, src_data, drm_buf)"]
      p.b["drv->export(drm_buf)"]
      subgraph p1 ["create_and_destroy_texture()"]
        p.c["egl_image_for_dma_buf_fd<br>(buf, fd, fourcc, img)"]
        p.d["texture_for_egl_image(img, texture)"]
        p.e["eglDestroyImageKHR(display, img)"]
        p.f["glDeleteTextures(1, &texture)"]
        p.g["glFinish()"]
      end
    end
  end
  subgraph t1 [Thread 1]
    subgraph lo11 [Loop 1]
      t1.a["eglCreateContext()"]
      t1.b["eglMakeCurrent()"]
      subgraph lo111 ["create_and_destroy_texture()"]
        t1.c["egl_image_for_dma_buf_fd<br>(buf, fd, fourcc, img)"]
        t1.d["texture_for_egl_image(img, texture)"]
        t1.e["eglDestroyImageKHR(display, img)"]
        t1.f["glDeleteTextures(1, &texture)"]
        t1.g["glFinish()"]
      end
    end
  end

  subgraph t2 [Thread 2]
    subgraph lo21 [Loop 1]
      t2.a["eglCreateContext()"]
      t2.b["eglMakeCurrent()"]
      subgraph lo211 ["create_and_destroy_texture()"]
        t2.c["egl_image_for_dma_buf_fd<br>(buf, fd, fourcc, img)"]
        t2.d["texture_for_egl_image(img, texture)"]
        t2.e["eglDestroyImageKHR(display, img)"]
        t2.f["glDeleteTextures(1, &texture)"]
        t2.g["glFinish()"]
      end
    end
  end

  p.a --> p.b --> p.c --> p.d --> p.e --> p.f --> p.g

  t1.a --> t1.b --> t1.c --> t1.d --> t1.e --> t1.f --> t1.g
  t2.a --> t2.b --> t2.c --> t2.d --> t2.e --> t2.f --> t2.g

  p --> t1
  p --> t2

  lo11 -- 100000 times --> lo11
  lo21 -- 100000 times --> lo21

  click p.a https://gitlab.freedesktop.org/mesa/piglit/-/blob/main/tests/util/piglit-framework-gl/piglit_drm_dma_buf.c#L519"
  click p.b https://gitlab.freedesktop.org/mesa/piglit/-/blob/main/tests/util/piglit-framework-gl/piglit_drm_dma_buf.c#L519"
```

<!--more-->

[Piglit](https://gitlab.freedesktop.org/mesa/piglit) 是为 OpenGL, Vulkan, OpenCL 实现设计的自动化测试套件。Piglit 的目标是提高 OpenGL, Vulkan 和 OpenCL 开源驱动的质量，并为开发者提供一个简单的回归测试途径。

这里我们通过 Piglit 里的几个有意思的测试用例，反过来了解 Mesa 实现的一些底层机制，比如 DMABUF, EGLImageKHR 等等。

# [ext_image_dma_buf_import/refcount-multithread](https://gitlab.freedesktop.org/mesa/piglit/-/blob/main/tests/spec/ext_image_dma_buf_import/refcount-multithread.c)

测试多线程下，驱动是否正确处理 dma-buf 的导入/导出，

- export `drmPrimeHandleToFD()`
- import `drmPrimeFDToHandle()`

FD 是文件描述符，而 bo handles 在内核态和用户态各自维护有一份，分别是 **GEM bo handles** 和 **Userspace bo handles**, 在 mesa 的实现里，这两个集合是 1:1 映射关系, 当一个 gem bo handle 被导入, 如果对应的 userspace bo handle(实际上就是 drmPrimeFDToHandle() 返回的整数)已经存在，mesa 驱动仅仅是将这个 userspace bo handle 的引用计数加 1. 这就有可能导致 bo_destroy 函数和 bo_import() 函数发生竞争 (race), 从而导致 UAF, 这个 Piglit 用例就是专门测试这种场景的。

## FAQ

### Q: 两个线程先后对同一个 FD 调用 `drmPrimeFDToHandle()`，返回给各自线程的 handle ，是同一个值吗？

A: 因为是同一个 FD, 所以它底层的或者说内核的 drm_gem_object 也是同一个，返回的 handle 是同一个值，这也正是 ext_image_dma_buf_import/refcount-multithread 这个用例要构建的场景。

### Q: `drmPrimeFDToHandle()`/`drmPrimeHandleToFD()` 会影响 drm_gem_object 的引用计数吗？

A: 不会

# [Waffle](https://gitlab.freedesktop.org/mesa/waffle)

[Waffle /wa:fl/ 动听而无意义的话](https://gitlab.freedesktop.org/mesa/waffle) 是一个跨平台的在运行时动态地选择一个 OpenGL API.piglit 用它来创建 EGLContext, 最终会创建 Galliumm pipe_context. 此外 apitrace, Dante (open source DOOM 3) 也使用 waffle
