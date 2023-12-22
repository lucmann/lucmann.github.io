---
title: DRM format modifier
date: 2020-11-11 15:35:43
tags: [DRM]
categories: graphics
---

# FourCC
FourCC format code是Linux内核DRM子系统用来描述framebuffer像素格式。它们被定义在内核源码[include/uapi/drm/drm_fourcc.h](https://github.com/torvalds/linux/blob/master/include/uapi/drm/drm_fourcc.h#L156). 基本上FourCC format code足以描述framebuffer中像素的内容了，但是随着GPU的更新，原来线性排列的buffers，为了获取更好的性能，可能会采取vendor specific的布局方式。另外，这些buffer也可能保存的是各种不同的压缩数据，这样一来，原来的FourCC format code就无法携带新的buffer
layout或compression format的信息，DRM format modifier就是用来解决这些问题的。

<!--more-->

# Modifier

# Examples

## [DRM_FORMAT_MOD_ARM_16X16_BLOCK_U_INTERLEAVED](https://elixir.bootlin.com/mesa/latest/source/include/drm-uapi/drm_fourcc.h#L1332)

### How U-interleaved improves performance?

![U-interleaved memory layout](u-interleaved.drawio.svg)

### What is the block size?

对于 U-interleaved 布局的纹理, 有两种可能的 block size:

- 4x4 (压缩格式)
- 16x16 (非压缩格式)

block size 被用来计算数据的 row stride(指纹理图片中相邻两行之间的字节数)。对于 Linear 布局的纹理, block size 是 1x1, 它的

$$ \[ row\_stride = bytes\_per\_block * effective\_width * block\_size.height \] $$

注意这里的 block 指的是 pixel format 的 block, 而非 layout 的 block. 对于非压缩格式，一个 format block 就是一个像素, 所以对于 Linear 布局的非压缩格式纹理 $bytes\_per\_block$ 即 $bytes\_per\_pixel$, $effective\_width$ 即它本身的 $width$, $block\_size.height$ 即等于 1.

