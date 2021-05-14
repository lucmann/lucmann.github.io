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
```


