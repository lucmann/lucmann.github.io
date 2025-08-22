---
title: Resizable PCI BAR
date: 2025-03-12 15:00:41
tags: [Hardware]
categories: hardware
---

![PCIe Type 0 (Non-Bridge) Configuration Space Header](/images/pci/Pci-config-space.svg)

<!--more-->

# PCI config space

A set of registers

## Access to config space

内核访问 PCI config space 的 API 都定义在 `drivers/pci/access.c` 中

```
int pci_read_config_byte(const struct pci_dev *dev, int where, u8 *val);
```
