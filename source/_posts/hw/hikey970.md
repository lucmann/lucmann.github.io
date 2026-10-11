---
title: HiKey970
date: 2023-06-14 15:00:41
tags: [Hardware]
categories: hardware
---

<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<title>HiKey 970 vs LicheePi 4A 详细规格对比</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body {
font-family: "PingFang SC", "Microsoft YaHei", "Segoe UI", sans-serif;
margin: 40px auto;
max-width: 1140px;
padding: 0 15px;
color: #333;
line-height: 1.45;
}
* {
box-sizing: border-box;
}
h1 {
font-size: 24px;
text-align: center;
}
.subtitle {
text-align: center;
color: #888;
font-size: 14px;
margin-bottom: 24px;
}
a {
color: #2c5f9e;
text-decoration: none;
}
a:hover {
text-decoration: underline;
}
.comparison {
overflow-x: auto;
/* 窄屏横向滚动 */
}
table {
width: 100%;
border-collapse: collapse;
}
th, td {
border: 1px solid #ddd;
padding: 10px 14px;
text-align: left;
vertical-align: top;
font-size: 14px;
}
th {
background: #f5f7fa;
font-weight: bold;
width: 130px;
}
td.winner {
background: #fff8e6;
}
.comparison tr:first-child td {
text-align: center;
border: none;
}
.comparison tr:first-child th {
border: none;
background: none;
}
.comparison img {
border-radius: 4px;
max-width: 120px;
height: auto;
}
.comparison .boardname {
display: block;
margin-top: 4px;
font-size: 1.05em;
}
.comparison .close {
text-align: right;
}
.comparison .close a {
color: #999;
font-weight: bold;
}
.note {
font-size: 0.85em;
color: #888;
}
footer.foot {
margin-top: 24px;
font-size: 13px;
color: #666;
}
</style>
</head>
<body>
<h1>HiKey 970 vs LicheePi 4A 详细规格对比</h1>
<p class="subtitle">黄色底色单元格 = 该项参数更优（数据来源见页脚）</p>
<main>
<div class="comparison">
<table>
<tr>
<th></th>

<td>
<div class="close">
<a href="https://www.96boards.org/product/hikey970/" title="查看 HiKey 970 官方资料">
<strong>&times;</strong>
</a>
</div>
<a href="https://www.96boards.org/product/hikey970/" class="image">
<img src="data:image/jpeg;base64,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" alt="HiKey 970">
<br>
<strong class="boardname">
HiKey 970
</strong>
</a>
</td>

<td>
<div class="close">
<a href="https://wiki.sipeed.com/hardware/zh/lichee/th1520/lp4a.html" title="查看 LicheePi 4A 官方 Wiki">
<strong>&times;</strong>
</a>
</div>
<a href="https://wiki.sipeed.com/hardware/zh/lichee/th1520/lp4a.html" class="image">
<img src="data:image/jpeg;base64,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" alt="LicheePi 4A">
<br>
<strong class="boardname">
LicheePi 4A
</strong>
</a>
</td>
</tr>
<!-- start of parameters -->

<tr>
<th>Manufacturer<br>厂商</th>
<td class="">
HiSilicon（华为海思）<br><span class="note">96Boards 合作项目</span>
</td>
<td class="">
Sipeed（矽速科技）
</td>
</tr>

<tr>
<th>Released<br>上市时间</th>
<td class="">
2018
</td>
<td class="">
2023
</td>
</tr>

<tr>
<th>Price<br>参考价格</th>
<td class="">
$ 299.99<br><span class="note">上市预售价，现已停产</span>
</td>
<td class="winner">
$ 119 起（8 GB 版）<br><span class="note">16 GB + 128 GB eMMC 版 $ 179</span>
</td>
</tr>

<tr>
<th>SoC<br>主控芯片</th>
<td class="">
HiSilicon Kirin 970（10 nm）
</td>
<td class="">
T-Head TH1520（12 nm，曳影 1520）
</td>
</tr>

<tr>
<th>CPU<br>处理器</th>
<td class="">
4&times; ARM Cortex-A73 @ 2.36 GHz<br>
4&times; ARM Cortex-A53 @ 1.8 GHz
</td>
<td class="">
4&times; XuanTie C910 @ 2.0 GHz<br>
<span class="note">RISC-V RV64GCV；每核 64 KB I/D 缓存，共享 1 MB L2</span>
</td>
</tr>

<tr>
<th>GPU<br>图形处理器</th>
<td class="">
ARM Mali-G72 MP12
</td>
<td class="">
Imagination BXM-4-64（50.7 GFLOPS）<br>
<span class="note">OpenGL ES 3.2 / Vulkan 1.2 / OpenCL 2.0</span>
</td>
</tr>

<tr>
<th>NPU<br>AI 加速</th>
<td class="">
专用 NPU（HiAI 架构）
</td>
<td class="winner">
4 TOPS @ INT8（主频 1 GHz）<br>
<span class="note">支持 TensorFlow / ONNX / Caffe</span>
</td>
</tr>

<tr>
<th>RAM<br>内存</th>
<td class="">
6.0 GiB LPDDR4X @ 1866 MHz
</td>
<td class="winner">
8 GB / 16 GB LPDDR4X-3733
</td>
</tr>

<tr>
<th>eMMC / UFS<br>板载存储</th>
<td class="">
64 GB UFS 2.1 &times; soldered（板载）
</td>
<td class="">
可选 eMMC：空贴 / 8 GB / 32 GB / 128 GB
</td>
</tr>

<tr>
<th>SD<br>存储卡</th>
<td class="">
1 &times; microSD 卡槽（可启动）
</td>
<td class="">
1 &times; TF（microSD）卡槽
</td>
</tr>

<tr>
<th>USB</th>
<td class="">
2 &times; USB 3.0 Type-A host<br>
2 &times; USB Type-C（1 &times; OTG + 1 &times; host）
</td>
<td class="winner">
4 &times; USB 3.0 Type-A host<br>
1 &times; USB 2.0 Type-C（仅用于烧录）
</td>
</tr>

<tr>
<th>Ethernet<br>以太网</th>
<td class="">
<div>1 &times; 1&nbsp;Gbps</div>
</td>
<td class="winner">
<div>2 &times; 1&nbsp;Gbps</div>
<div class="note">其一支持 PoE（可选 PoE 模块）</div>
</td>
</tr>

<tr>
<th>Wi-Fi</th>
<td class="">
板载双频 Wi-Fi（板载天线）<br>
2.4 GHz b/g/n<br>
5 GHz a/n/ac（TI WL1837）
</td>
<td class="">
选配 Wi-Fi 4 / Wi-Fi 6 模块<br>
<span class="note">基础版不板载</span>
</td>
</tr>

