---
title: 更换系统GCC版本
date: 2020-10-29 19:41:12
tags: gcc
---

# GCC版本
GCC是GNU Compiler Collection的缩写，指的是GNU项目下所有编译器的集合。编译器是用来构建可执行程序的程序，很多软件系统对GCC的版本有依赖或者要求，因为有些高级语言特性只有高版本的GCC才支持。编译器除了编译程序本身，还配套有所依赖的库和头文件。一般的系统只有一个默认的编译器，它也可以同时安装多个版本的编译器。

# GCC版本的切换
Ubuntu和CentOS系统都有一个`update-alternatives`的工具，用它可以快速干净的切换系统的编译器版本。它实质上是创建符号链接指向系统安装的不同版本的GCC. 比方系统上同时安装了gcc-4.8和gcc-8,那我们可以这样将`/usr/bin/gcc`指向gcc-4.8
```shell
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 11
```

`update-alternatives`的基本用法是：

update-alternatives --install <u>link</u> <u>name</u> <u>path</u> <u>priority</u>

其中，priority是越大优先级越高。

如果需要查看某个alternative当前的链接有哪些：

update-alternatives --display <u>name</u>

如果需要删除某个alternative的某个目标路径：

update-alternatives --remove <u>name</u> <u>path</u>
