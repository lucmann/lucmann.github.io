---
title: Render To Texture on TBR GPU
date: 2025-01-11 11:21:27
tags: OpenGL
categories: graphics
---

Render-To-Texture 是一种十分常见和简单的渲染技术，它将纹理对象和 FBO 绑定，把场景渲染到纹理中，以便之后可以反复使用。RTT 被广泛应用在 in-game cameras(virtual camera systems), post-processing 和各种特效中。本文主要比较 RTT 技术在两种不同的渲染架构下的不同和一些思考。

![rtt-on-tbr](/images/rtt/rtt-on-tbr.png)

<!--more-->

# TBR vs IMR

TBR (Tile-Based Rendering) 和 IMR (Immediate Mode Rendering) 是两种不同的渲染架构，前者常见于移动 GPU (Imagniation, Mali), 后者常见于桌面 GPU (NVIDIA, AMD)。

# References

- [Tutorial 14: Render To Texture](https://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/)
- [渲染架构比较：IMR,TBR & TBDR](https://zhuanlan.zhihu.com/p/390625258)