<tr>
<th>Bluetooth<br>蓝牙</th>
<td class="">
板载 Bluetooth 4.2
</td>
<td class="">
选配 Bluetooth 5.0（随 Wi-Fi 模块）
</td>
</tr>

<tr>
<th>Modem<br>蜂窝网络</th>
<td class="">
SoC 集成 Balong LTE 基带<br>
<span class="note">板卡未引出天线/卡槽</span>
</td>
<td class="">
-
</td>
</tr>

<tr>
<th>Video<br>视频编解码</th>
<td class="">
H.264 / H.265 解码<br>
HDMI 输出 1080p@60
</td>
<td class="winner">
解码最高 4K@75fps<br>
编码最高 4K@40fps（H.264 / H.265）
</td>
</tr>

<tr>
<th>Camera<br>摄像头</th>
<td class="">
1 &times; 4-lane MIPI-CSI<br>
1 &times; 2-lane MIPI-CSI（60-pin HS 接口）
</td>
<td class="winner">
1 &times; 4-lane MIPI-CSI<br>
2 &times; 2-lane MIPI-CSI
</td>
</tr>

<tr>
<th>Display<br>显示接口</th>
<td class="">
1 &times; HDMI（1080p@60）<br>
4-lane MIPI-DSI（HS 接口）
</td>
<td class="winner">
1 &times; HDMI 2.0（最高 4K@60）<br>
1 &times; 4-lane MIPI-DSI（最高 4K@50）
</td>
</tr>

<tr>
<th>Audio<br>音频</th>
<td class="">
3.5 mm 耳机/麦克风<br>
HDMI 音频<br>
1 &times; I2S
</td>
<td class="winner">
3.5 mm 耳机接口<br>
扬声器接口 + 2 &times; 板载麦克风<br>
<span class="note">音频由专用 C906 核处理</span>
</td>
</tr>

<tr>
<th>GPIO</th>
<td class="">
12 &times; GPIO<br>
<span class="note">40-pin LS 排针引出</span>
</td>
<td class="">
20-pin GPIO 排针<br>
<span class="note">UART / I2C / SPI 引出</span>
</td>
</tr>

<tr>
<th>PWM</th>
<td class="">
-
</td>
<td class="">
-
</td>
</tr>

<tr>
<th>ADC</th>
<td class="">
-
</td>
<td class="">
-
</td>
</tr>

<tr>
<th>I2C</th>
<td class="">
2 &times;
</td>
<td class="">
20-pin 排针引出（IIC）
</td>
</tr>

<tr>
<th>SPI</th>
<td class="">
1 &times;
</td>
<td class="">
20-pin 排针引出
</td>
</tr>

<tr>
<th>UART</th>
<td class="">
2 &times;
</td>
<td class="">
20-pin 排针引出
</td>
</tr>

<tr>
<th>CAN</th>
<td class="">
1 &times;
</td>
<td class="">
-
</td>
</tr>

<tr>
<th>PCIe / M.2<br>扩展插槽</th>
<td class="">
1 &times; mPCIe 插槽<br>
1 &times; M.2 Key M 插槽
</td>
<td class="">
-
</td>
</tr>

<tr>
<th>Pin header<br>排针接口</th>
<td class="">
40-pin LS + 60-pin HS<br>
<span class="note">96Boards 标准接口</span>
</td>
<td class="">
20-pin GPIO 排针<br>
<span class="note">自定义引脚，非树莓派兼容</span>
</td>
</tr>

<tr>
<th>Form factor<br>板型</th>
<td class="">
96Boards CE（Consumer Edition）
</td>
<td class="">
自定义迷你板型<br>
<span class="note">LM4A 核心板 + 底板结构</span>
</td>
</tr>

<tr>
<th>Dimensions<br>尺寸</th>
<td class="">
105.26 mm x 100 mm
</td>
<td class="winner">
84.45 mm x 52.07 mm
</td>
</tr>

<tr>
<th>Power<br>供电</th>
<td class="">
8 V – 18 V DC 圆口<br>
<span class="note">96Boards 标准供电</span>
</td>
<td class="">
5 V（5.5 mm DC 圆口或 USB Type-C）<br>
<span class="note">可选 PoE 模块供电</span>
</td>
</tr>

<tr>
<th>Temperature<br>工作温度</th>
<td class="">
0.0&deg;C - 70.0&deg;C
</td>
<td class="">
未标注
</td>
</tr>

<tr>
<th>Software<br>软件支持</th>
<td class="winner">
Linux: True（Debian / AOSP）<br>
Android: 8.1
</td>
<td class="">
Linux: True<br>
<span class="note">RevyOS（Debian）/ openEuler / OpenWrt / OpenHarmony</span><br>
Android: 适配中
</td>
</tr>

<!-- end of parameters -->
</table>
</div>
</main>

<footer class="foot">
<p>
数据来源：<a href="https://www.96boards.org/product/hikey970/">96Boards — HiKey 970 官方产品页</a>、
<a href="https://hackerboards.com/board/hisilicon-hikey-970">HackerBoards Board-DB</a>、
<a href="https://www.cnx-software.com/2018/04/16/hikey-970-development-board-now-up-for-pre-order-for-299-99/">CNX Software</a>、
<a href="https://wiki.sipeed.com/hardware/zh/lichee/th1520/lp4a.html">Sipeed Wiki — LicheePi 4A</a>（含官方规格书/尺寸图）。
价格与规格可能随版本变动，以官方最新信息为准。
</p>
</footer>
</body>
</html>

# 1. 环境

## WSL2 作为上位机

### 识别 Windows 11 Host 的 USB Serial Device

