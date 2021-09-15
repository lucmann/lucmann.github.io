---
title: ARB_point_parameters
date: 2020-06-05 13:06:50
tags: [OpenGL]
categories: graphics
---

# 概述(#14)

ARB_point_parameters允许用户设置点的扩展几何特征。这些点的几何特征主要用来渲染粒子或微小光源，通常被称为光点(Light Points).

<!--more-->

光栅化后的点的明亮度由多个因素决定: 点的面积(Area), 点的颜色(Color), 点的透明度(Transparency), 以及电子枪和磷光粉之间的反应。而点的面积和点的透明度由点的大小(Size)推导而来，在ARB_point_parameters之前，点的大小`size`这个参数由 `glPointSize`指定。

ARB_point_parameters扩展主要实现两个目的:

1. 点的大小(Size)要受距离衰减的影响，就是点的大小要随着点离观察点的距离的增大而减小。

这个扩展定义的点的明亮度的距离衰减公式是:


$$Attenuation(d) = \frac{1}{a + b \times d + c \times d^2}$$

$$Brightness(Pe) = Brightness \times Attenuation(\lvert Pe \rvert)$$


这里，
- **Pe** 指在眼睛坐标系里的一个点(Point in eye coordinates)
- **Brightness** 是与点的大小成正比的一个初始值
- **a**, **b**, **c** 是由`glPointParameterfv()`通过参数`GL_POINT_DISTANCE_ATTENUATION`传入的衰减因子

2. 点的大小(Size)到点的面积和透明度的映射要受一个门限的控制，就是点的面积小于一个门限后，点的Alpha分量(一般是Color的第4个分量，它决定透明度，Alpha等于0，表示完全透明)要随面积的减小而减小。

这个扩展定义的点的最终的Alpha(Pe)分量的计算公式是:

$$
Area(Pe) = \begin{cases}
  Brightness(Pe), & \text{if } Brightness(Pe) \geq ThresholdArea \\
  ThresholdArea,  & \text{otherwise }
\end{cases}
$$

$$Factor(Pe) = Brightness(Pe) / ThresholdArea$$

$$Alpha(Pe) = \alpha * Factor(Pe)$$

这里，
- $\alpha$ 是点的Color(RGBA)的第4个分量
- **ThresholdArea** 是与`glPointParameterf()`通过参数`GL_POINT_FADE_THRESHOLD_SIZE`传入的值的平方成正比的一个门限值

