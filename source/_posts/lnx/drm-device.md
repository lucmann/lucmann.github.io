---
title: DRM Device
date: 2021-04-07 15:49:16
tags: [DRM/KMS]
categories: graphics
---

# DRM Introduction
[Direct Rendering Manager](https://en.wikipedia.org/wiki/Direct_Rendering_Manager)是Linux kernel中负责与GPU接口的子系统，但它不仅仅用在Linux, 它也会用在像[OpenBSD](https://zh.wikipedia.org/zh-cn/OpenBSD)的其它一些类UNIX系统。

<!--more-->

# DRM Device Nodes

DRM Device node 有 3 种

- primary (cardn), 如 `/dev/dri/card0`
- renderDn, 如 `/dev/dri/renderD128`
- controlDn, 如 `/dev/dri/controlD0`

在各种 UMD 和 Compositor 的实现中使用比较多的是 cardn 和 renderDn 节点， 两者的主要区别在于文件权限，访问 cardn 需要 root 权限，cardn 是 Linux DRM 历史遗留产物，在现代图形应用中， 一般推荐使用 renderDn 节点。 比如 Xorg 使用的就是 cardn 节点。

关于 primary 节点，需要了解:
- 无论它是低端的显示卡，还是 SoC，或像 Intel/AMD/NVIDIA 那些桌面 GPU, 只要注册一个 DRM 设备就都会默认有 primary 节点 `/dev/dri/card123`
- primary 节点和 render 节点的主要区别在**权限**， 操作 (open, ioctl) primary 节点需要 root 权限， 而 render 节点不需要
- primary 节点和 render 节点底层可以是同一个物理设备，也可以是不同的物理设备，比如在一个使用 Mali GPU IP 和 MTK display controller IP 的 MTK SoC 上， MTK 设备只有显示能力 (`/dev/dri/card0`), 而 Mali 设备只有渲染能力，但它同时会有 primary 节点 (`/dev/dri/card1`) 和 render 节点 (`/dev/dri/renderD128`).
- render 节点是专门用来执行非全局的渲染命令的（那些全局的 modeset 命令是通过 primary 节点执行的)，如果一个驱动不支持 render 节点，那么它只能通过 primary 节点和 `drmAuth` 功能一起完成渲染任务。

## Major Number 
DRM设备的主设备号在不同的系统上不同。

| OS		    | Major Number |
|:--------------|:-------------|
| DragonFlyBSD  | 145          |
| NetBSD        | 34           |
| OpenBSD       | 88/87        |
| Linux         | 226          |

## Minor Number
每种类型的DRM设备都有一个**Base Minor**, 每种类型允许的子设备号个数是`64` 个. 所以，Linux下最大的DRM Render Node设备文件名是`/dev/dri/renderD191`.

```c
		r = idr_alloc(&drm_minors_idr,
			NULL,
			64 * type,
			64 * (type + 1),
			GFP_NOWAIT);
```

{% asset_img "drm-minor.png" %}

## Name Convention
DRM设备在文件系统中的文件名规则在不同的系统上不同。

|                       | OpenBSD         | Linux            |
|:----------------------|:----------------|:-----------------|
| DIR_NAME              | `/dev`          | `/dev/dri`       |
| PRIMARY_MINOR_NAME    | `drm`           | `card`           |
| CONTROL_MINOR_NAME    | `drmC`          | `controlD`       |
| RENDER_MINOR_NAME     | `drmR`          | `renderD`        |


1. [drm: implement experimental render nodes](https://cgit.freedesktop.org/~airlied/linux/commit/?h=drm-next&id=1793126fcebd7c18834f95d43b55e387a8803aa8)
