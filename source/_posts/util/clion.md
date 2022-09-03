---
title: CLion 使用笔记
date: 2022-09-03 22:18:16
tags: [c++]
categories: utilities
---

# Choice of CLion

<!--more-->

```
CLion 2020.2.5
Build #CL-202.8194.17, built on November 26, 2020
Licensed to CLion Evaluator
Expiration date: October 3, 2022
Runtime version: 11.0.9+11-b944.49 amd64
VM: OpenJDK 64-Bit Server VM by JetBrains s.r.o.
Windows 10 10.0
GC: ParNew, ConcurrentMarkSweep
Memory: 1987M
Cores: 8
Registry: run.processes.with.pty=TRUE
Non-Bundled Plugins: IdeaVIM, Lua, io.zhile.research.ide-eval-resetter, zielu.gittoolbox
```

# On CLion with WSL

## Avoid annoying event notification every startup

- filesystem case-sensitivity mismatch

原因：[WSL Ubuntu 是 case-sensitive OS, 而 Host 侧的 Windows 11 是 case-insensitive OS](https://confluence.jetbrains.com/display/IDEADEV/Filesystem+Case-Sensitivity+Mismatch)

- external file changes sync may be slow

原因：[Native file watcher will not work for the shared and network folders](https://intellij-support.jetbrains.com/hc/en-us/community/posts/115000013130-External-file-changes-sync-may-be-slow)

这两个问题其实都是我在使用 WSL 的原因，避免方法也很简单 `C:\Users\whoami\AppData\Roaming\JetBrains\CLion2020.2\idea.properties`

```
idea.case.sesitive.fs=true
idea.filewatcher.disabled=true
```
