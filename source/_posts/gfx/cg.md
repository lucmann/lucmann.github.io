---
title: OpenGL 中的一些概念对比
date: 2024-12-18 21:21:27
tags: OpenGL
categories: graphics
---

OpenGL 中的有些概念很相似，有些标准里也没有给出一个明确的定义，有些如果仅从字面意思理解很容易混淆，这里将平时自己查询的资料及自己的理解整理到一起，以备不时之需。

- [Clip vs Cull](https://computergraphics.stackexchange.com/questions/9214/whats-the-difference-between-clipping-and-culling)

<!--more-->

# Clip vs Cull

- Clip 是一个**做加减法** 的过程，就是它会将处在 clip-space 边界的图元分成多个图元，新生成的在 clip-space 之外的图元就丢弃掉了
- Cull 是一个**做纯减法** 的过程，它不会生成任何新的图元，只会丢弃图元
