---
title: lspci 学习笔记
date: 2021-01-25 20:42:05
tags: [tools]
categories: utilities
---

# 与 PCI 设备相关的一些 IDs

- Device ID: 4 位十六进制数字
- Vendor ID: 4 位十六进制数字
- Domain ID: 4 位十六进制数字

```
3772:00:00.0 0100: 1af4:1049 (rev 01)
5401:00:00.0 0880: 1af4:105a (rev 01)
b30b:00:00.0 0302: 1414:008e
c4c5:00:00.0 0100: 1af4:1049 (rev 01)
cdbf:00:00.0 0100: 1af4:1049 (rev 01)
```

`lspci -Dn` 的输出格式是：

```
<domain>:<bus>:<slot>.<func>
```

这里的 `slot` 其实就是 `device`, 而且只包含 2 位十六进制数字。我们可以从 `lspci` 的[源码](https://github.com/pciutils/pciutils/blob/master/lspci.c)看到

```c
static void
show_slot_path(struct device *d)
{
  struct pci_dev *p = d->dev;

  if (opt_path)
    {
      struct bus *bus = d->parent_bus;
      struct bridge *br = bus->parent_bridge;

      if (br && br->br_dev)
	{
	  show_slot_path(br->br_dev);
	  if (opt_path > 1)
	    printf("/%02x:%02x.%d", p->bus, p->dev, p->func);
	  else
	    printf("/%02x.%d", p->dev, p->func);
	  return;
	}
    }
  printf("%02x:%02x.%d", p->bus, p->dev, p->func);
}
```

lspci 之所以只使用 2 位十六进制数字分别表示 `bus`, `slot`(`dev`), 是因为它们被定义成 8 位位宽

```c
struct pci_dev {
  struct pci_dev *next;			/* Next device in the chain */
  u16 domain_16;			/* 16-bit version of the PCI domain for backward compatibility */
					/* 0xffff if the real domain doesn't fit in 16 bits */
  u8 bus, dev, func;			/* Bus inside domain, device and function */

  /* These fields are set by pci_fill_info() */
  unsigned int known_fields;		/* Set of info fields already known (see pci_fill_info()) */
  u16 vendor_id, device_id;		/* Identity of the device */
  u16 device_class;			/* PCI device class */
  int irq;				/* IRQ number */
  pciaddr_t base_addr[6];		/* Base addresses including flags in lower bits */
  pciaddr_t size[6];			/* Region sizes */
  pciaddr_t rom_base_addr;		/* Expansion ROM base address */
  pciaddr_t rom_size;			/* Expansion ROM size */
  struct pci_cap *first_cap;		/* List of capabilities */
  char *phy_slot;			/* Physical slot */
  char *module_alias;			/* Linux kernel module alias */
  char *label;				/* Device name as exported by BIOS */
  int numa_node;			/* NUMA node */
  pciaddr_t flags[6];			/* PCI_IORESOURCE_* flags for regions */
  pciaddr_t rom_flags;			/* PCI_IORESOURCE_* flags for expansion ROM */
  int domain;				/* PCI domain (host bridge) */
  pciaddr_t bridge_base_addr[4];	/* Bridge base addresses (without flags) */
  pciaddr_t bridge_size[4];		/* Bridge sizes */
  pciaddr_t bridge_flags[4];		/* PCI_IORESOURCE_* flags for bridge addresses */
  u8 prog_if, rev_id;			/* Programming interface for device_class and revision id */
  u16 subsys_vendor_id, subsys_id;	/* Subsystem vendor id and subsystem id */
  struct pci_dev *parent;		/* Parent device, does not have to be always accessible */
  int no_config_access;			/* No access to config space for this device */

  /* Fields used internally */
  struct pci_access *access;
  struct pci_methods *methods;
  u8 *cache;				/* Cached config registers */
  int cache_len;
  int hdrtype;				/* Cached low 7 bits of header type, -1 if unknown */
  void *aux;				/* Auxiliary data for use by the back-end */
  struct pci_property *properties;	/* A linked list of extra properties */
  struct pci_cap *last_cap;		/* Last capability in the list */
};
```

# Xorg config 之 `Device` Section

```
Section "Device"
Identifier "name"
Driver "driver"
BusID "bus-id"
EndSection
```

`Device` Section 中 `BusID` 的格式是

```
PCI:bus:device:func
```

# Xorg log 中的 PCI 信息格式

```c
        xf86Msg(X_PROBED, "PCI:%s(%u@%u:%u:%u) %04x:%04x:%04x:%04x ", prim,
                info->bus, info->domain, info->dev, info->func,
                info->vendor_id, info->device_id,
                info->subvendor_id, info->subdevice_id);
```

日志中的 PCI BusID 格式是

```
bus@domain:device:func
```
