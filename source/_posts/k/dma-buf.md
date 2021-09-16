---
title: Buffer Sharing and Synchronization
date: 2021-09-15 18:30:15
tags: [drm, dma-buf]
categories: kernel
---

本文的标题来自[Linux Kernel 5.6.0-rc4文档](https://01.org/linuxgraphics/gfx-docs/drm/driver-api/dma-buf.html), dma-buf作为一个内核子系统，它的使用场景不局限于drm "[prime]()" multi-GPU支持，它主要由3个组件支撑:

<!--more-->

- `dma_buf` 代表sg_table, 暴露给用户FD
- `dma_fence` 通知机制
- `dma_resv` 管理共享的或专有的fences

## dma-buf

dma-buf是Linux kernel中实现内存共享的一种机制，在Linux中"Everything is file"的思想的指导下，内存（大多时候是显存）被封装成`file`, 它的**file descriptor(FD)**被用来在进程之间传递(共享)，传出者叫**exporter**, 接收者叫**importer**. dma-buf可以说是披着`file`皮的`drm_gem_object`, 它所封装的内容还是DRM的内存管理对象。在kernel中，`drm_gem_object`的ID叫handle. 所以kernel中便有了以下两个函数:


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


## dma_resv

# dma_buf, dma_fence, dma_resv, 还有drm_gem_object, 这4者有什么关系？

{% asset_img dma-buf.drawio.png %}

在这里面dma_fence是用来实现drm_gem_object间的共享和同步的，而dma_resv就是所谓的“胶水”，将这两者联系在一起。