分别在 Windows 11 上安装 [usbipd-win](https://github.com/dorssel/usbipd-win)， 在 WSL2 上安装 [user space tools for USB/IP](https://github.com/dorssel/usbipd-win/wiki/WSL-support) 

```
Microsoft Windows [版本 10.0.22621.1702]
(c) Microsoft Corporation。保留所有权利。

C:\Windows\System32>usbipd wsl list
BUSID  VID:PID    DEVICE                                                        STATE
1-9    046d:c534  USB 输入设备                                                  Not attached
1-13   04e2:1410  USB 串行设备 (COM4)                                           Not attached
```

### WSL2 Ubuntu-20.04 创建 /dev/ttyUSB0

```
[Thu Jun 15 19:19:44 2023] vhci_hcd vhci_hcd.0: pdev(0) rhport(0) sockfd(3)
[Thu Jun 15 19:19:44 2023] vhci_hcd vhci_hcd.0: devid(65546) speed(2) speed_str(full-speed)
[Thu Jun 15 19:19:44 2023] vhci_hcd vhci_hcd.0: Device attached
[Thu Jun 15 19:19:45 2023] vhci_hcd: vhci_device speed not set
[Thu Jun 15 19:19:45 2023] usb 1-1: new full-speed USB device number 2 using vhci_hcd
[Thu Jun 15 19:19:45 2023] vhci_hcd: vhci_device speed not set
[Thu Jun 15 19:19:45 2023] usb 1-1: SetAddress Request (2) to port 0
[Thu Jun 15 19:19:45 2023] usb 1-1: New USB device found, idVendor=04e2, idProduct=1410, bcdDevice= 0.03
[Thu Jun 15 19:19:45 2023] usb 1-1: New USB device strings: Mfr=0, Product=0, SerialNumber=0
[Thu Jun 15 19:19:45 2023] xr_serial 1-1:1.0: xr_serial converter detected
[Thu Jun 15 19:19:45 2023] usb 1-1: xr_serial converter now attached to ttyUSB0
```

### Recovery 模式应该连接哪个 Type-C （USB SERIAL) 接口

HiKey970 有两个 Type-C 接口，而且当板子被设置为 Recovery 模式时，两个接口均会被识别为“串口”。在左手边的 (J3101) 是用来访问 Debug UART 的，而在 HDMI 和 USB 中间的那个(J1801)是在 Recovery 模式下使用的。而且这两个接口是两个不同厂家提供的芯片，使用完全不同的内核驱动模块

![USB-to-Serial on Hikey970](/images/hikey970/USB-Serial.png)

- 前者(J3101)：
    `Bus 001 Device 003: ID 04e2:1410 Exar Corp. XR21V1410 USB-UART IC`

![Select CONFIG_USB_SERIAL_XR on WSL2 Kernal config](/images/hikey970/exar-USB-SER-driver.png)

- 后者(J1801)：
    `Bus 001 Device 002: ID 12d1:3609 Huawei Technologies Co., Ltd. USB SER`

![Select CONFIG_USB_SERIAL_OPTION on WSL2 Kernel config](/images/hikey970/huawei-USB-SER-driver.png)

# 2. 启动

## eMMC 启动

### Arm Firmware

| Bootloader Level | File | Description |
|:-----------------|:-----|:------------|
| BL1 | l-loader.bin      |             |
| BL2 | included in fip.bin |           |
| BL31| included in fip.bin |           |
| BL32| included in fip.bin | Optional  |
| BL33| included in fip.bin | UEFI Boot Manager/U-Boot          |

#### Firmware Image Package (fip.bin)

`fiptool` 

``` fiptool info fip.bin
Trusted Boot Firmware BL2: offset=0xD8, size=0x4248, cmdline="--tb-fw"
SCP Firmware SCP_BL2: offset=0x4320, size=0x36064, cmdline="--scp-fw"
EL3 Runtime Firmware BL31: offset=0x3A384, size=0x8010, cmdline="--soc-fw"
Non-Trusted Firmware BL33: offset=0x42394, size=0xF0000, cmdline="--nt-fw"
```

### Build rootfs.img and boot.img

#### boot.img

`boot.img` 主要提供 bootloader, 所以它可以只包含 **grub.efi**, Hikey970 使用的 boot.img 是 64M 大小

#### rootfs.img

`rootfs.img` 就是整个系统了(根文件系统)，内核可执行文件(Image)和设备树二进制文件(.dtb) 都包含在它的 boot 目录里，Hikey970 使用的 rootfs.img 原始大小是 4.0GB, 但经过 android-tools 工具包里的 `img2simg` 处理后只有 716M

```Sparse image 与 Raw image 大小对比
-rw-r--r-- 1 luc luc 4.0G 11月26日 06:48 rootfs.img
-rw-r--r-- 1 luc luc 716M 11月18日 21:47 rootfs.sparse.img
```

#### debootstrap

- `/usr/sbin/qemu-debootstrap`
- `/usr/sbin/debootstrap`

是两个 **Shell 脚本**, 主要就是通过下载相应平台的 binaries，通过 **chroot** 来制作根文件系统

### fastboot

fastboot 是用来从 Host 向开发板烧写固件和镜像的常用工具之一，在 Arch Linux 上它可以通过以下命令安装

```install fastboot
yay -S android-sdk-platform-tools
```

#### fastboot 常用命令

```从设备的 bootloader 获取各种信息，如 version, partition
fastboot getvar all
```

```列出已连接设备的 ID, 可以用来判断设备是否已与 Host 正常连接就绪
fastboot devices
```

```在指定分区刷写指定镜像
fastboot flash ptable 64gtoendprm_ptable.img
fastboot flash xloader sec_xloader.img
fastboot flash fastboot l-loader.bin
fastboot flash fip fip.bin
fastboot flash boot boot2grub.uefi.img
```

```对于比较大的 Sparse 镜像，指定传输块大小，每 8M 传输
fastboot -S 8M flash system rootfs.sparse.img
```

### Bootloader

```sudo mount -o loop boot2grub.uefi.img /mnt
➜  /mnt ls -lh /mnt/EFI/BOOT
总计 881K
-rwxr-xr-x 1 root root  29K 2018年 2月14日 fastboot.efi
-rwxr-xr-x 1 root root 852K 2018年 2月14日 grubaa64.efi
```

```启动后的 /boot 目录
hikey970% ls -lhR /boot
/boot:
total 23M
drwxr-xr-x 3 root root 4.0K Jun 12  2018 EFI
-rw-r--r-- 1 root root  23M Jun 12  2018 Image
drwxr-xr-x 2 root root 4.0K Jun 12  2018 grub
-rw-r--r-- 1 root root  68K Jun 12  2018 kirin970-hikey970.dtb

/boot/EFI:
total 4.0K
drwxr-xr-x 2 root root 4.0K Jun 12  2018 BOOT

/boot/EFI/BOOT:
total 32K
-rw-r--r-- 1 root root 29K Jun 12  2018 fastboot.efi

/boot/grub:
total 4.0K
-rw-r--r-- 1 root root 462 Jun 12  2018 grub.cfg
```

![Hikey970 boot ubuntu bionic](/images/hikey970/hikey970-boot.png)

> **NOTE:**
> HiKey970 的输入电压要求在 8V ~ 18V 之间，但最好使用 **12V 以上接近 18V**的输入电压，否则可能出现 `fastboot flash` 时出现板子自己重启的怪现象
> ![Hikey970 Input Power Voltage](/images/hikey970/hikey970-power.jpg)

#### 连接 WiFi

![Hikey970 wifi](/images/hikey970/wifi-connect.png)

![Hikey970 WiFi LED indicator](/images/hikey970/hikey970-wifi-led.jpg)

#### xfce4 桌面

吃灰5,6年的板子又再一次亮了

![Hikey970 xfce4](/images/hikey970/hikey970-xfce4.jpg)

## SD 卡启动

### 制作 rootfs, initramfs

环境是 qemu-system-aarch64 Debian 13 Trixie, 折腾了一圈，最后还是发现 Ubuntu, Debian, Arch Linux 这3个，还是 Debian 对 Aarch64 支持最好，Ubuntu 甚至还一个桌面版的 Arm 安装镜像都没有(Arm架构的安装镜像似乎都是服务器版的)。

![Debian vs. Ubuntu](/images/hikey970/debian-vs-ubuntu.png)

#### NetworkManager vs wpasupplicant

点亮吹灰 Hikey970 用的是 [hikey970-ubuntu-image](https://github.com/mengzhuo/hikey970-ubuntu-image), 它的 rootfs 里安装的是 NetworkManager, 当我试着将 hikey970-ubuntu-image 转换成 hikey970-debian-image 时，发现 debootstrap 会因为奇怪的包依赖问题，无法安装 NetworkManager， 而且了解到 wpasupplicant 可以完成同样的事情(连接 WiFi,让板子联网), 而且体量更小，更适合这种开发板。

所以这个 Debian 12 rootfs 网络这块使用了 **wpasupplicant**, **iw**, **iproute2**, **dhcpcd5** 四剑客

- wpasupplicant

```rootfs/etc/systemd/system/wpa_supplicant@.service
[Unit]
Description=WPA supplicant daemon (interface %i)
After=sys-subsystem-net-devices-%i.device
Wants=sys-subsystem-net-devices-%i.device

[Service]
Type=simple
ExecStart=/sbin/wpa_supplicant -c /etc/wpa_supplicant/wpa_supplicant.conf -i %i
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

Note:

1. 当 `systemctl enable wpa_supplicant@wlan0.service` 时 systemd 会自动创建一个符号链接文件到 `/etc/systemd/system/wpa_supplicant@.service`, 而文件里的 `%i` 是无线网络设备接口名，即 **wlan0**.
2. 手动创建 `/etc/wpa_supplicant/wpa_supplicant.conf`
    ```
    wpa_passphrase <SSID> <PASSWORD> >/etc/wpa_supplicant/wpa_supplicant.conf
    ```

- iw

```/sbin/iw
iw dev wlan0 scan | grep 'SSID:'
iw dev wlan0 connect <SSID>
```
- iproute2

```/bin/ip
ip link show wlan0
ip addr wlan0
```

- dhcpcd5

```
systemctl enable dhcpcd.service
systemctl start dhcpcd.service
```

- ntpdate

```
/sbin/ntpdate ntp.aliyun.com
```


##### SD 卡不识别

现象：系统启动后 `lsblk` 看不到 `/dev/mmcblk0`

使用原来的 [hikey970-ubuntu-image (@mengzhuo)](https://github.com/mengzhuo/hikey970-ubuntu-image) 镜像，SD 卡是正常识别的，这就说明电源和卡本身没有问题(之前因为使用 12V 的电源, fastboot flash 总是失败的教训很深刻🐶)。

将下来主要排查设备树和内核配置的问题，在这里和 ChatGPT/DeepSeek 交流了很多，总体感觉 ChatGPT 在这方面比 DeepSeek 靠谱一点。了解到了 HI3670 SoC 的 MMC 控制器使用的是 Synopsis DesignWare MMC, 它是一种不遵循 SD Host Controller Interface 规范的厂家自定义接口。

##### SD 卡启动

SD 卡启动有两个主要问题:

- [准备好 sdcard.img](https://github.com/lucmann/hikey970-debian-image/blob/hikey970-debian-image/hikey970_dual_boot_builder.sh)
- 让 UEFI 能够识别 sdcard 上的 boot 分区

![Hikey970 BootManager](/images/hikey970/BootManager.png)
![Hikey970 BootManager](/images/hikey970/DevicesList.png)
![Hikey970 BootManager](/images/hikey970/BootFromFile.png)
![Hikey970 BootManager](/images/hikey970/boot_sdcard_0x30a7d232_0x3f_0x7ffc1.png)
![Hikey970 BootManager](/images/hikey970/grub.png)

# 3. 驱动

## DC 驱动

### `kirin-drm` Display Pipeline Engine driver

![From claude.ai](/images/hikey970/hikey970-display.svg)
![](/images/hikey970/atomic_modeset.png)

```mermaid
flowchart TB
    A["kirin_drm_platform_probe()<br/>@platform_driver.probe"]
    subgraph "kirin_drm_bind()@component_master_ops.bind"
        B["drm_dev_alloc()"]
        subgraph "kirin_drm_kms_init(drm_dev)"
            C1["dev_set_drvdata(drm_dev->dev_private)"]
            C2["drm_mode_config_init(drm_dev)"]
            C3["kirin_drm_mode_config_init(drm_dev)<br/>kirin-specific mode config"]
            C31["dss_drm_init()@kirin_dc_ops.init"]
            C4["component_bind_all(dev, drm_dev)"]
            C5["drm_vblank_init(drm_dev, num_crtc(2))"]
            C6["drm_mode_config_reset(drm_dev)"]
            C7["kirin_drm_fbdev_init(drm_dev)"]
            C8["drm_kms_helper_poll_init(drm_dev)"]
            C9["drm_helper_hpd_irq_event()"]
        end
        D["drm_dev_register(drm_dev)"]
        E["kirin_drm_connectors_register(drm_dev)"]
    end

    N31["display controller init"]
    N6["reset all the states of crtc/plane/encoder/connector"]
    N8["init kms poll for handling hotplug"]
    N9["force detection after connectors init"]

    C31 -.-> N31
    C6 -.-> N6
    C8 -.-> N8
    C9 -.-> N9
    A --> B --> C1 --> C2 --> C3 --> C31 --> C4 --> C5 --> C6 --> C7 --> C8 --> C9 --> D --> E
```

### `dw-dsi` DesignWare MIPI DSI Host Controller Driver

```mermaid
flowchart TB
    subgraph "dsi_probe()@platform_driver.probe"
        direction TB
        A["devm_kzalloc()"]
        subgraph "dsi_parse_endpoint(dw_dsi, np, OUT_HDMI)"
            subgraph "for_each_endpoint_of_node()"
                subgraph "dsi_parse_bridge_endpoint(dsi, ep_node)"
                    B1["of_graph_get_remote_port_parent()"]
                    B2["of_drm_find_bridge()"]
                end
            end
        end
        C["dsi_host_init(dev, dw_dsi)"]
        D["dsi_parse_endpoint(dw_dsi, np, OUT_PANEL)"]
        E["dsi_parse_dt(plat_dev, dw_dsi)"]
        F["platform_set_drvdata()"]
        G["component_add(dev, &dsi_ops)"]
    end 
    N2["if bridge return NULL,<br/>then return -EPROBE_DEFER"]

    A --> B1 --> B2 --> C --> D --> E --> F --> G
    B2 -.-> N2
```

#### [DSI bridge probe](https://lore.kernel.org/dri-devel/e5ec9763-37fe-6cd8-6eca-52792afbdb94@samsung.com/T/)

DSI bridge driver probe [陷入死循环](https://gist.github.com/lucmann/7ae4bf26d49807a4d951db2e989a271c)

```
[    3.270639] [drm] the bridge node is adv7533
[    3.270645] [drm] wait for external HDMI bridge driver.
[    3.270832] of_irq_parse_raw:  /soc/gpio@e8a0c000:00000001,00000002
[    3.270905] adv7511_probe dev->of_node (____ptrval____)
[    3.270909] __devm_drm_bridge_alloc
[    3.271426] adv7511_init_regulators
[    3.330257] [drm] the bridge node is adv7533
[    3.330264] [drm] wait for external HDMI bridge driver.
[    3.330449] of_irq_parse_raw:  /soc/gpio@e8a0c000:00000001,00000002
[    3.330522] adv7511_probe dev->of_node (____ptrval____)
[    3.330526] __devm_drm_bridge_alloc
[    3.331015] adv7511_init_regulators
[    3.390209] [drm] the bridge node is adv7533
[    3.390215] [drm] wait for external HDMI bridge driver.
[    3.390397] of_irq_parse_raw:  /soc/gpio@e8a0c000:00000001,00000002
[    3.390464] adv7511_probe dev->of_node (____ptrval____)
[    3.390468] __devm_drm_bridge_alloc
[    3.390937] adv7511_init_regulators
[    3.446194] [drm] the bridge node is adv7533
[    3.446200] [drm] wait for external HDMI bridge driver.
```

按照上游对 hisilicon/kirin/dw_drm_dsi.c 的[修改](https://lists.freedesktop.org/archives/freedreno/2021-October/012892.html)，将 hikey9xx/gpu/kirin9xx_dw_drm_dsi.c 中的 `dsi_probe()` 在 probe ordering 调整后可解决此问题

![From claude.ai](/images/hikey970/dsi_probe_ordering_comparison.svg)

### `kirin9xx-dw-dsi` MIPI DSI Host Controller Driver

```mermaid
flowchart TB
    subgraph "dsi_probe()@platform_driver.probe"
        direction TB
        A["devm_kzalloc()"]
        subgraph "dsi_parse_endpoint(dw_dsi, np, OUT_HDMI)"
            subgraph "dsi_parse_bridge_endpoint(dsi, ep_node)"
                B["of_graph_get_remote_port_parent(ep_node)"]
                B_["of_drm_find_bridge(bridge_node)"]
            end
        end
        subgraph "dsi_host_init(dev, dw_dsi)"
            B1["mipi_dsi_host_register(dw_dsi::mipi_dsi_host::ops)"]
        end
        C["dsi_parse_endpoint(dw_dsi, np, OUT_PANEL)"]
        D["dsi_parse_dt(plat_dev, dw_dsi)"]
        E["platform_set_drvdata()"]
        subgraph "dsi_host_init(dev, dw_dsi)"
            F1["mipi_dsi_host_register(dw_dsi::mipi_dsi_host::ops)"]
        end
        subgraph "dsi_host_attach()@mipi_dsi_host_ops.attach"
            G1["component_add(dev, &dsi_ops)"]
        end
        subgraph "dsi_bind()@component_ops.bind"
            subgraph "dw_drm_encoder_init(dev, drm_dev, &dsi->encoder)"
                direction TB
                H11["drm_of_find_possible_crtcs(drm_dev, dev->of_node)"]
                H12["drm_encoder_init(drm_dev, encoder, &dw_encoder_funcs,<br/>DRM_MODE_ENCODER_DSI, NULL)"]
                H13["drm_encoder_helper_add(encoder, &dw_encoder_helper_funcs)"]
                H14["drm_of_find_panel_or_bridge(np, 1, 0, NULL, &bridge)"]
                H15["drm_bridge_attach(encoder, bridge, NULL, 0)"]
            end
        end

        style B  fill: #f5f5f5, stroke-dasharray: 5,5
        style B_ fill: #f5f5f5, stroke-dasharray: 5,5
        style B1 fill: #f5f5f5, stroke-dasharray: 5,5
    end
    A --> B --> B_ --> B1 --> C --> D --> E --> F1 --> G1 --> H11 --> H12 --> H13 --> H14 --> H15
    B1 --> F1
    B_ --> H14
```

### `adv7511` ADV7535 Bridge Driver

```c
static void adv7511_power_on(struct adv7511 *adv7511)
```

`adv7511_power_on()` 只在 3 个地方调用了:

- `static void adv7511_hpd_work(struct work_struct *work)` 它被放入一个 workqueue, 由单独的 kthread 异步执行
- `static enum drm_connector_status adv7511_detect(struct adv7511 *adv7511)` adv7511 driver probe 时并不会执行，由回调函数 `adv7511_bridge_funcs.detect()` 调用
- `static void adv7511_bridge_atomic_enable(struct drm_bridge *bridge, struct drm_atomic_state *state)` 由回调函数 `adv7511_bridge_funcs.atomic_enable()` 调用

目前 **kirin9xx-dw-dsi** driver 的问题是 **`adv7511_bridge_funcs.atomic_enable()` 没有被调用**


```
[    2.363067] [drm] Initialized kirin9xx 1.0.0 for e8600000.dpe on minor 0
[    2.363085] kirin9xx-drm e8600000.dpe: [drm:drm_client_modeset_probe] 
[    2.363091] [drm:drm_mode_object_get] OBJ ID: 37 (2)
[    2.363097] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] [CONNECTOR:37:HDMI-A-1]
[    2.567054] [drm] Valid mode: 640x480@60, clock 25175 (adjusted to 25175)
[    2.567079] [drm] Valid mode: 800x600@56, clock 36000 (adjusted to 36000)
[    2.567100] [drm] Valid mode: 800x600@60, clock 40000 (adjusted to 40000)
[    2.567119] [drm] Valid mode: 848x480@60, clock 33750 (adjusted to 33750)
[    2.567137] [drm] Valid mode: 1024x768@60, clock 65000 (adjusted to 65000)
[    2.567144] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] [CONNECTOR:37:HDMI-A-1] probed modes:
[    2.567151] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "1024x768": 60 65000 1024 1048 1184 1344 768 771 777 806 0x40 0xa
[    2.567158] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "800x600": 60 40000 800 840 968 1056 600 601 605 628 0x40 0x5
[    2.567165] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "800x600": 56 36000 800 824 896 1024 600 601 603 625 0x40 0x5
[    2.567171] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "848x480": 60 33750 848 864 976 1088 480 486 494 517 0x40 0x5
[    2.567177] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "640x480": 60 25175 640 656 752 800 480 490 492 525 0x40 0xa
[    2.567183] kirin9xx-drm e8600000.dpe: [drm:drm_client_modeset_probe] [CONNECTOR:37:HDMI-A-1] enabled? yes
[    2.567192] kirin9xx-drm e8600000.dpe: [drm:drm_client_modeset_probe] Not using firmware configuration
[    2.567200] kirin9xx-drm e8600000.dpe: [drm:drm_client_modeset_probe] [CONNECTOR:37:HDMI-A-1] found first mode: 1024x768
[    2.567206] kirin9xx-drm e8600000.dpe: [drm:drm_client_modeset_probe] picking CRTCs for 2048x2048 config
[    2.567214] kirin9xx-drm e8600000.dpe: [drm:drm_client_modeset_probe] [CRTC:35:crtc-0] desired mode 1024x768 set (0,0)
[    2.567219] [drm:drm_mode_object_get] OBJ ID: 37 (2)
[    2.567223] [drm:drm_mode_object_put.part.0] OBJ ID: 37 (3)
[    2.567231] kirin9xx-drm e8600000.dpe: [drm:__drm_fb_helper_initial_config_and_unlock] test CRTC 0 primary plane
[    2.567240] kirin9xx-drm e8600000.dpe: [drm:drm_fbdev_dma_driver_fbdev_probe] surface width(1024), height(768) and bpp(32)
[    2.574254] kirin9xx-drm e8600000.dpe: [drm:drm_mode_addfb2] [FB:39]
[    2.574260] [drm:drm_mode_object_put.part.0] OBJ ID: 39 (2)
[    2.574337] kirin9xx-drm e8600000.dpe: [drm] fb0: kirin9xxdrmfb frame buffer device
[    2.574341] [drm] Initialized kirin9xx 1.0.0 on minor 0
[    2.574344] [drm] host attach, client name=[adv7533], id=0
[    2.574347] adv7511 0-0039: adv7511_probe attach dsi ok
[    2.574350] adv7511 0-0039: adv7511_probe ok
```

```
[    2.204280] [drm] luc Initialized kirin9xx 1.0.0 on minor 0
[    2.204471] [drm] dss irq = 51.
[    2.204475] kirin9xx-drm e8600000.dpe: Looking up ldo3-supply from device node /dpe@e8600000
[    2.204576] [drm] dsi_bind
[    2.204578] kirin9xx-dw-dsi e8601000.dsi: dw_drm_encoder_init:
[    2.204586] kirin9xx-dw-dsi e8601000.dsi: Initializing CRTC encoder: 1
[    2.224171] kirin9xx-drm e8600000.dpe: bound e8601000.dsi (ops dsi_ops)
[    2.267712] dwmmc_k3 ff37f000.dwmmc1: tuning ok best_clksmpl 11 tuning_sample_flag bff7fff7
[    2.267729] mmc0: new UHS-I speed SDR104 SDHC card at address aaaa
[    2.268024] mmcblk0: mmc0:aaaa SC16G 14.8 GiB
[    2.282367]  mmcblk0: p1 p2
[    2.446942] kirin9xx-drm e8600000.dpe: [drm:check_connector_changed] [CONNECTOR:37:HDMI-A-1] status updated from unknown to connected
[    2.446962] kirin9xx-drm e8600000.dpe: [drm:check_connector_changed] [CONNECTOR:37:HDMI-A-1] Changed epoch counter 0 => 1
[    2.446969] [drm:drm_mode_object_get] OBJ ID: 37 (2)
[    2.446978] kirin9xx-drm e8600000.dpe: [drm:drm_sysfs_connector_hotplug_event] [CONNECTOR:37:HDMI-A-1] generating connector hotplug event
[    2.446994] [drm:drm_mode_object_put.part.0] OBJ ID: 37 (2)
[    2.446999] [drm] kirin_drm_kms_init ok
[    2.447023] [drm:drm_minor_register] 
[    2.447107] [drm:drm_minor_register] new minor registered 128
[    2.447111] [drm:drm_minor_register] 
[    2.447155] [drm:drm_minor_register] new minor registered 0
[    2.447159] [drm:drm_minor_register] 
[    2.447182] kirin9xx-drm e8600000.dpe: [drm:drm_sysfs_connector_add] [CONNECTOR:37:HDMI-A-1] adding connector to sysfs
[    2.447246] kirin9xx-drm e8600000.dpe: [drm:drm_sysfs_connector_hotplug_event] [CONNECTOR:37:HDMI-A-1] generating connector hotplug event
[    2.458988] [drm] Initialized kirin9xx 1.0.0 for e8600000.dpe on minor 0
[    2.459005] kirin9xx-drm e8600000.dpe: [drm:drm_client_modeset_probe] 
[    2.459013] [drm:drm_mode_object_get] OBJ ID: 37 (2)
[    2.459018] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] [CONNECTOR:37:HDMI-A-1]
[    2.577598] kirin9xx-drm e8600000.dpe: [drm:update_display_info.part.0] [CONNECTOR:37:HDMI-A-1] CEA VCDB 0xd5
[    2.577609] kirin9xx-drm e8600000.dpe: [drm:update_display_info.part.0] [CONNECTOR:37:HDMI-A-1] HDMI: DVI dual 0, max TMDS clock 360000 kHz
[    2.577617] kirin9xx-drm e8600000.dpe: [drm:update_display_info.part.0] [CONNECTOR:37:HDMI-A-1] HDMI sink does deep color 30.
[    2.577622] kirin9xx-drm e8600000.dpe: [drm:update_display_info.part.0] [CONNECTOR:37:HDMI-A-1] Assigning HDMI sink color depth as 10 bpc.
[    2.577629] kirin9xx-drm e8600000.dpe: [drm:update_display_info.part.0] [CONNECTOR:37:HDMI-A-1] HDMI sink does YCRCB444 in deep color.
[    2.577633] kirin9xx-drm e8600000.dpe: [drm:update_display_info.part.0] [CONNECTOR:37:HDMI-A-1] HDMI sink should do DC_36, but does not!
[    2.577641] kirin9xx-drm e8600000.dpe: [drm:update_display_info.part.0] [CONNECTOR:37:HDMI-A-1] ELD monitor Sculptor
[    2.577646] kirin9xx-drm e8600000.dpe: [drm:update_display_info.part.0] [CONNECTOR:37:HDMI-A-1] HDMI: latency present 0 0, video latency 0 0, audio latency 0 0
[    2.577652] kirin9xx-drm e8600000.dpe: [drm:update_display_info.part.0] [CONNECTOR:37:HDMI-A-1] ELD size 32, SAD count 1
[    2.577792] [drm] Valid mode: 1920x1080@60, clock 148500 (adjusted to 144000)
[    2.577812] [drm] Valid mode: 1920x1080@50, clock 117000 (adjusted to 117000)
[    2.577830] [drm] Valid mode: 1152x864@60, clock 81579 (adjusted to 81579)
[    2.577833] [drm] Valid mode: 1280x720@60, clock 74250 (adjusted to 72000)
[    2.577852] [drm] Valid mode: 1280x1024@60, clock 108000 (adjusted to 108000)
[    2.577870] [drm] Valid mode: 1600x900@60, clock 108000 (adjusted to 108000)
[    2.577888] [drm] Valid mode: 800x600@60, clock 40000 (adjusted to 40000)
[    2.577906] [drm] Valid mode: 1024x768@60, clock 65000 (adjusted to 65000)
[    2.577910] [drm] Valid mode: 1920x1080@60, clock 148500 (adjusted to 144000)
[    2.577915] [drm] Valid mode: 1280x720@50, clock 74250 (adjusted to 72000)
[    2.577917] [drm] Valid mode: 1280x720@60, clock 74250 (adjusted to 72000)
[    2.577922] [drm] Valid mode: 1920x1080@50, clock 148500 (adjusted to 144000)
[    2.577925] [drm] Valid mode: 1280x720@60, clock 74176 (adjusted to 74176)
[    2.577929] [drm] Valid mode: 1920x1080@60, clock 148352 (adjusted to 148352)
[    2.577935] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] [CONNECTOR:37:HDMI-A-1] probed modes:
[    2.577942] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "1920x1080": 60 148500 1920 2008 2052 2200 1080 1082 1087 1125 0x48 0x5
[    2.577948] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "1920x1080": 60 148500 1920 2008 2052 2200 1080 1084 1089 1125 0x40 0x5
[    2.577955] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "1920x1080": 60 148352 1920 2008 2052 2200 1080 1084 1089 1125 0x40 0x5
[    2.577961] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "1920x1080": 50 148500 1920 2448 2492 2640 1080 1084 1089 1125 0x40 0x5
[    2.577967] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "1920x1080": 50 117000 1920 2008 2052 2080 1080 1084 1089 1125 0x40 0x5
[    2.577973] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "1600x900": 60 108000 1600 1624 1704 1800 900 901 904 1000 0x40 0x5
[    2.577979] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "1280x1024": 60 108000 1280 1328 1440 1688 1024 1025 1028 1066 0x40 0x5
[    2.577984] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "1152x864": 60 81579 1152 1216 1336 1520 864 865 868 895 0x0 0x6
[    2.577990] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "1280x720": 60 74250 1280 1390 1430 1650 720 725 730 750 0x40 0x5
[    2.577996] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "1280x720": 60 74250 1280 1390 1430 1650 720 725 730 750 0x40 0x5
[    2.578002] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "1280x720": 60 74176 1280 1390 1430 1650 720 725 730 750 0x40 0x5
[    2.578007] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "1280x720": 50 74250 1280 1720 1760 1980 720 725 730 750 0x40 0x5
[    2.578013] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "1024x768": 60 65000 1024 1048 1184 1344 768 771 777 806 0x40 0xa
[    2.578018] kirin9xx-drm e8600000.dpe: [drm:drm_helper_probe_single_connector_modes] Probed mode: "800x600": 60 40000 800 840 968 1056 600 601 605 628 0x40 0x5
[    2.578025] kirin9xx-drm e8600000.dpe: [drm:drm_client_modeset_probe] [CONNECTOR:37:HDMI-A-1] enabled? yes
[    2.578034] kirin9xx-drm e8600000.dpe: [drm:drm_client_modeset_probe] Not using firmware configuration
[    2.578041] kirin9xx-drm e8600000.dpe: [drm:drm_client_modeset_probe] [CONNECTOR:37:HDMI-A-1] found preferred mode: 1920x1080
[    2.578047] kirin9xx-drm e8600000.dpe: [drm:drm_client_modeset_probe] picking CRTCs for 2048x2048 config
[    2.578054] kirin9xx-drm e8600000.dpe: [drm:drm_client_modeset_probe] [CRTC:35:crtc-0] desired mode 1920x1080 set (0,0)
[    2.578059] [drm:drm_mode_object_get] OBJ ID: 37 (2)
[    2.578063] [drm:drm_mode_object_put.part.0] OBJ ID: 37 (3)
[    2.578070] kirin9xx-drm e8600000.dpe: [drm:__drm_fb_helper_initial_config_and_unlock] test CRTC 0 primary plane
[    2.578078] kirin9xx-drm e8600000.dpe: [drm:drm_fbdev_dma_driver_fbdev_probe] surface width(1920), height(1080) and bpp(32)
[    2.588560] kirin9xx-drm e8600000.dpe: [drm:drm_mode_addfb2] [FB:40]
[    2.588566] [drm:drm_mode_object_put.part.0] OBJ ID: 40 (2)
[    2.588646] kirin9xx-drm e8600000.dpe: [drm] fb0: kirin9xxdrmfb frame buffer device
[    2.588649] [drm] Initialized kirin9xx 1.0.0 on minor 0
[    2.588653] [drm] host attach, client name=[adv7533], id=0
```

#### fbdev vs drm_client

kernel command line 选项 `drm_client_lib.active=fbdev` 可以覆盖内核配置项 `CONFIG_DRM_CLIENT_DEFAULT` 

#### modetest -M kirin9xx -s 37@35:#0

<video width="100%" controls preload="none">
  <source src="/images/hikey970/modetest-M-kirin9xx.mp4" type="video/mp4">
  您的浏览器不支持 HTML5 视频播放。
</video>

#### fbtest

<video width="100%" controls preload="none">
  <source src="/images/hikey970/fbtest.mp4" type="video/mp4">
  您的浏览器不支持 HTML5 视频播放。
</video>

#### xfce4

<video width="100%" controls preload="none">
  <source src="/images/hikey970/xfce-glxgears.mp4" type="video/mp4">
  您的浏览器不支持 HTML5 视频播放。
</video>

#### Assisted-by: deepseek-v4-pro

花了50元的token 费用，终于算是把这块“古老”的Hikey970 变成了一个"MiniPC"了(不用借助上位机串口工具了)。AI 帮助解决了以下问题, 而且搭建了 [lucmann/linux](https://github.com/lucmann/linux/actions) 和 [lucmann/hikey970-debian-image](https://github.com/lucmann/hikey970-debian-image/actions) 的 GitHub CI, 分别用来生成 Image.gz/hi3670-hikey970.dtb 和 rootfs image.

- bootloader (grubaa64.efi) 不支持 ext4 rootfs
- 修改 grub.cfg 不生效
- 2.4G Wireless Mouse/Keyboard USB receiver reset loop (Logi M212/K240)

<video width="100%" controls preload="none">
  <source src="https://github.com/lucmann/lucmann.github.io/releases/download/videos/hikey970-back-crf22.mp4" type="video/mp4">
  您的浏览器不支持 HTML5 视频播放。
</video>

## GPU 驱动

### `panfrost` GPU Driver

Pathor(C) 和 Tyr(Rust) 都是为 Valhall 架构以上的 Mali GPU (即基于 Command Stream Frontend 的 GPU) 而写的驱动, HiKey 970 (HI3670 SoC) 搭载的是 Mali G72 MP12 (Bifrost)，所以只能使用 Panfrost 驱动。上面可以启动的内核是 v4.19, 当时的 GPU 驱动还是 lima.

HiKey 970 开发板对应的 devicetree 源文件 **hi3670-hikey970.dts**， 在 v4.19 时也合入了主线。

> commit 5510ee99c0deb0c0235acee6498a6745c8317df1
> Refs: v4.19-rc1-6-g5510ee99c0de
> Author:     Manivannan Sadhasivam <mani@kernel.org>
> AuthorDate: Fri Aug 10 23:23:39 2018 +0530
> Commit:     Wei Xu <xuwei5@hisilicon.com>
> CommitDate: Wed Sep 19 16:15:25 2018 +0100
> 
>     arm64: dts: Add devicetree support for HiKey970 board
> 
>     Add devicetree support for HiKey970 development board which
>     based on Hi3670 SoC and is also one of the 96Boards Consumer
>     Edition and AI platform.
> 
>     Only UART6 is enabled which is the default console required
>     by the 96Boards Consumer Edition Specification.
> 
>     This patch has been tested on HiKey970 Board.
> 
>     Signed-off-by: Manivannan Sadhasivam <manivannan.sadhasivam@linaro.org>
>     Signed-off-by: Wei Xu <xuwei5@hisilicon.com>
> ---
>  arch/arm64/boot/dts/hisilicon/hi3670-hikey970.dts | 35 +++++++++++++++++++++++++++++++++++
>  1 file changed, 35 insertions(+)

#### v6.19

到目前为止，使用 [@mengzhuo/hikey970-ubuntu-image](https://github.com/mengzhuo/hikey970-ubuntu-image) 可以正常启动 HiKey970，而且安装了 Xfce，所以我 fork 了这个仓库，将其更名为 [hikey970-debian-image](https://github.com/lucmann/hikey970-debian-image)，将基于 Ubuntu bionic (18.04 LTS) 的 rootfs.img 移植到基于 Debian bookworm (12) 的 rootfs.img，并成功启动。之后准备将这块 2018 年 3 月发布的板子作为学习和测试内核最新驱动的平台，所以现在就看看主线编译的设备树 (arch/arm64/boot/dts/hisilicon/hi3670-hikey970.dtb) 和内核 (arch/arm64/boot/Image.gz) 是否能正常启动。 

- kernel + dtb
![Try latest kernel on hikey970](/images/hikey970/try-latest-kernel-on-hikey970.png)
- kernel only
![Try latest kernel on hikey970](/images/hikey970/try-latest-kernel-on-hikey970-2.png)

##### `random: crng init done` took **70** minutes

```
[    5.201987] random: perl: uninitialized urandom read (4 bytes read)
[    5.202263] random: perl: uninitialized urandom read (4 bytes read)
[FAILED] Failed to start systemd-ra…rvice - Load/Save Random Seed.
See 'systemctl status systemd-random-seed.service' for details.
         Starting systemd-tmpfiles-…leanup of Temporary Directories...
