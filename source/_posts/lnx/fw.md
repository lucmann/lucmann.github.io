---
title: GPU Firmware
date: 2025-02-21 07:57:06
tags: firmware
categories: linux
---

```
UVD feature version: 0, firmware version: 0x40000d00
MC feature version: 0, firmware version: 0x00a777d0
ME feature version: 29, firmware version: 0x00000091
PFP feature version: 29, firmware version: 0x00000054
CE feature version: 29, firmware version: 0x0000003d
RLC feature version: 1, firmware version: 0x00000001
SMC feature version: 0, program: 16, firmware version: 0x10020000 (2.0.0)
VBIOS version: 113-xxx-xxx
```

<!--more-->

现代 GPU/NPU 芯片里一般都会搭载多个微处理器(或微控制器)， 像 Intel [GuC](https://docs.kernel.org/gpu/i915.html#guc) (uC 指 µC, micro controller), AMD [MEC](https://docs.kernel.org/gpu/amdgpu/driver-core.html#graphics-and-compute-microcontrollers)(MicroEngine Compute), NVIDIA [GSP](https://download.nvidia.com/XFree86/Linux-x86_64/510.39.01/README/gsp.html) (GPU System Processor)。跑在这些微控制器上的代码一般以**固件(Firmware)** 的形式存在，即使是在 Linux 内核里也是二进制格式，源代码一般是不公开的，Linux 社区有一个专门的 git tree [*linux-firmware*](https://git.kernel.org/pub/scm/linux/kernel/git/firmware/linux-firmware.git/tree/) 来维护各种硬件的固件二进制文件，而这些 bin 文件在 Linux 系统中会存储在文件系统， 路径一般是 `/lib/firmware/`。

例如，在一个 5.4.18-85-generic 的 Linux 发行版系统中， amdgpu 和 radeon 的 gpu ip 固件 bin 文件就分别有 **469**, **247** 个

```bash
for dir in amdgpu radeon; do ls /lib/firmware/$dir | wc; done
```

而且 KMD 也提供 sys 文件系统接口允许用户查看固件信息 (需要 root 权限)， 例如，在 AMD R7340 显卡机器上输出本文开头的固件信息

```bash
cat /sys/kernel/debug/dri/0/amdgpu_firmware_info | grep -v 'firmware version: 0x00000000'
```
