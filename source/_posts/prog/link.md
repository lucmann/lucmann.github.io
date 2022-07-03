---
title: Notes on Linker on Linux
date: 2022-06-15 17:02:49
tags: linker
categories: programming
---

动态链接库 (shared library) 无处不在。使用动态链接库基本上是通过链接器 (linker, generally a program suffixing with "ld")。本文主要回答以下问题:

<!--more-->

- 有哪些链接器?
- 如何查看一个可执行文件链接哪些动态库?
- 链接器从哪里找到动态库?
