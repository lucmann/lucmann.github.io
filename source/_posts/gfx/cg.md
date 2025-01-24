---
title: OpenGL 中的一些概念对比
date: 2024-12-18 21:21:27
tags: OpenGL
categories: graphics
---

OpenGL (可能也有其它与 OpenGL 有关的)中的有些概念很相似，有些标准里也没有给出一个明确的定义，有些如果仅从字面意思理解很容易混淆，这里将平时自己查询的资料及自己的理解整理到一起，以备不时之需。

- [Clip vs Cull](https://computergraphics.stackexchange.com/questions/9214/whats-the-difference-between-clipping-and-culling)
- [Glyph vs Font](https://graphicdesign.stackexchange.com/questions/45162/what-is-the-difference-between-glyph-and-font)

<!--more-->

# Clip vs Cull

- Clip 是一个**做加减法** 的过程，就是它会将处在 clip-space 边界的图元分成多个图元，新生成的在 clip-space 之外的图元就丢弃掉了
- Cull 是一个**做纯减法** 的过程，它不会生成任何新的图元，只会丢弃图元

# [`glFrustum` vs `glm::perspective`](https://blog.csdn.net/iteye_13202/article/details/82490241?spm=1001.2101.3001.6650.2&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-2-82490241-blog-133176951.235%5Ev43%5Epc_blog_bottom_relevance_base9&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-2-82490241-blog-133176951.235%5Ev43%5Epc_blog_bottom_relevance_base9&utm_relevant_index=5)

```c
glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
glm::mat4 glm::perspective(float fovy, float aspect, float zNear, float zFar);
```
