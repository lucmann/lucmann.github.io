---
title: Ubuntu 的 OpenGL 环境
date: 2024-03-29 15:00:56
tags: [OpenGL]
---

# GLVND(Vendor Neutral GL dispatch library)

| Source Directory |  Target              | APT Package                    |
|:-----------------|:---------------------|:-------------------------------|
| GLX              |                      |                                |
| EGL              |                      |                                |
| GLDispatch       | libGLdispatch.so     | libglvnd0                      |
| OpenGL           | libOpenGL.so         | libopengl0                     |
| GLESv1           | libGLESv1_CM.so      | libgles1                       |
| GLESv2           | libGLESv2.so         | libgles2                       |
| GL               | libGL.so.1.7.0       | libgl1                         |

<!--more-->

# Mesa

| APT Package      |  Target              | Description                    |
|:-----------------|:---------------------|:-------------------------------|
| libglapi-mesa    | libglapi.so          | free implementation of GL API  |
| libgles2-mesa    |                      | transitional dummy package     |
|                  |                      |                                |
|                  |                      |                                |
|                  |                      |                                |


