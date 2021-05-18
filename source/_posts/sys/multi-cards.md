---
title: Multi-VideoCards in Xorg
date: 2021-05-14 11:20:00
tags: xorg
categories: sys
---

# Global Platform Devices Array
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

`OdevAttributes`可以理解为**Output Device Attributes**, 这个结构体的每个成员都值得说一下。
- path

	字符串，kernel device node, `/dev/dri/card0`

- syspath

	字符串，system device path, `/sys/devices/pci0000:00/0000:00:01.0/0000:01:00.0/drm/card1`

- busid

	字符串，DRI格式的Bus ID， `PCI:1:0:0` (我猜的)

- fd

	文件描述符，`open(/dev/dri/card0)`返回值

- major

	主设备号

- minor

	次设备号

- driver

	字符串， kernel driver name, `drm_driver.name`


Xserver依赖两个接口来填写该结构，这个过程也体现了Xserver加载输出设备(显卡)及驱动的过程

- libpciaccess
- libdrm

1. 通过`libpciaccess`的接口发现系统PCI设备，获取`syspath`, `path`
2. 通过`open`系统调用打开侦测到的`drm_device`，`fd`有了
3. 通过`libdrm`接口获取`major`, `minor`和`driver`

