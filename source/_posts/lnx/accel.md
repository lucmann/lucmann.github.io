---
title: Linux accel 子系统
date: 2024-09-06 10:42:45
tags: accel
categories: linux
---

Linux 计算加速器子系统旨在以通用的方式向用户空间开放计算加速器的接口。这些计算加速器可以是独立的 ASIC, 也可以是 SoC/GPU 内的 IP 块。通常计算加速器包括以下几种类型:

- 边缘 AI - 在边缘设备上进行推理计算
- 推理数据中心 - 大型服务器中的单用户/多用户设备
- 训练数据中心 - 类似于推理数据中心卡，但通常具有更强的计算能力和更高的存储带宽 

<!--more-->

```
if DRM

menuconfig DRM_ACCEL
	bool "Compute Acceleration Framework"
	help
	  Framework for device drivers of compute acceleration devices, such
	  as, but not limited to, Machine-Learning and Deep-Learning
	  acceleration devices.
	  If you say Y here, you need to select the module that's right for
	  your acceleration device from the list below.
	  This framework is integrated with the DRM subsystem as compute
	  accelerators and GPUs share a lot in common and can use almost the
	  same infrastructure code.
	  Having said that, acceleration devices will have a different
	  major number than GPUs, and will be exposed to user-space using
	  different device files, called accel/accel* (in /dev, sysfs
	  and debugfs).

source "drivers/accel/habanalabs/Kconfig"
source "drivers/accel/ivpu/Kconfig"
source "drivers/accel/qaic/Kconfig"

endif
```

下面是 accel 子系统被合入主线时的 commit message

> drivers/accel: define kconfig and register a new major
> 
> Add a new Kconfig for the accel subsystem. The Kconfig currently
> contains only the basic CONFIG_DRM_ACCEL option that will be used to
> decide whether to compile the accel registration code. Therefore, the
> kconfig option is defined as bool.
> 
> The accel code will be compiled as part of drm.ko and will be called
> directly from the DRM core code. The reason we compile it as part of
> drm.ko and not as a separate module is because of cyclic dependency
> between drm.ko and the separate module (if it would have existed).
> This is due to the fact that DRM core code calls accel functions and
> vice-versa.
> 
> The accelerator devices will be exposed to the user space with a new,
> dedicated major number - 261.
> 
> The accel init function registers the new major number as a char device
> and create corresponding sysfs and debugfs root entries, similar to
> what is done in DRM init function.
> 
> I added a new header called drm_accel.h to include/drm/, that will hold
> the prototypes of the drm_accel.c functions. In case CONFIG_DRM_ACCEL
> is set to 'N', that header will contain empty inline implementations of
> those functions, to allow DRM core code to compile successfully
> without dependency on CONFIG_DRM_ACCEL.
> 
> I Updated the MAINTAINERS file accordingly with the newly added folder
> and I have taken the liberty to appropriate the dri-devel mailing list
> and the dri-devel IRC channel for the accel subsystem.

目前 accel 下已经有 3 个 AI 加速芯片的驱动:

- **habanalabs.ko** - Intel 旗下的哈瓦那 (Habana) AI 处理器 (AIP)
- **intel_vpu.ko** - Intel Neural Proccessing Unit (NPU) (之间叫 Versatile Proccessing Unit)
- **qaic.ko** - Qualcomm Cloud AI 加速器
