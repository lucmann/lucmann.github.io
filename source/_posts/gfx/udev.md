---
title: udev, sysfs & libudev
date: 2021-05-19 11:38:10
tags: [linux]
categories: graphics
---

# udev
udev是Linux系统管理`/dev`的内核组件，它负责在系统运行时动态的创建和删除`/dev`下的设备文件(节点)。

- 当设备被检测到时创建设备文件
- 当设备被移除时删除设备文件
- `/etc/udev/rules.d`允许用户自定义设备文件的权限，路径和符号链接

udev显然是支持Hotplug, 而且保证`/dev`下的设备节点是实时更新的。而且它强大的用户自定义行为非常灵活。但它的问题是：

- 用户自定义规则的存在让应用程序很难确定特定的设备文件和设备类型
- 同一类型的多个设备，设备文件的创建顺序是不确定的

# Reference
[libudev and sysfs tutorial](https://blog.csdn.net/fjb2080/article/details/7528894)
