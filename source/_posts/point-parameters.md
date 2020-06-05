---
title: ARB_point_parameters
date: 2020-06-05 13:06:50
tags: [OpenGL]
---

## 概述
作为ARB Extension #14(编号)，这个OpenGL Extension支持允许用户设置点的扩展几何特征。这些点的几何特征主要用来渲染粒子或微小光源，通常被称为光点(Light Points).

光栅化后的点的明亮度由多个因素决定: 点的面积(Area), 点的颜色(Color), 点的透明度(Transparency), 以及电子枪和磷光粉之间的反应。而点的面积和点的透明度由点的大小(Size)推导而来，在ARB_point_parameters之前，点的大小`size`这个参数由 `glPointSize`指定。

ARB_point_parameters扩展主要实现两个目的:

1. 点的大小(Size)要受距离衰减的影响，就是点的大小要随着点离观察点的距离的增大而减小。
2. 点的大小(Size)到点的面积和透明度的映射要受一个门限的控制，就是点的面积小于一个门限后，点的Alpha分量(一般是Color的第4个分量，它决定透明度，Alpha等于0，表示完全透明)要随面积的减小而减小。

这个扩展定义的点的明亮度的距离衰减公式是:

{% katex [displayMode] %} 

dist_atten(d) = \frac{1}{a + b \times d + c \times d^2}

{% endkatex %}
