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

### What is the row stride?
row stride 通常指纹理图片中相邻两行之间的字节数。对于 Linear 布局的纹理, block size 是 1x1, 它的 row stride 即为

$$ RowStride = BytesPerPixel * Width $$

这也是所谓的逻辑 row stride, 而对于 U-interleaved 布局的纹理，由于它在内存中是按块存储的（块与块之间是线性的），所以 U-interleaved 布局的纹理的 row stride 已经不是通常意义的“行”了，而是由**块**组成的**行**。所以 U-interleaved 布局的纹理的 row stride 为

$$ RowStride = BytesPerBlock * nBlocksX $$

#### 非压缩格式的 U-interleaved 布局的 row stride

![U-interleaved row stride in regular format](u-interleaved-row-stride-non-compress.drawio.svg)

#### 压缩格式的 U-interleaved 布局的 row stride
以 [BC1 压缩格式](https://sv-journal.org/2014-1/06/en/index.php?lang=en#5)为例, BC1 是 S3TC 家族的一员(所有的 S3TC 家族都使用 4x4 的块大小)，而 DRM_FORMAT_MOD_ARM_16X16_BLOCK_U_INTERLEAVED 是 16x16 大小的，所以一个 U-interleaved 块中包含 4x4 个 BC1 压缩块。

![U-interleaved memory layout](u-interleaved-row-stride-compress.drawio.svg)

