---
title: Wayland
date: 2021-04-01 16:31:25
tags: [xorg]
categories: sys
---

# Wayland Architecture
为了理解wayland的架构和它与X的区别，最好是看看从一个输入事件到这个输入事件的结果呈现到屏幕上这个过程都发生了什么。

<!--more-->

{% asset_img x-architecture.png "X Architecture" %}

1. kernel获取到一个输入事件，通过`evdev`输入设备驱动发送给X Server.这里kernel做了很多工作，包括驱动输入设备，转换各种不同的设备事件协议为标准的Linux `evdev`输入事件。

2. X Server决定这个事件影响哪个窗口，并发送这个事件给该窗口所属的那些客户端，但问题是，X Server并不能真正确定是哪个窗口，因为在屏幕坐标里的窗口位置是由compositor控制的，窗口位置可以被很多操作改变，缩小，旋转，晃动等等，而X Server并不知晓这些操作。

3. 客户端收到这个事件，然后决定做什么。通常对事件的响应是UI的改变，也许是一个多选框被打勾，也许是鼠标落到一个按钮上，按钮高亮。因此客户端又向X Server发回一个渲染请求。

4. 当X Server收到渲染请求后，又转发给驱动，由驱动控制图形硬件完成渲染，这里的驱动可能是像AMD显卡的驱动，也可能是一个像Mesa这样的OpenGL软件实现。X Server负责计算渲染区域的边界，以一个`damage event`发送结果给compositor.

5. `damage event`告诉compositor, 窗口的内容已经改变，如果这个窗口是可见的，那么需要重新合成屏幕的这一部分。compositor基于它的场景图和X Window的内容负责渲染整个屏幕，但是它还得通过X Server去完成渲染，因为只有X Server才能和驱动打交道。

6. X Server收到compositor的渲染请求后，或者拷贝compositor back buffer到front buffer,或者做一次pageflip. 通常情况下，X Server必须做这一步，因为它需要计算窗口重叠，这可能需要裁剪操作，并且决定它是否可以pageflip. 但是，对于总是执行全屏操作的compositor来说，这又是一次不必要的上下文切换。

上面的X Server的工作流程有一些问题。首先, X Server没有相关的信息去决定到底哪个窗口应该接收这个输入事件，它也不能将屏幕坐标转换成窗口内部坐标，甚至X Server把最后的屏幕渲染工作的职责都交给了合成管理器，X Server仍然控制着front buffer和modesetting. X Server过去大多数复杂的工作现在要么是在kernel中可以完成(`KMS`, `evdev`), 要么可以在一些自包含的库里完成(`mesa`, `fontconfig`, `freetype`, `cairo`, `Qt`, 等等). 总的来说，X Server现在已经成了在应用程序和compositor之间，compositor和硬件之间引入额外步骤的中间人了。


在wayland里，compositor就是**Display Server**. 我们把`KMS`和`evdev`的控制权移交给compositor. Wayland协议让compositor直接发送输入事件给客户端，让客户端直接发送`damage event`给compositor.

{% asset_img wayland-architecture.png "Wayland Architecture" %}

1. kernel发送输入事件给compositor, 这一步与X Server类似，这没有问题，因为这样可以复用内核中所有的输入设备驱动。

2. compositor浏览它的场景图来决定输入事件应该发给哪个窗口。场景图对应屏幕上的显示，compositor知晓它已经应用到场景图中各个元素的坐标变换，因此compositor可以正确选择哪个窗口接收输入事件，也可以将屏幕坐标转换为窗口内部坐标通过逆变换。所能应用到某个窗口的变换类型完全取决于compositor的实现，只要它可以计算出对应输入事件的逆变换。

3. 和X Server的场景类似，当客户端收到输入事件后，它相应地更新UI. 但在wayland下，客户端不再发送渲染请求给谁，渲染任务由客户端自己完成，客户端只需要发送消息告诉compositor哪块区域已经更新了。

4. compositor收集来自它的客户端的`damage event`, 然后重新合成整个屏幕。compositor能直接发送`ioctl`请求给KMS执行一次pageflip.


[[阅读原文]](https://wayland.freedesktop.org/architecture.html)

