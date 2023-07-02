---
title: Linux Graphics Application Common Issues
date: 2023-07-01
tags: [mesa]
categories: graphics
---

# 应用启动时 MESA-LOADER 报错

常见的错误包括如下。这种错误的原因一般是 Mesa gallium 的驱动动态链接库(这里不是指 libGL.so.1, 而是 DRIVERNAME_dri.so)加载出现问题，一般是

- 动态链接库不存在
- 动态链接库存在，但 dlopen 时发生错误

第2种情况就是下面第1次错误报出的情况。dl.so 报错是 No such file or directory (), 但是如果你去 /usr 下搜索 swrast_dri.so, 该动态库又是存在的。原因是就是 /usr/lib/x86_64-linux-gnu/dri/swrast_dri.so dlopen 时出错了，又驱动动态链接库的 search paths 是一个列表，MESA-LOADER 打开第1个搜索到 swrast_dri.so 失败，又去下一个搜索路径下搜索，在下面的这种情况下，即去 /usr/lib/dri 下

<!--more-->

```
➜  mesa git:(main) ✗ unset LIBGL_DRIVERS_PATH
➜  mesa git:(main) ✗ glxinfo -B
name of display: :0
libGL error: MESA-LOADER: failed to open swrast: /usr/lib/dri/swrast_dri.so: cannot open shared object file: No such file or directory (search paths /usr/lib/x86_64-linux-gnu/dri:\$${ORIGIN}/dri:/usr/lib/dri, suffix _dri)
libGL error: failed to load driver: swrast
X Error of failed request:  BadValue (integer parameter out of range for operation)
  Major opcode of failed request:  148 (GLX)
  Minor opcode of failed request:  24 (X_GLXCreateNewContext)
  Value in failed request:  0x0
  Serial number of failed request:  63
  Current serial number in output stream:  64
➜  mesa git:(main) ✗ export LIBGL_DRIVERS_PATH=/usr/lib/x86_64-linux-gnu/dri:/usr/lib/dri
➜  mesa git:(main) ✗ glxinfo -B
name of display: :0
libGL error: MESA-LOADER: failed to open swrast: /usr/lib/dri/swrast_dri.so: cannot open shared object file: No such file or directory (search paths /usr/lib/x86_64-linux-gnu/dri:/usr/lib/dri, suffix _dri)
libGL error: failed to load driver: swrast
X Error of failed request:  BadValue (integer parameter out of range for operation)
  Major opcode of failed request:  148 (GLX)
  Minor opcode of failed request:  24 (X_GLXCreateNewContext)
  Value in failed request:  0x0
  Serial number of failed request:  63
  Current serial number in output stream:  64
```
