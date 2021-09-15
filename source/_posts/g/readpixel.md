---
title: ReadPixels in Mesa
date: 2021-09-15 16:57:44
tags: OpenGL
---

# ReadPixels有几种实现方法

- blit-based ReadPixels packing

    它取决于pipe是否支持`PIPE_CAP_PREFER_BLIT_BASED_TEXTURE_TRANSFER`的能力，如果支持，则由pipe完成一次GPU --> CPU的拷贝

- CPU-based ReadPixels packing
 
    也即fallback to`_mesa_readpixels`, 由mesa完成一次CPU --> CPU的拷贝
