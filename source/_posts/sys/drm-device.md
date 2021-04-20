---
title: DRM Device
date: 2021-04-07 15:49:16
tags: [dri]
categories: sys
---

# DRM Introduction
[Direct Rendering Manager](https://en.wikipedia.org/wiki/Direct_Rendering_Manager)是Linux kernel中负责与GPU接口的子系统，但它不仅仅用在Linux, 它也会用在像[OpenBSD](https://zh.wikipedia.org/zh-cn/OpenBSD)的其它一些类UNIX系统。

<!--more-->

# DRM Device
## Type
DRM Device分为3类：
- primary
- control
- render

例如:
- `/dev/dri/card0`
- `/dev/dri/controlD0`
- `/dev/dri/renderD0`

## Major Number 
DRM设备的主设备号在不同的系统上不同。

| OS		| Major Number |
|:--------------|:-------------|
| DragonFlyBSD  | 145          |
| NetBSD        | 34           |
| OpenBSD       | 88/87        |
| Linux         | 226          |

## Minor Number
每种类型的DRM设备都有一个**Base Minor**, 每种类型允许的子设备号个数是`16`. 所以，Linux下最大的DRM设备号是`/dev/dri/renderD144`.

{% asset_img "drm-minor.png" %}

## Name Convention
DRM设备在文件系统中的文件名规则在不同的系统上不同。

|                       | OpenBSD         | Linux            |
|:----------------------|:----------------|:-----------------|
| DIR_NAME              | `/dev`          | `/dev/dri`       |
| PRIMARY_MINOR_NAME    | `drm`           | `card`           |
| CONTROL_MINOR_NAME    | `drmC`          | `controlD`       |
| RENDER_MINOR_NAME     | `drmR`          | `renderD`        |


