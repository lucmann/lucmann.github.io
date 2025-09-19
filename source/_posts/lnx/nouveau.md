---
title: Nouveau
date: Mon Sep  8 13:18:33 CST 2025
tags: [kernel]
categories: graphics
---

```mermaid
---
config:
    theme: 'default'
    themeVariables:
        cScale0: '#d3d3d3'
        cScaleLabel0: '#000000'
        cScale1: '#a6a6a6'
        cScaleLabel1: '#000000'
        cScale2: '#76b900'
        cScaleLabel2: '#ffffff'
---
timeline
    title NVIDIA Desktop GPUs History
    section early NVxx era
        1999 : NV04
        2004 : NV40
        2006 : NV50 : Tesla : Introduction of per context VMA
    section pre-Turing era (no reclocking)
        2010 : GF1xx : NVC0 - Fermi
        2012 : GK1xx : NVE0 - Kepler : Vulkan support : nouveau start support reclocking
        2014 : GM1xx : Maxwell
        2014 : GM2xx : Maxwell 2 : Start of signed firmware thus little chance to reclock
        2016 : GP1xx : Pascal
        2017 : GV1xx : Volta
    section New era for open source (GSP)
        2018 : TU1xx : Turing : GPU System Processor support
        2020 : GA1xx : Ampere
        2022 : AD1xx : Ada Lovelace
        2025 : GB2xx : Blackwell
```
<!--more-->

![](/images/nouveau/benchmarks-on-gp108.png)

# Reference

- [GPU Architecture Overview](https://insujang.github.io/2017-04-27/gpu-architecture-overview/)
- [LWN: NVIDIA GPU 和 Nouveau](https://blog.csdn.net/Linux_Everything/article/details/127780944)
- [List of Nvidia GPUs](https://en.wikipedia.org/wiki/List_of_Nvidia_graphics_processing_units)
