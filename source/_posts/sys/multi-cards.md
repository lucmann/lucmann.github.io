---
title: Multi-VideoCards in Xorg
date: 2021-05-14 11:20:00
tags: xorg
categories: sys
---

# Global Platform Devices Array

<!--more-->

## <a name="xf86_platform_device"></a>A xf86 platform device

```
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

## <a name="xf86_platform_devices"></a>Array of xf86 platform devices 

```
struct xf86_platform_device *xf86_platform_devices;

```

这个数组是全局的。

- path

字符串，kernel device node, `/dev/dri/card0`

- syspath

字符串，system device path, `/sys/devices/pci0000:00/0000:00:01.0/0000:01:00.0/drm/card1`

- busid

字符串，DRI格式的Bus ID， `pci:0000:04:00.0`, 这种格式包含4个数字，分别表示：

1. PCI domain
2. PCI bus
3. PCI device
4. PCI function

要注意这种格式与`Xorg.0.log`中的那种打印格式的区别

``` xorg-server-1.18.4
[     8.456] (--) PCI: (0:5:0:0) 1002:677b:174b:3000 rev 0, Mem @ 0x1040000000/268435456, 0x58600000/131072, I/O @ 0x00002000/256, BIOS @ 0x????????/131072
```

这种格式`PCI: (0:5:0:0) 1002:677b:174b:3000`, 一共包含了8个数字，分别表示：
1. PCI domain
2. PCI bus
3. PCI device
4. PCI function
5. PCI vendor ID
6. PCI device ID
7. PCI subvendor ID
8. PCI subdevice ID

``` xorg-server-1.20.0
[    40.891] (--) PCI: (10@0:0:0) 1a03:2000:1a03:2000 rev 65, Mem @ 0x60000000/16777216, 0x61000000/131072, I/O @ 0x00002000/128, BIOS @ 0x????????/65536
```

这种格式`PCI: (10@0:0:0) 1a03:2000:1a03:2000`, 也是共8个数字，与前面的区别是PCI domain与PCI bus换了位置。

- fd

文件描述符，`open(/dev/dri/card0)`返回值

- major

主设备号

- minor

次设备号

- driver

字符串， kernel driver name, `drm_driver.name`


Xserver依赖下面的用户空间库来填写该结构，这个过程也体现了Xserver加载输出设备(显卡)及驱动的过程

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