[  OK  ] Finished systemd-tmpfiles-… Cleanup of Temporary Directories.
         Starting dpkg-db-backup.se…ly dpkg database backup service...
         Starting logrotate.service - Rotate log files...
[  OK  ] Finished dpkg-db-backup.se…aily dpkg database backup service.
[  OK  ] Finished logrotate.service - Rotate log files.
[ 4396.712657] random: crng init done
[ 4396.712674] random: 4 urandom warning(s) missed due to ratelimiting
```

crng init 花这么长时间的原因是系统 entropy sources 不足，内核一直在填充 entropy pool, 这种情况只发生在刷写系统后第一次启动，之后 entropy pool 应该固化在 UFS 存储里了，启动时间就正常了。但因为需要频繁刷写系统，所以还是得解决这个问题。尝试了 rng-tools5 和 haveged 后，发现 haveged 可以解决这个问题。理论上 rng-tools5/rng-tools 也可以借助 `/dev/hwrng` 解决这个问题，但不知为何 rngd 服务始终[不能正常工作](https://gist.githubusercontent.com/lucmann/bfabaac6a2c904877629dd3ce97229eb/raw/ceb01cbe992a09d8e1c20a944ea272c8145c7a67/rngd.log)，怀疑可能是 Hi3670 SoC 的 TRNG 驱动有问题。


# 4. 参考

- [HiKey970 User Guide](https://www.96boards.org/documentation/consumer/hikey/hikey970/hardware-docs/files/hikey970-user-manual.pdf)
- [HiKey970 Board Recovery](https://www.96boards.org/documentation/consumer/hikey/hikey970/installation/board-recovery.md.html)
- [HiKey970 GitHub](https://github.com/96boards/documentation/tree/master/consumer/hikey/hikey970)
- [Create a Bootable SD Card for HiKey](https://www.96boards.org/documentation/consumer/hikey/hikey620/installation/linux-sd-boot.md.html)
- [Linaro downloads](https://releases.linaro.org/96boards/reference-platform/debian/hikey/)
- [DRM driver for HiKey 970](https://lore.kernel.org/bpf/20200820100440.2d30dc02@coco.lan/T/)
- [usbipd-win github issues](https://github.com/dorssel/usbipd-win/issues/59)
- [跨平台更新制作 rootfs](https://notes.leconiot.com/debootstrap.html)
- [Racing karts on a Rust GPU kernel driver](https://www.collabora.com/news-and-blog/news-and-events/racing-karts-on-a-rust-gpu-kernel-driver.html)
- [制作 Hikey970 的 rootfs](https://github.com/lucmann/hikey970-ubuntu-image)
- [UEFI Shell Specification](https://uefi.org/sites/default/files/resources/UEFI_Shell_Spec_2_0_Errata_A.pdf)
- [When should ADV7511 bridge attach to DSI host](https://patchwork.kernel.org/project/dri-devel/patch/20190627151740.2277-1-matt.redfearn@thinci.com/)
- [ADV7511 Programming Guide](https://www.analog.com/media/en/technical-documentation/user-guides/ADV7511_Programming_Guide.pdf)
