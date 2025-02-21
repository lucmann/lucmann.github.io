---
title: GPU Firmware
date: 2025-02-21 07:57:06
tags: firmware
categories: linux
---

```
VCE feature version: 0, firmware version: 0x00000000
UVD feature version: 0, firmware version: 0x40000d00
MC feature version: 0, firmware version: 0x00a777d0
ME feature version: 29, firmware version: 0x00000091
PFP feature version: 29, firmware version: 0x00000054
CE feature version: 29, firmware version: 0x0000003d
RLC feature version: 1, firmware version: 0x00000001
RLC SRLC feature version: 0, firmware version: 0x00000000
RLC SRLG feature version: 0, firmware version: 0x00000000
RLC SRLS feature version: 0, firmware version: 0x00000000
RLCP feature version: 0, firmware version: 0x00000000
RLCV feature version: 0, firmware version: 0x00000000
MEC feature version: 0, firmware version: 0x00000000
IMU feature version: 0, firmware version: 0x00000000
SOS feature version: 0, firmware version: 0x00000000
ASD feature version: 0, firmware version: 0x00000000
TA XGMI feature version: 0x00000000, firmware version: 0x00000000
TA RAS feature version: 0x00000000, firmware version: 0x00000000
TA HDCP feature version: 0x00000000, firmware version: 0x00000000
TA DTM feature version: 0x00000000, firmware version: 0x00000000
TA RAP feature version: 0x00000000, firmware version: 0x00000000
TA SECUREDISPLAY feature version: 0x00000000, firmware version: 0x00000000
SMC feature version: 0, program: 16, firmware version: 0x10020000 (2.0.0)
SDMA0 feature version: 0, firmware version: 0x00000000
SDMA1 feature version: 0, firmware version: 0x00000000
VCN feature version: 0, firmware version: 0x00000000
DMCU feature version: 0, firmware version: 0x00000000
DMCUB feature version: 0, firmware version: 0x00000000
TOC feature version: 0, firmware version: 0x00000000
MES_KIQ feature version: 0, firmware version: 0x00000000
MES feature version: 0, firmware version: 0x00000000
VBIOS version: 113-xxx-xxx
```

<!--more-->

现代 GPU/NPU 芯片里一般都会搭载多个微处理器(或微控制器)， 像 Intel [GuC](https://docs.kernel.org/gpu/i915.html#guc) (uC 指 µC, micro controller), AMD [MEC](https://docs.kernel.org/gpu/amdgpu/driver-core.html#graphics-and-compute-microcontrollers)(MicroEngine Compute), NVIDIA [GSP](https://download.nvidia.com/XFree86/Linux-x86_64/510.39.01/README/gsp.html) (GPU System Processor)。跑在这些微控制器上的代码一般以**固件 (Firmware)**的形式存在，即使是在 Linux 内核里也是二进制格式，源代码一般是不公开的，Linux 社区有一个专门的 git tree [*linux-firmware*](https://git.kernel.org/pub/scm/linux/kernel/git/firmware/linux-firmware.git/tree/) 来维护各种硬件的 **firmware/blob**。

上面的 firmware 信息是从 AMD R7340 显卡机器上输出的 (under root privilege)

```bash
cat /sys/kernel/debug/dri/0/amdgpu_firmware_info
```
