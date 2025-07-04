---
title: SiPEED LicheePi4A
date: 2024-10-28 12:07:56
tags: [Hardware]
categories: hardware
---

![LicheePi4A close-up](/images/licheepi4a/lp4a-closeup.jpg)
<!--more-->
![LicheePi4A specification](/images/licheepi4a/lp4a-spec.png)
![LicheePi4A bring-up](/images/licheepi4a/lp4a-working.jpg)
![LicheePi4A desktop (debian)](/images/licheepi4a/lp4a-desktop.jpg)

# Upstream Drivers
- [drm/imagination: Enable PowerVR driver for RISC-V (WIP)](https://patchwork.kernel.org/project/dri-devel/patch/20241203134137.2114847-12-m.wilczynski@samsung.com/)
- [drm/verisilicon : support DC8200 and inno hdmi (WIP)](https://lore.kernel.org/all/20241120061848.196754-1-keith.zhao@starfivetech.com/)
  (平头哥 TH1520 和 赛昉 JH7110 都集成了显示控制器 IP - DC8200 (好像来自芯原))

# Boot from SD-Card

dd 写入 [img](https://fast-mirror.isrc.ac.cn/revyos/extra/images/lpi4a/20250110/) 到 sd-card 之后 lsblk

```
NAME        MAJ:MIN RM   SIZE RO TYPE MOUNTPOINTS
sda           8:0    1 119.1G  0 disk 
├─sda1        8:1    1     2M  0 part 
├─sda2        8:2    1   500M  0 part /media/luc/44c74757-aab2-4d25-9ed8-c29e027c3f3f
├─sda3        8:3    1     4G  0 part 
└─sda4        8:4    1     5G  0 part /media/luc/f929e2c1-6c06-419a-9c7b-28f5ded4e665
```

# References

[Installing RevyOS](https://docs.revyos.dev/en/docs/Installation/licheepi4a/)