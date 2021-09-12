---
title: winsys in Mesa
date: 2021-09-12 11:10:57
tags: winsys
---

# winsys有什么用
winsys像一个桥梁，它要将GPU渲染的结果传输到窗口系统的framebuffer，由显示系统将其呈现在屏幕上。

{% asset_img winsys-Page-2.drawio.png "winsys" %}


# mesa中的实现

mesa中winsys有两种实现: 一种是基于dri的，另一种是不基于dri的

## 基于dri

- amdgpu_winsys
- radeon_winsys
- nouveau_winsys

## 非dri

- sw_winsys
