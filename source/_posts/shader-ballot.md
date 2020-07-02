---
title: ARB_shader_ballot
date: 2020-06-09 12:17:07
tags: [OpenGL]
---

# 编号

ARB Extension #183

# 概述

ARB_shader_ballot给一组线程(a group of invocations)提供某些形式的线程间(cross-invocation)通信的能力。它要么是通过广播某个线程作用域里的值，要么是通过一个位数组(bitarray)表示一组线程中每个线程作用域里的可能的值。