---
title: Buffer Sharing and Synchronization
date: 2021-09-15 18:30:15
tags: [DRM]
categories: linux
---

本文的标题来自[Linux Kernel 5.6.0-rc4文档](https://01.org/linuxgraphics/gfx-docs/drm/driver-api/dma-buf.html), dma-buf作为一个内核子系统，它的使用场景不局限于drm "[PRIME](https://blog.csdn.net/hexiaolong2009/article/details/105961192)" multi-GPU支持，它主要由3个组件支撑:

<!--more-->

- `dma_buf` 代表sg_table, 暴露给用户FD
- `dma_fence` 通知机制
- `dma_resv` 管理共享的或专有的fences

# DMA-BUF

dma-buf是Linux内核中在上下文间，进程间，设备间，子系统间进行 buffer 共享的一种实现。 它十几年前就已经合入内核了。

```
commit 3248877ea1796915419fba7c89315fdbf00cb56a
Author: Dave Airlie <airlied@redhat.com>
Date:   Fri Nov 25 15:21:02 2011 +0000

    drm: base prime/dma-buf support (v5)

    This adds the basic drm dma-buf interface layer, called PRIME. This
    commit doesn't add any driver support, it is simply and agreed upon starting
    point so we can work towards merging driver support for the next merge window.

    Current drivers with work done are nouveau, i915, udl, exynos and omap.

    The main APIs exposed to userspace allow translating a 32-bit object handle
    to a file descriptor, and a file descriptor to a 32-bit object handle.

    The flags value is currently limited to O_CLOEXEC.

    Acknowledgements:
    Daniel Vetter: lots of review
    Rob Clark: cleaned up lots of the internals and did lifetime review.

    v2: rename some functions after Chris preferred a green shed
    fix IS_ERR_OR_NULL -> IS_ERR
    v3: Fix Ville pointed out using buffer + kmalloc
    v4: add locking as per ickle review
    v5: allow re-exporting the original dma-buf (Daniel)

    Reviewed-by: Daniel Vetter <daniel.vetter@ffwll.ch>
    Reviewed-by: Rob Clark <rob.clark@linaro.org>
    Reviewed-by: Sumit Semwal <sumit.semwal@linaro.org>
    Reviewed-by: Inki Dae <inki.dae@samsung.com>
    Acked-by: Ben Widawsky <benjamin.widawsky@intel.com>
    Signed-off-by: Dave Airlie <airlied@redhat.com>
```

- exporter调用

```c
/**
 * drm_gem_prime_handle_to_fd - PRIME export function for GEM drivers
 * @dev: dev to export the buffer from
 * @file_priv: drm file-private structure
 * @handle: buffer handle to export
 * @flags: flags like DRM_CLOEXEC
 * @prime_fd: pointer to storage for the fd id of the create dma-buf
 *
 * This is the PRIME export function which must be used mandatorily by GEM
 * drivers to ensure correct lifetime management of the underlying GEM object.
 * The actual exporting from GEM object to a dma-buf is done through the
 * &drm_gem_object_funcs.export callback.
 */
int drm_gem_prime_handle_to_fd(struct drm_device *dev,
			       struct drm_file *file_priv, uint32_t handle,
			       uint32_t flags,
			       int *prime_fd)

```

- importer调用

```c
/**
 * drm_gem_prime_fd_to_handle - PRIME import function for GEM drivers
 * @dev: dev to export the buffer from
 * @file_priv: drm file-private structure
 * @prime_fd: fd id of the dma-buf which should be imported
 * @handle: pointer to storage for the handle of the imported buffer object
 *
 * This is the PRIME import function which must be used mandatorily by GEM
 * drivers to ensure correct lifetime management of the underlying GEM object.
 * The actual importing of GEM object from the dma-buf is done through the
 * &drm_driver.gem_prime_import driver callback.
 *
 * Returns 0 on success or a negative error code on failure.
 */
int drm_gem_prime_fd_to_handle(struct drm_device *dev,
			       struct drm_file *file_priv, int prime_fd,
			       uint32_t *handle)

```

从上面两个函数的接口看，它们共同涉及3个数据对象：

- drm_device
- dma_buf fd
- drm_gem_object handle

显然两个函数里的`dev`一定是**不同**的`drm_device`, `dma_buf` fd一定是同一个FD（也即同一个dma-buf, 要不然也不叫共享了），那么`handle`呢？肯定也是**不同**的`handle`, 因为`handle`其实是对`device`而言的，它是一个设备持有的`drm_gem_object`的ID. 但这个ID背后的东西(backing storage)可能是**同一个**东西。

可以打个比方，你去银行要办两种业务，两种业务分别排号，假如你要办的A业务排到7号，B业务也刚好排到7号（注意:号码相同，但是两个号），但是很有可能是同一个业务员为你办理这两种业务，这里的业务就是设备驱动，而那个业务员就是dma-buf(或者它封装的那块显存)。

## dma_fence


`dma_fence_default_wait` 是 dma-fence 默认的 wait 操作。该函数会让当前进程(task) 进入睡眠状态 (可中断睡眠或不可中断睡眠，取决于调用者传入的参数 `intr`）, 直到 dma-fence 被 signaled 或者设置的超时时间到。

```c
	cb.base.func = dma_fence_default_wait_cb;
	cb.task = current;
	list_add(&cb.base.node, &fence->cb_list);
```

## dma_resv

```mermaid
classDiagram
	dma_resv  -- dma_resv_list : *fences
    dma_fence -- dma_fence_ops : *ops
    dma_resv_list o-- dma_fence: max_fences
	class dma_resv{
		+ww_mutex lock
		+dma_resv_list *fences
	}
    class dma_resv_list{
        -rcu_head rcu
        -u32 num_fences
        -u32 max_fences
        -dma_fence *table[]
    }
	class dma_fence{
		+spinlock_t *lock
		+dma_fence_ops *ops
        +list_head cb_list
        +ktime_t timestamp
        +rcu_head rcu
        +u64 context
        +u64 seqno
        +unsigned long flags
        +kref refcount
        +int error
	}
    class dma_fence_ops{
        +use_64bit_seqno
        +get_driver_name(fence) char *
        +get_timeline_name(fence) char *
        +enable_signaling(fence) bool
        +signaled(fence) bool
        +wait(fence, intr, timeout) signed long
        +release(fence) void
        +fence_value_str(fence, char *, size) void
        +timeline_value_str(fence, char *, size) void
        +set_deadline(fence, deadline) void
    }
```

# Synchronization

## Implicit Synchronization

## Explicit Synchronization
