---
title: DRM format modifier
date: 2020-11-11 15:35:43
tags: [DRM]
---

# FourCC
FourCC format code是Linux内核DRM子系统用来描述framebuffer像素格式。它们被定义在内核源码[include/uapi/drm/drm_fourcc.h](https://github.com/torvalds/linux/blob/master/include/uapi/drm/drm_fourcc.h#L156). 基本上FourCC format code足以描述framebuffer中像素的内容了，但是随着GPU的更新，原来线性排列的buffers，为了获取更好的性能，可能会采取vendor specific的布局方式。另外，这些buffer也可能保存的是各种不同的压缩数据，这样一来，原来的FourCC format code就无法携带新的buffer
layout或compression format的信息，DRM format modifier就是用来解决这些问题的。

<!--more-->

# Modifier
