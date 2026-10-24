---
title: HiKey 970 vs LicheePi 4A 详细规格对比
date: 2023-06-14 15:00:41
tags: [Hardware]
categories: hardware
---
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>HiKey 970 vs LicheePi 4A 详细规格对比</title>
<style>
  body {
    font-family: "PingFang SC", "Microsoft YaHei", "Segoe UI", sans-serif;
    margin: 40px auto;
    max-width: 1200px;
    color: #333;
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
    background: #2c5f9e;
    color: #fff;
  }
  td:first-child {
    font-weight: bold;
    background: #f5f7fa;
    width: 130px;
  }
  tr.diff td {
    background: #fff8e6;
  }
  tr.diff td:first-child {
    background: #fdeec9;
  }
  .na {
    color: #bbb;
  }
  .note {
    margin-top: 16px;
    font-size: 13px;
    color: #888;
  }
  .sources {
    margin-top: 24px;
    font-size: 13px;
    color: #666;
  }
  .sources a {
    color: #2c5f9e;
  }
</style>
</head>
<body>
<h1>HiKey 970 vs LicheePi 4A 详细规格对比</h1>
<p class="subtitle">黄色行为两款产品有差异的指标</p>
<table>
  <thead>
    <tr>
      <th>技术指标</th>
      <th>HiKey 970</th>
      <th>LicheePi 4A</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>厂商</td>
      <td>HiSilicon（华为海思）<br><span class="note">96Boards 合作项目</span></td>
      <td>Sipeed（矽速科技）</td>
    </tr>
    <tr>
      <td>上市时间</td>
      <td>2018</td>
      <td>2023</td>
    </tr>
    <tr class="diff">
      <td>参考价格</td>
      <td>$ 299.99<br><span class="note">上市预售价，现已停产</span></td>
      <td>$ 119 起（8 GB 版）<br><span class="note">16 GB + 128 GB eMMC 版 $ 179</span></td>
    </tr>
    <tr>
      <td>主控芯片</td>
      <td>HiSilicon Kirin 970（10 nm）</td>
      <td>T‑Head TH1520（12 nm，曳影 1520）</td>
    </tr>
    <tr>
      <td>处理器</td>
      <td>4× ARM Cortex‑A73 @ 2.36 GHz<br>4× ARM Cortex‑A53 @ 1.8 GHz</td>
      <td>4× XuanTie C910 @ 2.0 GHz<br><span class="note">RISC‑V RV64GCV；每核 64 KB I/D 缓存，共享 1 MB L2</span></td>
    </tr>
    <tr>
      <td>图形处理器</td>
      <td>ARM Mali‑G72 MP12</td>
      <td>Imagination BXM‑4‑64（50.7 GFLOPS）<br><span class="note">OpenGL ES 3.2 / Vulkan 1.2 / OpenCL 2.0</span></td>
    </tr>
    <tr class="diff">
      <td>AI加速(NPU)</td>
      <td>专用 NPU（HiAI 架构）</td>
      <td>4 TOPS @ INT8（主频 1 GHz）<br><span class="note">支持 TensorFlow / ONNX / Caffe</span></td>
    </tr>
    <tr class="diff">
      <td>内存(RAM)</td>
      <td>6.0 GiB LPDDR4X @ 1866 MHz</td>
      <td>8 GB / 16 GB LPDDR4X‑3733</td>
    </tr>
    <tr>
      <td>板载存储</td>
      <td>64 GB UFS 2.1 板载焊接</td>
      <td>可选 eMMC：空贴 / 8 GB / 32 GB / 128 GB</td>
    </tr>
    <tr>
      <td>存储卡</td>
      <td>1 × microSD 卡槽（可启动）</td>
      <td>1 × TF（microSD）卡槽</td>
    </tr>
    <tr class="diff">
      <td>USB接口</td>
      <td>2 × USB 3.0 Type‑A host<br>2 × USB Type‑C（1 × OTG + 1 × host）</td>
      <td>4 × USB 3.0 Type‑A host<br>1 × USB 2.0 Type‑C（仅用于烧录）</td>
    </tr>
    <tr class="diff">
      <td>以太网</td>
      <td>1 × 1 Gbps</td>
      <td>2 × 1 Gbps<br><span class="note">其一支持 PoE（可选 PoE 模块）</span></td>
    </tr>
    <tr>
      <td>Wi‑Fi</td>
      <td>板载双频 Wi‑Fi（板载天线）<br>2.4 GHz b/g/n<br>5 GHz a/n/ac（TI WL1837）</td>
      <td>选配 Wi‑Fi 4 / Wi‑Fi 6 模块<br><span class="note">基础版不板载</span></td>
    </tr>
    <tr>
      <td>蓝牙</td>
      <td>板载 Bluetooth 4.2</td>
      <td>选配 Bluetooth 5.0（随 Wi‑Fi 模块）</td>
    </tr>
    <tr>
      <td>蜂窝网络</td>
      <td>SoC 集成 Balong LTE 基带<br><span class="note">板卡未引出天线/卡槽</span></td>
      <td><span class="na">-</span></td>
    </tr>
    <tr class="diff">
      <td>视频编解码</td>
      <td>H.264 / H.265 解码<br>HDMI 输出 1080p@60</td>
      <td>解码最高 4K@75fps<br>编码最高 4K@40fps（H.264 / H.265）</td>
    </tr>
    <tr class="diff">
      <td>摄像头接口</td>
      <td>1 × 4‑lane MIPI‑CSI<br>1 × 2‑lane MIPI‑CSI（60‑pin HS 接口）</td>
      <td>1 × 4‑lane MIPI‑CSI<br>2 × 2‑lane MIPI‑CSI</td>
    </tr>
    <tr class="diff">
      <td>显示接口</td>
      <td>1 × HDMI（1080p@60）<br>4‑lane MIPI‑DSI（HS 接口）</td>
      <td>1 × HDMI 2.0（最高 4K@60）<br>1 × 4‑lane MIPI‑DSI（最高 4K@50）</td>
    </tr>
    <tr class="diff">
      <td>音频</td>
      <td>3.5 mm 耳机/麦克风<br>HDMI 音频<br>1 × I2S</td>
      <td>3.5 mm 耳机接口<br>扬声器接口 + 2 × 板载麦克风<br><span class="note">音频由专用 C906 核处理</span></td>
    </tr>
    <tr>
      <td>GPIO</td>
      <td>12 × GPIO<br><span class="note">40‑pin LS 排针引出</span></td>
      <td>20‑pin GPIO 排针<br><span class="note">UART / I2C / SPI 引出</span></td>
    </tr>
    <tr>
      <td>PWM</td>
      <td><span class="na">-</span></td>
      <td><span class="na">-</span></td>
    </tr>
    <tr>
      <td>ADC</td>
      <td><span class="na">-</span></td>
      <td><span class="na">-</span></td>
    </tr>
    <tr>
      <td>I2C</td>
      <td>2 ×</td>
      <td>20‑pin 排针引出（IIC）</td>
    </tr>
    <tr>
      <td>SPI</td>
      <td>1 ×</td>
      <td>20‑pin 排针引出</td>
    </tr>
    <tr>
      <td>UART</td>
      <td>2 ×</td>
      <td>20‑pin 排针引出</td>
    </tr>
    <tr>
      <td>CAN</td>
      <td>1 ×</td>
      <td><span class="na">-</span></td>
    </tr>
    <tr>
      <td>扩展插槽</td>
      <td>1 × mPCIe 插槽<br>1 × M.2 Key M 插槽</td>
      <td><span class="na">-</span></td>
    </tr>
    <tr>
      <td>排针接口</td>
      <td>40‑pin LS + 60‑pin HS<br><span class="note">96Boards 标准接口</span></td>
      <td>20‑pin GPIO 排针<br><span class="note">自定义引脚，非树莓派兼容</span></td>
    </tr>
    <tr>
      <td>板型</td>
      <td>96Boards CE（Consumer Edition）</td>
      <td>自定义迷你板型<br><span class="note">LM4A 核心板 + 底板结构</span></td>
    </tr>
    <tr class="diff">
      <td>尺寸</td>
      <td>105.26 mm x 100 mm</td>
      <td>84.45 mm x 52.07 mm</td>
    </tr>
    <tr>
      <td>供电</td>
      <td>8 V – 18 V DC 圆口<br><span class="note">96Boards 标准供电</span></td>
      <td>5 V（5.5 mm DC 圆口或 USB Type‑C）<br><span class="note">可选 PoE 模块供电</span></td>
    </tr>
    <tr>
      <td>工作温度</td>
      <td>0.0℃‑70.0℃</td>
      <td><span class="na">未标注</span></td>
    </tr>
    <tr class="diff">
      <td>软件支持</td>
      <td>Linux：Debian / AOSP<br>Android：8.1</td>
      <td>Linux：RevyOS(Debian)/openEuler/OpenWrt/OpenHarmony<br><span class="note">Android：适配中</span></td>
    </tr>
  </tbody>
</table>

<div class="sources">
数据来源：
<a href="https://www.96boards.org/product/hikey970/">96Boards — HiKey 970 官方产品页</a> ·
<a href="https://hackerboards.com/board/hisilicon‑hikey‑970">HackerBoards Board‑DB</a> ·
<a href="https://www.cnx‑software.com/2018/04/16/hikey‑970‑development‑board‑now‑up‑for‑pre‑order‑for‑299‑99/">CNX Software</a> ·
<a href="https://wiki.sipeed.com/hardware/zh/lichee/th1520/lp4a.html">Sipeed Wiki — LicheePi 4A</a>（含官方规格书/尺寸图）。
价格与规格可能随版本变动，以官方最新信息为准。
</div>
</body>
</html>
