---
title: Installable Client Driver
date: 2023-04-23 09:08:05
tags: vulkan
categories: utilities
---

# 来自 [ChatGPT](https://openprompt.co/ChatGPT) 的解释

## What is Vulkan ICD?

> Vulkan ICD (Installable Client Driver) is a software component that enables the Vulkan API (Application Programming Interface) to interact with the hardware of a computer system. Vulkan is a low-level graphics API developed by the Khronos Group, which provides developers with more control over hardware resources, leading to improved performance and efficiency in graphics-intensive applications such as video games. The Vulkan ICD is responsible for managing the communication between the Vulkan API and the graphics hardware of a system. It is typically provided by the manufacturer of the graphics card as a driver for the system.

## What is OpenCL ICD?

> OpenCL ICD (Installable Client Driver) is a component of OpenCL (Open Computing Language) that enables a computer system to access the processing power of multiple devices, such as CPUs and GPUs, as a unified compute resource. OpenCL is a framework for programming heterogeneous systems, which means systems that consist of multiple processing units with different architectures. The OpenCL ICD manages the communication between the OpenCL API and the devices available on the system, allowing developers to write code that can run on a variety of devices. It provides flexibility to the developers in choosing the device based on the requirements of the application. The OpenCL ICD is typically provided by the manufacturer of the device as a driver for the system.

# `mesa-opencl-icd` 与 `intel-opencl-icd` 的对比

## `mesa-opencl-icd`

```
drwxr-xr-x root/root         0 2022-03-09 00:37 ./
drwxr-xr-x root/root         0 2022-03-09 00:37 ./etc/
drwxr-xr-x root/root         0 2022-03-09 00:37 ./etc/OpenCL/
drwxr-xr-x root/root         0 2022-03-09 00:37 ./etc/OpenCL/vendors/
-rw-r--r-- root/root        19 2022-03-09 00:37 ./etc/OpenCL/vendors/mesa.icd
drwxr-xr-x root/root         0 2022-03-09 00:37 ./usr/
drwxr-xr-x root/root         0 2022-03-09 00:37 ./usr/lib/
drwxr-xr-x root/root         0 2022-03-09 00:37 ./usr/lib/x86_64-linux-gnu/
drwxr-xr-x root/root         0 2022-03-09 00:37 ./usr/lib/x86_64-linux-gnu/gallium-pipe/
-rw-r--r-- root/root  10555528 2022-03-09 00:37 ./usr/lib/x86_64-linux-gnu/gallium-pipe/pipe_iris.so
-rw-r--r-- root/root   7895008 2022-03-09 00:37 ./usr/lib/x86_64-linux-gnu/gallium-pipe/pipe_nouveau.so
-rw-r--r-- root/root   6214144 2022-03-09 00:37 ./usr/lib/x86_64-linux-gnu/gallium-pipe/pipe_r300.so
-rw-r--r-- root/root   7228928 2022-03-09 00:37 ./usr/lib/x86_64-linux-gnu/gallium-pipe/pipe_r600.so
-rw-r--r-- root/root   7677888 2022-03-09 00:37 ./usr/lib/x86_64-linux-gnu/gallium-pipe/pipe_radeonsi.so
-rw-r--r-- root/root   6449632 2022-03-09 00:37 ./usr/lib/x86_64-linux-gnu/gallium-pipe/pipe_swrast.so
-rw-r--r-- root/root   6217504 2022-03-09 00:37 ./usr/lib/x86_64-linux-gnu/gallium-pipe/pipe_vmwgfx.so
-rw-r--r-- root/root   1549288 2022-03-09 00:37 ./usr/lib/x86_64-linux-gnu/libMesaOpenCL.so.1.0.0
drwxr-xr-x root/root         0 2022-03-09 00:37 ./usr/share/
drwxr-xr-x root/root         0 2022-03-09 00:37 ./usr/share/bug/
drwxr-xr-x root/root         0 2022-03-09 00:37 ./usr/share/bug/mesa-opencl-icd/
-rw-r--r-- root/root        44 2022-03-09 00:37 ./usr/share/bug/mesa-opencl-icd/control
-rwxr-xr-x root/root       448 2022-03-09 00:37 ./usr/share/bug/mesa-opencl-icd/script
drwxr-xr-x root/root         0 2022-03-09 00:37 ./usr/share/doc/
drwxr-xr-x root/root         0 2022-03-09 00:37 ./usr/share/doc/mesa-opencl-icd/
-rw-r--r-- root/root       823 2022-03-09 00:37 ./usr/share/doc/mesa-opencl-icd/changelog.Debian.gz
-rw-r--r-- root/root     14241 2022-03-09 00:37 ./usr/share/doc/mesa-opencl-icd/copyright
drwxr-xr-x root/root         0 2022-03-09 00:37 ./usr/share/lintian/
drwxr-xr-x root/root         0 2022-03-09 00:37 ./usr/share/lintian/overrides/
-rw-r--r-- root/root        49 2022-03-09 00:37 ./usr/share/lintian/overrides/mesa-opencl-icd
lrwxrwxrwx root/root         0 2022-03-09 00:37 ./usr/lib/x86_64-linux-gnu/libMesaOpenCL.so -> libMesaOpenCL.so.1
lrwxrwxrwx root/root         0 2022-03-09 00:37 ./usr/lib/x86_64-linux-gnu/libMesaOpenCL.so.1 -> libMesaOpenCL.so.1.0.0
```

## `intel-opencl-icd`

```
drwxr-xr-x root/root         0 2020-04-09 03:55 ./
drwxr-xr-x root/root         0 2020-04-09 03:55 ./etc/
drwxr-xr-x root/root         0 2020-04-09 03:55 ./etc/OpenCL/
drwxr-xr-x root/root         0 2020-04-09 03:55 ./etc/OpenCL/vendors/
-rw-r--r-- root/root        52 2020-04-09 03:55 ./etc/OpenCL/vendors/intel.icd
drwxr-xr-x root/root         0 2020-04-09 03:55 ./usr/
drwxr-xr-x root/root         0 2020-04-09 03:55 ./usr/bin/
-rwxr-xr-x root/root    406064 2020-04-09 03:55 ./usr/bin/ocloc
drwxr-xr-x root/root         0 2020-04-09 03:55 ./usr/lib/
drwxr-xr-x root/root         0 2020-04-09 03:55 ./usr/lib/x86_64-linux-gnu/
drwxr-xr-x root/root         0 2020-04-09 03:55 ./usr/lib/x86_64-linux-gnu/intel-opencl/
-rw-r--r-- root/root   5049688 2020-04-09 03:55 ./usr/lib/x86_64-linux-gnu/intel-opencl/libigdrcl.so
drwxr-xr-x root/root         0 2020-04-09 03:55 ./usr/share/
drwxr-xr-x root/root         0 2020-04-09 03:55 ./usr/share/doc/
drwxr-xr-x root/root         0 2020-04-09 03:55 ./usr/share/doc/intel-opencl-icd/
-rw-r--r-- root/root       389 2020-04-09 03:55 ./usr/share/doc/intel-opencl-icd/changelog.Debian.gz
-rw-r--r-- root/root      5085 2020-04-06 17:05 ./usr/share/doc/intel-opencl-icd/copyright
```

安装 `*-opencl-icd` 软件包并不会安装 OpenCL 开发相关头文件：`<CL/cl.h>`

