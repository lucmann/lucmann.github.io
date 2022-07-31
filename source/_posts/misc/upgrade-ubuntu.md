---
title: Sogou IM on Ubuntu Jammy Jellyfish
date: 2022-07-31 20:47:35
tags: ubuntu
categories: misc
---

# Reminiscence

<!--more-->

# Display Drivers for NVIDIA GT 520M

![Before NVIDIA Display Driver Installed](before-install-driver.png)

# Sogou IM

Sogou 输入法真心做得不错，支持主流的国产 Linux 操作系统。今天将系统升级到 Ubuntu 22.04.1 LTS 后之前安装的 Sogoupinyin 突然不工作了。刚开始没想到是缺少 `libqt5qml5` 这个依赖，一顿重装 fcitx, sogoupinyin 操作后，还是不行，google 后才发现原来 sogoupinyin 安装后，可执行程序都在

```
/opt/sogoupinyin/files/bin
```

通过运行这个目录下的 `sogoupinyin-service` 这个程序后才发现它无法加载 libqt5qml5.

另外，`sogoupinyin-configtool` 也在上面这个目录，而且 sogoupinyin 也支持五笔

![sogoupinyin config interface](sogoupinyin-configtool.png)
