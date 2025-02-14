---
title: Android Glossary
date: 2025-02-14 11:38:10
tags: [Android]
categories: linux
---

# 1 号进程 - init

在 Android 系统中， 1 号进程 (PID 1) 是 **init** 进程。它是系统启动后由 Linux 内核直接创建的第一个用户空间进程，负责初始化系统环境并启动关键服务。

```mermaid
mindmap
  root((kernel))
    init<br>PID 1
      zygote<br>后续启动的 Java App 均由它 fork
      ServiceManager<br>Binder IPC的核心服务
      SurfaceFlinger<br>GPU compositer
      adbd<br>Android Debug Bridge Daemon
      ueventd<br>管理 /dev 设备节点
```

# Apex - Android Pony EXpress

**Apex** 是 Android 10 引入的容器格式，用于在系统启动早期安装或加载不适用 APK 安装的底层组件，如 Native Service, HAL, ART 等。

# Zygote
