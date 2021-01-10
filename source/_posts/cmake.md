---
title: CMake
date: 2021-01-10 07:42:04
tags:
categories: /bin
---

最近看了一些关于Modern CMake(since 3.0.0)的教程，这里记录一下.

# Guideline
- Don't make any assumption about the platform and compiler.
- Make sure that all your projects can be built both standalone and as a subproject of another project.
- Always add namespaced aliases for libraries.
    ```cmake
    add_library(foo STATIC
      foo1.cpp
      foo2.cpp
      )
    add_library(my::foo ALIAS foo)
    ```
- Don't make libraries STATIC/SHARED unless they cannot be built otherwise.
- Leave the control of BUILD_SHARED_LIBS to your clients.
