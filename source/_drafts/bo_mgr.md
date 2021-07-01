---
title: panfrost的BO管理
tags: [Mesa]
categories: lib
author: luc@sietium.com
date: 2021-06-24
---

BO管理是GPU用户态驱动里最重要的话题之一。它强依赖KMD(Kernel Mode Driver)的内存管理框架，当前Linux内核DRM子系统主流的内存管理框架是[GEM](https://www.kernel.org/doc/html/latest/gpu/drm-mm.html?highlight=drm#the-graphics-execution-manager-gem)。用户态的BO管理一定会涉及到GEM，但这里GEM不是我们讨论的主要内容。

这里我们以Mesa里的[panfrost](https://docs.mesa3d.org/drivers/panfrost.html)驱动为例，简单记录一下它的用户态BO的是如何管理的。

# panfrost_bo

```

struct panfrost_bo {
        /* Must be first for casting */
        struct list_head bucket_link;

        /* Used to link the BO to the BO cache LRU list. */
        struct list_head lru_link;

        /* Store the time this BO was use last, so the BO cache logic can evict
         * stale BOs.
         */
        time_t last_used;

        /* Atomic reference count */
        int32_t refcnt;

        struct panfrost_device *dev;

        /* Mapping for the entire object (all levels) */
        struct panfrost_ptr ptr;

        /* Size of all entire trees */
        size_t size;

        int gem_handle;

        uint32_t flags;

        /* Combination of PAN_BO_ACCESS_{READ,WRITE} flags encoding pending
         * GPU accesses to this BO. Useful to avoid calling the WAIT_BO ioctl
         * when the BO is idle.
         */
        uint32_t gpu_access;

        /* Human readable description of the BO for debugging. */
        const char *label;
};

```


# bo_map

`bo_map`是`panfrost_device`结构体里的一个成员，它是一个`util_sparse_array`, 它保存的是BO的`gem_handle`。

```
struct util_sparse_array {
    size_t elem_size;
    unsigned node_size_log2;

    uintptr_t root;
}
```

# bo_cache

`bo_cache`是`panfrost_device`结构体里的一个成员，它是由`NR_BO_CACHE_BUCKETS + 1`个双向链表和一个互斥锁组成，用来缓存[BO](#panfrost_bo)。

```
struct {
        pthread_mutex_t lock;

        /* List containing all cached BOs sorted in LRU (Least
         * Recently Used) order. This allows us to quickly evict BOs
         * that are more than 1 second old.
         */
        struct list_head lru;

        /* The BO cache is a set of buckets with power-of-two sizes
         * ranging from 2^12 (4096, the page size) to
         * 2^(12 + MAX_BO_CACHE_BUCKETS).
         * Each bucket is a linked list of free panfrost_bo objects. */

        struct list_head buckets[NR_BO_CACHE_BUCKETS];
} bo_cache;
```

- `lru`是按最近最少使用排序的BO链表。

- `buckets`是按BO大小划分的链表数组，目前共有11个，即4KB, 8KB, 16KB, 32KB, 64KB, 128KB, 256KB, 512KB, 1MB, 2MB, 4MB。

NOTE: BO并不是只有这11种大小，例如4KB链表中的BO大小都小于8KB, 所有大小超过4MB(包括4MB)的BO都在4MB链表里。特别地，panfrost不会申请小于4KB的BO。这样这11个链表就囊括了所有大小的BO。


# 基于`panfrost_bo`的操作(Method)有哪些呢？

+-------+-----------------------------------------+----------------------------------------------------+
| Op    |  DRM ioctl                              |  Return Value                                      |
+:======+:========================================+:===================================================+
|Alloc  | - `DRM_IOCTL_PANFROST_CREATE_BO`        | - 返回一个`panfrost_bo`或者`null`                  |
|       |                                         | - **gpu** va                                       |
+-------+-----------------------------------------+----------------------------------------------------+
|Free   | - `DRM_IOCTL_GEM_CLOSE`                 | - void                                             |
|       |                                         |                                                    |
+-------+-----------------------------------------+----------------------------------------------------+
|Fetch  | - `DRM_IOCTL_PANFROST_WAIT_BO`          | - 如果BO被占用返回`false`, 否则`true`              |
|       | - `DRM_IOCTL_PANFROST_MADVISE`          | - 如果BO没有被保留(retained)，则释放它，否则返回它 | 
+-------+-----------------------------------------+----------------------------------------------------+
|Map    | - `DRM_IOCTL_PANFROST_MMAP_BO`          | - `offset`(`mmap`的最后一个参数)                   |
|       | - `mmap`                                | - **cpu** va                                       |
+-------+-----------------------------------------+----------------------------------------------------+
|Unmap  | - NO DRM ioctl, 只须系统调用`munmap`    | - void                                             |
|       |                                         |                                                    |
+-------+-----------------------------------------+----------------------------------------------------+
|Submit | - `DRM_IOCTL_PANFROST_SUBMIT`           | - 成功返回0, 否则返回errno                         |
|       |                                         |                                                    |
+-------+-----------------------------------------+----------------------------------------------------+

下面逐一说明(主要说明用户态接口，不过多涉及`drm`接口):

## Alloc

```
struct panfrost_bo *
panfrost_bo_alloc(struct panfrost_device *dev,
                  size_t size,
                  uint32_t flags,
                  const char *label);
```

- `size` 字节数，不是页对齐的
- `flags` 主要决定BO映射不映射，什么时候映射
    * `PAN_BO_EXECUTE`
    * `PAN_BO_GROWABLE`
    * `PAN_BO_INVISIBLE`
    * `PAN_BO_DELAY_MMAP`
    * `PAN_BO_SHARED`
- `label` 一个表明BO用途的字串
    * `Indirect dispatch shader`
    * `Indirect dispatch descriptors` (64B + 32B)
    * `Indirect draw states`
    * `Indirect draw varying heap` (512MB)
    * `Tile Heap` (64MB)
    * `Sample positions` (1KB)
    * `Occlusion query result`
    * `CRC data`
    * `Unused (BO cache)`

总的来说，Alloc返回两个关键信息:

-  `gem_handle` BO在内核态与用户态之间的纽带，它本质上是BO所封装的这块内存对象的[id](https://www.kernel.org/doc/html/latest/core-api/idr.html), panfrost将它保存在[util_sparse_array](#bo_map)的数据结构中。当Submit BO时，这些`gem_handle`又会被传回内核，这样GPU便知道Job Descriptor存放在哪里。
-  `gpu va` BO所封装的这块内存的GPU起始虚拟地址。

## Free

```
void
panfrost_bo_free(struct panfrost_bo *bo);
```

- `bo` 要释放的BO

Free主要做两件事:

- 内核态: 告诉内核将这个BO的`gem_handle`释放
- 用户态: 将这个BO从`util_sparse_array`中释放

## Fetch

```
struct panfrost_bo *
panfrost_bo_cache_fetch(struct panfrost_device *dev,
                        size_t size,
                        uint32_t flags,
                        const char *label,
                        bool dontwait);
```

相比Alloc/Free, Fetch稍微复杂一些，首先我们要问两个关于Fetch的问题:

- 从哪Fetch?
- 怎样Fetch?

### 从哪Fetch

为了减小申请BO的开销，panfrost为BO建立了[cache](#bo_cache), 当需要申请BO时，不会直接向内核申请，而是先从`bo_cache`中搜索是否有足够大小且空闲的BO，如果有，就不必费事向内核申请。如果BO使用完毕，也不会立即让内核释放，而是先放回`bo_cache`。

### 怎样Fetch

1. 根据`size`选中合适的`bucket`, 例如申请64MB的Tiler Heap, 则应该先到`dev->bo_cache.buckets[10]`，也就是最后一个链表中取，如果有，则成功，如果没有，则向内核申请，申请成功后缓存到这个链表中。

2. 遍历步骤1中那个`bucket`, 对其中每个BO，向内核确认两件事:

- BO准备好了吗？
- BO保留了吗？


+---------------------------------------+------------------------------------------------+--------------------------------+
|IOCTL_PANFROST_WAIT_BO                 |```                                             | - 如果BO准备好了，返回true,    |
|                                       |struct drm_panfrost_wait_bo req = {             |   否则，返回false              |
|                                       |    .handle = bo->gem_handle,                   | - 如果返回true, 则将这个BO从   |
|                                       |    .timeout_ns = timeout_ns,                   |   这个`bucket`取出来`list_del`,|
|                                       |};                                              |   准备向内核确认第2件事        |
|                                       |```                                             |                                |
+---------------------------------------+------------------------------------------------+--------------------------------+
|IOCTL_PANFROST_MADVISE                 |```                                             | - 向内核确认"我可以用这个BO吗",|
|                                       |struct drm_panfrost_madvise madv = {            |   "这个BO给我保留了吗?", 如果请|
|                                       |    .handle = bo->gem_handle,                   |   求成功且内核保留了这个BO, 则 |
|                                       |    .madv = PANFROST_MADV_WILLNEED,             |   返回BO, 否则释放这个BO, 继续 |
|                                       |};                                              |   确认下一个                   |
|                                       |```                                             |                                |
+---------------------------------------+------------------------------------------------+--------------------------------+


## Map

```
void
panfrost_bo_mmap(struct panfrost_bo *bo);
```

BO申请之后，因为它是显存，所以CPU要能读写它前必须映射到CPU的虚拟地址空间。Map的过程分为两步:


+--------------------------------------+----------------------------------------------+-----------------------------------+
|IOCTL_PANFROST_MMAP_BO                |```                                           | - 如果失败，则程序退出；如果成    |
|                                      |struct drm_panfrost_mmap_bo req = {           |   功，返回`offset`, 这个`offset`  |
|                                      |    .handle = bo->gem_handle,                 |   会作为第2步`mmap`的最后一个参数 |
|                                      |};                                            |                                   |
|                                      |```                                           |                                   |
+--------------------------------------+----------------------------------------------+-----------------------------------+
|mmap                                  |```                                           | - 如果失败，返回`MAP_FAILED`, 但程|
|                                      |bo->ptr.cpu = mmap(NULL, bo->size,            |   序不会退出；否则，返回有效的cpu |
|                                      |    PROT_READ | PROT_WRITE, MAP_SHARED,       |   va                              |
|                                      |    bo->dev->fd, req.offset);                 |                                   |
|                                      |                                              |                                   |
|                                      |```                                           |                                   |
+--------------------------------------+----------------------------------------------+-----------------------------------+

Map的复杂之处在于**要不要Map**, **什么时候Map**。如果BO不需要CPU读写，则不需要Map(Map的开销也很大)。而这个信息是在不同的场景下由`flags`这个参数传达的。

+--------------------------------------+-----------------------+
| BO申请后立即Map                      | - `PAN_BO_EXECUTE`    |
|                                      | - `PAN_BO_SHARED`     |
+--------------------------------------+-----------------------+
| BO申请后不立即Map，需要时再Map       | - `PAN_BO_INVISIBLE`  |
|                                      | - `PAN_BO_DELAY_MMAP` |
+--------------------------------------+-----------------------+
| BO申请后不Map                        | - `PAN_BO_GROWABLE`   |
|                                      |                       |
+--------------------------------------+-----------------------+

## Unmap

略

## Submit

```
int
panfrost_batch_submit_ioctl(struct panfrost_batch *batch,
                            mali_ptr first_job_desc,
                            uint32_t reqs,
                            uint32_t in_sync,
                            uint32_t out_sync);
```

BO Submit的主要问题在于BO同步(或者说Job Descriptor同步，因为Job Descriptors都是放在BO里的)，panfrost采用DRM子系统的`syncobj`实现，所以同步问题主要是由内核处理的。BO同步是另外一个重要话题，我们会记录在另一篇文档中，这里不再讨论。

