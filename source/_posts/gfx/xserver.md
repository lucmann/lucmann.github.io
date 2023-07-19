---
title: Device Probe in Xorg
date: 2023-07-05 11:20:00
tags: [xserver]
categories: graphics
---

# 古老的 Xserver

![20-year-old Xserver code](20-years-old-patch.png)

# 多显卡系统的显示输出

要想弄清楚 Xserver 在多显卡 Linux 系统是如何选择显示输出设备的，按目前 Xserver 的实现，我们先要了解

- Platform Bus Devices
- [libpciaccess](https://gitlab.freedesktop.org/xorg/lib/libpciaccess)
- udev (主要与 hotplug 有关)

<!--more-->

本文想尝试回答的问题是，当主板上同时存在多张PCI显卡，但只有一个显示器(假设显示器至少有两个HDMI接口)时，Linux系统是如何选择优先使用哪一张显卡输出的。


上述场景又可细分为下面3种情况(OS: Kylin V10, Kernel: 4.4, Window System: Xorg 1.20.4)：

| 场景           | 显示                                              |
|:---------------|:--------------------------------------------------|
| A卡 + A卡      | 显示器可任意选择输入源, 默认PCI slotX 显示        |
| A卡 + N卡      | 显示器可任意选择输入源, 默认PCI slotX             |
| N卡 + N卡      | 显示器可任意选择输入源, 默认PCI slotX             |

# [Linux VGAArbiter](https://www.kernel.org/doc/html/v4.10/gpu/vgaarbiter.html)

Linux kernel vgaarbiter 的创建

```
commit deb2d2ecd43dfc51efe71eed7128fda514da96c6
Author: Benjamin Herrenschmidt <benh@kernel.crashing.org>
Date:   Tue Aug 11 15:52:06 2009 +1000

    PCI/GPU: implement VGA arbitration on Linux

    Background:
    Graphic devices are accessed through ranges in I/O or memory space. While most
    modern devices allow relocation of such ranges, some "Legacy" VGA devices
    implemented on PCI will typically have the same "hard-decoded" addresses as
    they did on ISA. For more details see "PCI Bus Binding to IEEE Std 1275-1994
    Standard for Boot (Initialization Configuration) Firmware Revision 2.1"
    Section 7, Legacy Devices.

    The Resource Access Control (RAC) module inside the X server currently does
    the task of arbitration when more than one legacy device co-exists on the same
    machine. But the problem happens when these devices are trying to be accessed
    by different userspace clients (e.g. two server in parallel). Their address
    assignments conflict. Therefore an arbitration scheme _outside_ of the X
    server is needed to control the sharing of these resources. This document
    introduces the operation of the VGA arbiter implemented for Linux kernel.

    Signed-off-by: Benjamin Herrenschmidt <benh@kernel.crashing.org>
    Signed-off-by: Tiago Vignatti <tiago.vignatti@nokia.com>
    Signed-off-by: Dave Airlie <airlied@redhat.com>
    Signed-off-by: Jesse Barnes <jbarnes@virtuousgeek.org>

 drivers/gpu/Makefile     |    2 +-
 drivers/gpu/vga/Kconfig  |   10 +
 drivers/gpu/vga/Makefile |    1 +
 drivers/gpu/vga/vgaarb.c | 1206 ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 drivers/pci/pci.c        |   44 ++++
 drivers/video/Kconfig    |    2 +
 include/linux/pci.h      |    2 +
 include/linux/vgaarb.h   |  195 +++++++++++++++++
 8 files changed, 1461 insertions(+), 1 deletion(-)
```

# X Server的输出设备检测和驱动加载
## <a name="xf86_platform_device"></a>Xorg抽象的输出设备
Linux下的显示输出设备一般要么是一个`pci_device`，要么是一个`xf86_platform_device`
Xorg定义了一个全局数组`xf86_platform_devices`, 这个数组的元素类型是`xf86_platform_device`, 存储是动态申请的，Xorg探测到一个PCI显卡时，就申请一个存储。 

```
struct xf86_platform_device *xf86_platform_devices;

struct xf86_platform_device {
		struct OdevAttributes *attribs;
		struct pci_device *pdev;
		int flags;
};

struct OdevAttributes {
		char *path;
		char *syspath;
		char *busid;
		int fd;
		int major;
		int minor;
		char *driver;
}
```

`OdevAttributes`定义了一个PCI输出设备的属性。

- path

    kernel device node, `/dev/dri/card0`

- syspath

    system device path, `/sys/devices/pci0000:00/0000:00:01.0/0000:01:00.0/drm/card1`

- busid

    DRI 风格的Bus ID， 如 "pci:0000:04:00.0"

- fd

    文件描述符，`open(/dev/dri/card0)`返回值

- major

    主设备号

- minor

    次设备号

- driver

    kernel driver name, 如 "amdgpu"

# PCI BusID

## DRI-style 

```
pci:0000:04:00.0
```

- `0000` PCI domain
- `  04` PCI bus
- `  00` PCI device
- `   0` PCI function

## Xorg 日志

- 来自 xorg-server-1.18.4 版本的 Xorg 日志

```
[     8.456] (--) PCI: (0:5:0:0) 1002:677b:174b:3000 rev 0, Mem @ 0x1040000000/268435456, 0x58600000/131072, I/O @ 0x00002000/256, BIOS @ 0x????????/131072
```

- 来自 xorg-server-1.20.0 版本的 Xorg 日志

```
[    40.891] (--) PCI: (10@0:0:0) 1a03:2000:1a03:2000 rev 65, Mem @ 0x60000000/16777216, 0x61000000/131072, I/O @ 0x00002000/128, BIOS @ 0x????????/65536
```

变更发生于 2017-06-19:

```
commit e905b19a53f96013c4417bec993a1dea5a3b0a5f
Author: Michel Dänzer <michel.daenzer@amd.com>
Date:   Mon Jun 19 19:05:29 2017 +0900

    xfree86: Print BusID stanza compatible bus IDs for found devices

    The PCI domain has to be specified like this:

     "PCI:<bus>@<domain>:<device>:<function>"

    Example before:

     (--) PCI:*(0:0:1:0) 1002:130f:1043:85cb [...]
     (--) PCI: (0:1:0:0) 1002:6939:1458:229d [...]

    after:

     (--) PCI:*(0@0:1:0) 1002:130f:1043:85cb [...]
     (--) PCI: (1@0:0:0) 1002:6939:1458:229d [...]

    Reviewed-by: Alex Deucher <alexander.deucher@amd.com>
    Signed-off-by: Michel Dänzer <michel.daenzer@amd.com>
```

## Xorg 配置文件 "Device" Section

上面两种格式都可以作为`Device`段里`BusID`的格式，X11规定的`BusID`的格式是

"bus<font color="green">[@domain]</font>:device<font color="green">[:func]</font>"


Xserver依赖下面的用户空间库来填写该结构，这个过程也体现了Xserver检测显卡及加载驱动的过程

- [libpciaccess](https://gitlab.freedesktop.org/xorg/lib/libpciaccess)
- [libudev](https://github.com/systemd/systemd/tree/main/src/libudev)
- [libdrm](https://gitlab.freedesktop.org/mesa/drm)

1. 通过`libpciaccess`的接口发现系统PCI设备，获取`syspath`, `path`
2. 通过`open`系统调用打开侦测到的`drm_device`，`fd`有了
3. 通过`libdrm`接口获取`major`, `minor`和`driver`


# Device Detect Routines
Xserver提供了两种设备检测方法：
```
void xf86PlatformDeviceProbe(struct OdevAttributes *attribs);
void xf86PciProbe(void);
```

默认是使用`xf86PlatformDeviceProbe`， 当这个函数完成`xf86_add_platform_device`后，除了`attribs`, `xf86_platform_device`的其它成员还没有被填写，剩下的任务交由`libudev`, `libpciaccess`和`libdrm`的接口完成。

## libudev
- udev_enumerate_add_match_subsys
- udev_enumerate_add_match_sysname

## libpciaccess
- pci_device_probe
- pci_device_is_boot_vga

## libdrm
- drmSetInterfaceVersion
- drmGetBusid
- drmGetVersion

# Primary Bus
在多卡的情况下，Xserver启动后默认使用哪个显卡显示？

```
typedef struct _bus {
	BusType type;
	union {
		struct pci_device *pci;
		SbusBusId sbus;
		struct xf86_platform_device *plat;
	} id;
} BusRec, *BusPtr;

```

Xserver定义了一个全局的`BusRec`类型变量`primaryBus`

```
BusRec primaryBus = { BUS_NONE, {0} };
```

这个`primaryBus`将是Xserver启动后默认使用的显卡设备的唯一候选者。

# Who Is The Lucky Boy?
Xserver有两个规则去确定[primaryBus](#primary-bus):

- [config_udev_odev_probe](#libudev)
- [pci_device_is_boot_vga](#libpciaccess)

## How `config_udev_odev_probe` Works?
`config_udev_odev_probe`唯一的参数是一个callback函数`xf86PlatformDeviceProbe`, `config_udev_odev_probe`要做的就是调用[libudev](#libudev)的接口枚举出`/dev`文件系统里注册的`drm`设备，将它的`path`填到[xf86_platform_device.attribs->path](#xf86_platform_device)。所以这一步决定了[xf86_platform_devices](#xf86_platform_devices)数组中platform device的顺序，按照此顺序最后那个`pci_device_is_boot_vga`返回`True`的显示设备将为`primaryBus`, 但这是在没有配置`PrimaryGPU`选项时的行为。下面的代码来自`libudev`


```
_public_ int sd_device_enumerator_add_match_sysname(sd_device_enumerator *enumerator, const char *sysname) {
        int r;

        assert_return(enumerator, -EINVAL);
        assert_return(sysname, -EINVAL);

        r = set_put_strdup(&enumerator->match_sysname, sysname);
        if (r <= 0)
                return r;

        enumerator->scan_uptodate = false;

        return 1;
}
```

这里参数`sysname`是一个正则表达式`card[0-9]*`

## How `pci_device_is_boot_vga` Works?
`pci_device_is_boot_vga`是一个虚接口，在Linux下的实现是`pci_device_linux_sysfs_boot_vga`, 下面的代码来自`libpciaccess`

```
static int pci_device_linux_sysfs_boot_vga(struct pci_device *dev)
{
    char name[256];
    char reply[3];
    int fd, bytes_read;
    int ret = 0;

    snprintf( name, 255, "%s/%04x:%02x:%02x.%1u/boot_vga",
	      SYS_BUS_PCI,
	      dev->domain,
	      dev->bus,
	      dev->dev,
	      dev->func );

    fd = open( name, O_RDONLY | O_CLOEXEC);
    if (fd == -1)
       return 0;

    bytes_read = read(fd, reply, 1);
    if (bytes_read != 1)
	goto out;
    if (reply[0] == '1')
	ret = 1;
out:
    close(fd);
    return ret;
}
```

这里`SYS_BUS_PCI`被定义为`/sys/bus/pci/devices`, `pci_device_is_boot_vga`的返回值取决于显示设备的**kernel driver**如何实现`sysfs`文件系统中的`/sys/bus/pci/devices/0000:05:00.0/boot_vga`节点。


以上两点规则说明，在多卡系统中，不使用`PrimaryGPU`的情况下，当且仅当目标卡的`drm`设备节点号最大，而且`/sys/bus/pci/devices/XXXX:XX:XX.X/boot_vga`被实现为read它返回ASCII字符`'1'`. 这样目标卡才能做为`primaryBus`设备默认显示输出。

