---
title: OpenGL (ES) and GLSL (ES) Version
date: 2021-03-31 11:52:59
tags: [GL]
categories: media
---

# Overview
本文梳理以下4个由[Khronos](https://www.khronos.org/)组织发布的图形相关的标准。

<!--more-->

| Standard | Birth |
|:---------|:-----:|
| OpenGL   | 1992  |
| OpenGL ES| 2003  |
| GLSL     | 2004  |
| GLSL ES  | 2007  |

# OpenGL Version
| GL Version | Release | GLSL Version | Preprocessor |
|:----------:|:-------:|:------------:|:-------------|
| 1.0        | 1992    |              |              | 
| 1.1        | 1997    |              |              |
| 1.2        | 1998    |              |              |
| 1.3        | 2001    |              |              |
| 1.4        | 2002    |              |              |
| 1.5        | 2003    |              |              |
| 2.0        | 2004    | 1.10         | #version 110 |
| 2.1        | 2006    | 1.20         | #version 120 |
| 3.0        | 2008    | 1.30         | #version 130 |
| 3.1        | 2009    | 1.40         | #version 140 |
| 3.2        | 2009    | 1.50         | #version 150 | 
| 3.3        | 2010    | 3.30         | #version 330 |
| 4.0        | 2010    | 4.00         | #version 400 |
| 4.1        | 2010    | 4.10         | #version 410 |
| 4.2        | 2011    | 4.20         | #version 420 |
| 4.3        | 2012    | 4.30         | #version 430 |
| 4.4        | 2013    | 4.40         | #version 440 |
| 4.5        | 2014    | 4.50         | #version 450 |
| 4.6        | 2017    | 4.60         | #version 460 |

# OpenGL (Shading Language) ES Version
| ES Version | Release | GLSL ES Version | Preprocessor   | Based on GLSL |
|:----------:|:-------:|:---------------:|:---------------|:-------------:|
| 1.0        | 2003    |                 |                |               | 
| 1.1        | 2004    |                 |                |               |
| 2.0        | 2007    | 1.00            | #version 100   | 1.20          |
| 3.0        | 2012    | 3.00            | #version 300 es| 3.30          |
| 3.1        | 2014    | 3.10            | #version 310 es| 3.30          |
| 3.2        | 2015    | 3.20            | #version 320 es| 3.30          |

# FAQ
1. 为什么GLSL ES 1.00后面是3.00版本？

在GLSL ES 3.00 Spec里有这样一段说明：
{% blockquote %}
This version of the language is based on version 3.30 of the desktop GLSL. However it includes a number of features that are in version 4.20 but not 3.30. The previous version of GLSL ES was version 1.00 so this version could be called version 2.00.

RESOLUTION: Follow the desktop GLSL convention so that the language version matches the API version. Hence this version will be called 3.00
{% endblockquote %}

GLSL ES 3.10 & 3.20里同样有这段说明，所以，3.00主要是为了对应OpenGL ES API的版本号，3.10, 3.20同样是。

