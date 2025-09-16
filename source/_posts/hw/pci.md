---
title: Resizable PCI BAR
date: 2025-03-12 15:00:41
tags: [Hardware]
categories: hardware
---


![pci-configuration-space-memory-map](/images/pci/pci-configuration-space-memory-map.png)
<!--more-->

# PCI Config Space (<span style="color: red;">256 Bytes per Device</span>)

PCI config space 本质上是 64 个 32-bit 寄存器

![Type 0 / 1 PCI Configuration Space Header](/images/pci/pci-configuration-space-headers.png)
## PCI 3.0 Type 0 Configuration Space Header

**Type 0 Configuration Space Header** 用于最终的实际功能设备，也就是 **Endpoint Device**

## PCI 3.0 Type 1 Configuration Space Header

**Type 1 Configuration Space Header** 用于连接两条总线的设备，也就是 **Bridge Device**

```
pci 0000:00:00.0: config space:
00000000: 86 80 34 3e
00000004: 06 00 90 20
00000008: 0c 00 00 06
0000000c: 00 00 00 00 
00000010: 00 00 00 00 <- BAR0
00000014: 00 00 00 00 <- BAR1
00000018: 00 00 00 00 <- BAR2
0000001c: 00 00 00 00 <- BAR3
00000020: 00 00 00 00 <- BAR4
00000024: 00 00 00 00 <- BAR5
00000028: 00 00 00 00
0000002c: 28 10 20 09
00000030: 00 00 00 00
00000034: e0 00 00 00
00000038: 00 00 00 00
0000003c: 00 00 00 00
00000040: 01 90 d1 fe
00000044: 00 00 00 00
00000048: 01 00 d1 fe
0000004c: 00 00 00 00
00000050: c1 02 00 00
00000054: b1 80 00 00
00000058: 47 00 70 8f
0000005c: 07 01 00 8a
00000060: 01 00 00 e0
00000064: 00 00 00 00
00000068: 01 80 d1 fe
0000006c: 00 00 00 00
00000070: 00 00 00 fc
00000074: 01 00 00 00
00000078: 00 0c 00 fc
0000007c: 7f 00 00 00
00000080: 01 00 00 00
00000084: 00 00 00 00
00000088: 1a 00 00 00
0000008c: 00 00 00 00
00000090: 01 00 00 fc
00000094: 01 00 00 00
00000098: 01 00 70 6c
0000009c: 02 00 00 00
000000a0: 01 00 00 00
000000a4: 02 00 00 00
000000a8: 01 00 80 6c
000000ac: 02 00 00 00
000000b0: 01 00 80 8b
000000b4: 01 00 00 8b
000000b8: 01 00 00 8a
000000bc: 01 00 80 8f
000000c0: 00 00 00 00
000000c4: 00 00 00 00
000000c8: 00 00 00 00
000000cc: 00 00 00 00
000000d0: 00 00 00 00
000000d4: 00 00 00 00
000000d8: 00 00 00 00
000000dc: 00 00 00 00
000000e0: 09 00 10 01
000000e4: c1 60 61 7a
000000e8: 8c 80 11 94
000000ec: 00 c0 04 00
000000f0: 00 00 00 00
000000f4: c8 0f 0c 00
000000f8: 00 00 00 00
000000fc: 00 00 00 00
pci 0000:00:00.0: [8086:3e34] type 00 class 0x060000 conventional PCI endpoint
pci 0000:00:02.0: config space:
00000000: 86 80 a0 3e
00000004: 07 00 10 00
00000008: 02 00 00 03
0000000c: 00 00 00 00
00000010: 04 00 00 c0 <- BAR 0 * (type=mem)
00000014: 00 00 00 00 <- BAR 1
00000018: 0c 00 00 b0 <- BAR 2 * (type=mem)
0000001c: 00 00 00 00 <- BAR 3
00000020: 01 40 00 00 <- BAR 4 * (type=io)
00000024: 00 00 00 00 <- BAR 5
00000028: 00 00 00 00
0000002c: 28 10 20 09
00000030: 00 00 00 00
00000034: 40 00 00 00 <- Capability Pointer : 40h
00000038: 00 00 00 00
0000003c: ff 01 00 00
00000040: 09 70 0c 01 <- Capability Next : 70h, ID: 09h (Vendor Specific)
00000044: c1 60 61 7a
00000048: 8c 80 11 94
0000004c: 00 00 00 00
00000050: c1 02 00 00
00000054: b1 80 00 00
00000058: 00 00 00 00
0000005c: 01 00 80 8b
00000060: 00 00 01 00
00000064: 00 00 00 00
00000068: 00 00 00 00
0000006c: 00 00 00 00
00000070: 10 ac 92 00 <- Capability Next : ach, ID: 10h (PCI Express)
00000074: 00 80 00 10
00000078: 00 00 00 00
0000007c: 00 00 00 00
00000080: 00 00 00 00
00000084: 00 00 00 00
00000088: 00 00 00 00
0000008c: 00 00 00 00
00000090: 00 00 00 00
00000094: 00 00 00 00
00000098: 00 00 00 00
0000009c: 00 00 00 00
000000a0: 00 00 00 00
000000a4: 00 00 00 00
000000a8: 00 00 00 00
000000ac: 05 d0 00 00 <- Capability Next : d0h, ID: 05h (Message Signaled Interrupts)
000000b0: 00 00 00 00
000000b4: 00 00 00 00
000000b8: 00 00 00 00
000000bc: 00 00 00 00
000000c0: 00 00 00 00
000000c4: 00 00 00 00
000000c8: 00 00 00 00
000000cc: 00 00 00 00
000000d0: 01 00 22 00 <- Capability Last : 00h, ID: 01h (PCI Power Management Interface)
000000d4: 00 00 00 00
000000d8: 00 00 00 00
000000dc: 00 00 00 00
000000e0: 00 00 00 00
000000e4: 00 00 00 00
000000e8: 00 80 00 00
000000ec: 00 00 00 00
000000f0: 00 00 00 00
000000f4: 00 00 00 00
000000f8: 00 00 00 00
000000fc: 18 00 e1 88
pci 0000:00:02.0: [8086:3ea0] type 00 class 0x030000 PCIe Root Complex Integrated Endpoint
pci 0000:00:02.0: BAR 0 [mem 0xc0000000-0xc0ffffff 64bit] <- FB (mmapped Write-Combining)
pci 0000:00:02.0: BAR 2 [mem 0xb0000000-0xbfffffff 64bit pref] <- (mapped Write-Through)
pci 0000:00:02.0: BAR 4 [io  0x4000-0x403f]
pci 0000:00:02.0: Video device with shadowed ROM at [mem 0x000c0000-0x000dffff]
...
pci 0000:01:00.0: config space:
00000000: de 10 13 1d
00000004: 00 00 10 00
00000008: a1 00 02 03
0000000c: 00 00 00 00
00000010: 00 00 00 c1 <- BAR 0 * (type=mem)
00000014: 0c 00 00 90 <- BAR 1 * (type=mem)
00000018: 00 00 00 00 <- BAR 2
0000001c: 0c 00 00 a0 <- BAR 3 * (type=mem)
00000020: 00 00 00 00 <- BAR 4
00000024: 01 30 00 00 <- BAR 5 * (type=io)
00000028: 00 00 00 00
0000002c: 28 10 20 09
00000030: 00 00 f8 ff
00000034: 60 00 00 00 <- Capability Pointer : 60h
00000038: 00 00 00 00
0000003c: ff 01 00 00
00000040: 28 10 20 09
00000044: 00 00 00 00
00000048: 00 00 00 00
0000004c: 00 00 00 00
00000050: 00 00 00 00
00000054: 01 00 00 00
00000058: ce d6 23 00
0000005c: 00 00 00 00
00000060: 01 68 03 00 <- Capability Next : 68h, ID: 01h (PCI Power Management Interface)
00000064: 08 00 00 00
00000068: 05 78 80 00 <- Capability Next : 78h, ID: 05h (Message Signaled Interrupts)
0000006c: 00 00 00 00
00000070: 00 00 00 00
00000074: 00 00 00 00
00000078: 10 00 02 00 <- Capability Last : 00h, ID: 10h (PCI Express)
0000007c: e1 8d e8 07
00000080: 30 29 09 00
00000084: 43 4c 45 00
00000088: 42 01 43 10
0000008c: 00 00 00 00
00000090: 00 00 00 00
00000094: 00 00 00 00
00000098: 00 00 00 00
0000009c: 13 08 04 00
000000a0: 00 00 00 00
000000a4: 0e 00 00 00
000000a8: 03 00 1e 00
000000ac: 00 00 00 00
000000b0: 00 00 00 00
000000b4: 09 00 14 01
000000b8: 00 00 00 00
000000bc: 00 00 00 00
000000c0: 00 00 00 00
000000c4: 00 00 00 00
000000c8: 00 00 00 00
000000cc: 00 00 00 00
000000d0: 00 00 00 00
000000d4: 00 00 00 00
000000d8: 00 00 00 00
000000dc: 00 00 00 00
000000e0: 00 00 00 00
000000e4: 00 00 00 00
000000e8: 00 00 00 00
000000ec: 00 00 00 00
000000f0: 00 00 00 00
000000f4: 00 00 00 00
000000f8: 00 00 00 00
000000fc: 00 00 00 00
pci 0000:01:00.0: [10de:1d13] type 00 class 0x030200 PCIe Endpoint
pci 0000:01:00.0: BAR 0 [mem 0xc1000000-0xc1ffffff]
pci 0000:01:00.0: BAR 1 [mem 0x90000000-0x9fffffff 64bit pref]
pci 0000:01:00.0: BAR 3 [mem 0xa0000000-0xa1ffffff 64bit pref]
pci 0000:01:00.0: BAR 5 [io  0x3000-0x307f]
pci 0000:01:00.0: ROM [mem 0xfff80000-0xffffffff pref]
```
上面的 PCI config space dump 可以通过内核启动参数 <span style="background-color: yellow; padding: 4px;">pci=earlydump</span> 开启打印。所谓 **earlydump** 是指打印的是内核**未经修改的** config space, 也就是说是 UEFI 初始化过的 config space, 像 BAR 里的物理地址一般不会是这个设备最终的物理内存起始地址，内核一般都会**重新规划** PCI 设备的物理内存地址空间，由内核重新填入的 BAR 值，可以通过

```bash
lspci -xxx -s <Bus:Dev.Func>
```

或

```bash
xxd /sys/bus/pci/devices/<Domain:Bus:Dev.Func>/config
```

来查看。

# PCI Extended Config Space (<span style="color: red;">0x100 ~ 0xFFF</span>)

PCI 的 Capability 有 Base 和 Extended 之分，Base Capability List 在 PCI Config Space, 而 Extended Capability List 在 Extended Config Space，即在整个配置空间 0x100 的位置。

查看方法同上

```bash
lspci -xxxx -s <Bus:Dev.Func>
```

或

```bash
xxd /sys/bus/pci/devices/<Domain:Bus:Dev.Func>/config
```

以上都是纯数据形式打印，如果想以 Human-readable 形式看到，请使用


```bash
lspci -vv -s <Domain:Bus:Dev.Func>
```

# 消失的南桥和北桥

![](/images/pci/modern-computer.png)

# 参考

- [osdev.org: PCI](https://wiki.osdev.org/PCI)
- [PCI Code and ID Assignment Specification,Rev. 1.11](https://pcisig.com/sites/default/files/files/PCI_Code-ID_r_1_11__v24_Jan_2019.pdf)
