---
title: ReadPixels in Mesa
date: 2021-09-15 16:57:44
tags: [OpenGL, Mesa]
categories: graphics
---

# ReadPixels有几种实现方法

- blit-based ReadPixels packing

<!--more-->

它取决于pipe是否支持`PIPE_CAP_PREFER_BLIT_BASED_TEXTURE_TRANSFER`的能力，如果支持，则由pipe完成一次GPU --> CPU的拷贝

- CPU-based ReadPixels packing
 
否则fallback到`_mesa_readpixels`, 由mesa完成一次CPU --> CPU的拷贝

