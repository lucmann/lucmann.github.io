---
title: 景美 JM1100 vs JM9100M vs JM9100 性能指标对比
date: 2026-10-11
tags: [Hardware]
categories: hardware
---
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>景美 JM1100 vs JM9100M vs JM9100 性能指标对比</title>
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

<h1>景美 JM1100 vs JM9100M vs JM9100 性能指标对比</h1>
<p class="subtitle">黄色行为三款产品有差异的指标（含仅部分产品公布的指标）</p>

<table>
  <thead>
    <tr>
      <th>技术指标</th>
      <th>JM1100</th>
      <th>JM9100M</th>
      <th>JM9100</th>
    </tr>
  </thead>
  <tbody>
    <tr class="diff">
      <td>时钟频率</td>
      <td>GPU 主频 1.3 GHz</td>
      <td>内核时钟频率 1000MHz（支持动态调频）</td>
      <td>最高 1.35 GHz</td>
    </tr>
    <tr class="diff">
      <td>主机接口</td>
      <td>PCIe 4.0 x16（向下兼容）</td>
      <td>PCIe 4.0 x8/x4/x2/x1</td>
      <td>PCIe 4.0 x8/x4/x2/x1</td>
    </tr>
    <tr class="diff">
      <td>运算能力</td>
      <td>FP32 1.3 TFLOPS，FP16 2.6 TFLOPS</td>
      <td>FP32 512 GFLOPS</td>
      <td><span class="na">未公布</span></td>
    </tr>
    <tr class="diff">
      <td>显存容量</td>
      <td>16GB</td>
      <td>支持 1GB–4GB</td>
      <td>2GB / 4GB</td>
    </tr>
    <tr class="diff">
      <td>显存类型 / 位宽</td>
      <td>LPDDR4 / DDR4，位宽 256 bit</td>
      <td>LPDDR4 / DDR4</td>
      <td>LPDDR4，位宽 64 位</td>
    </tr>
    <tr class="diff">
      <td>显存带宽 / 频率</td>
      <td>135.6 GB/s（LPDDR4）/ 102 GB/s（DDR4）；LPDDR4@4266MHz、DDR4@3200MHz</td>
      <td>显存带宽 25.6 GB/s</td>
      <td>显存频率 3.733 GHz</td>
    </tr>
    <tr class="diff">
      <td>可信计算</td>
      <td><span class="na">未公布</span></td>
      <td>支持国密一级 SM2/SM3/SM4 算法</td>
      <td>支持国密一级 SM2/SM3/SM4 算法</td>
    </tr>
    <tr class="diff">
      <td>渲染能力</td>
      <td>像素填充率 38.4 GPixel/s，纹理填充率 38.4 GTexel/s</td>
      <td>含 256 个 shader 核，像素填充率 8 GPixel/s，纹理填充率 16 GTexel/s</td>
      <td>像素填充率 8 GPixel/s，纹理填充率 16 GTexel/s</td>
    </tr>
    <tr class="diff">
      <td>编解码</td>
      <td>4 路 4K@60fps 解码、2 路 4K@60fps 编码；H.265/264 编解码，VP9、AV1、AVS 等解码，支持 YUV444 编码</td>
      <td>一路 4K@60fps 解码，支持 H.265/264 等常用格式</td>
      <td>一路 4K@60fps 解码，支持 H.265/264 等常用格式</td>
    </tr>
    <tr class="diff">
      <td>显示输出</td>
      <td>4 路独立显示控制器（4 屏异显），单路最大 4K@60Hz；4 路 HDMI 2.0、2 路 eDP 1.2、1 路 VGA</td>
      <td>2 路独立显示控制器；2 路 HDMI 2.0、1 路 eDP 1.2、1 路 VGA</td>
      <td>2 路独立显示控制器；2 路 HDMI 2.0、1 路 eDP 1.2、1 路 VGA</td>
    </tr>
    <tr class="diff">
      <td>其他外设接口</td>
      <td><span class="na">未公布</span></td>
      <td>GPIO、SPI、I2C、PWM、Temp Sensor、SMBUS</td>
      <td>GPIO、SPI、I2C、PWM、Temp Sensor、SMBUS</td>
    </tr>
    <tr class="diff">
      <td>操作系统</td>
      <td>Windows、Linux、Android；兼容 ARM、X86、C86、LoongArch、SW64 平台</td>
      <td><span class="na">未公布</span>（简介称全面支持国产 CPU / 操作系统 / 固件）</td>
      <td><span class="na">未公布</span>（简介称全面支持国产 CPU / 操作系统 / 固件）</td>
    </tr>
    <tr class="diff">
      <td>API 接口</td>
      <td>OpenGL 4.6、OpenCL 3.0、Vulkan 1.3、OpenGL ES 3.2、WebGL 2.0、DirectX 11</td>
      <td>OpenGL 4.0、OpenCL 3.0、OpenGL ES 3.2（简介另提及 Vulkan 1.1）</td>
      <td>OpenGL 4.0、OpenCL 3.0、OpenGL ES 3.2（简介另提及 Vulkan 1.1）</td>
    </tr>
    <tr class="diff">
      <td>封装</td>
      <td><span class="na">未公布</span></td>
      <td>23mm × 23mm FCBGA</td>
      <td><span class="na">未公布</span></td>
    </tr>
    <tr class="diff">
      <td>功耗</td>
      <td>TDP 60W（整卡）</td>
      <td>2 ~ 15W</td>
      <td>5 ~ 15W</td>
    </tr>
    <tr>
      <td>工作温度</td>
      <td>0℃–70℃ 商业级、-40℃–85℃ 工业级</td>
      <td>0℃–70℃、-40℃–85℃</td>
      <td>0℃–70℃、-40℃–85℃</td>
    </tr>
  </tbody>
</table>

<p class="note">注："未公布"表示该产品官网规格表中未列出该项。JM1100 定位高性能渲染 / 云桌面 / 云手机等云端渲染场景；JM9100 / JM9100M 面向 PC、工控机、图形工作站等设备。</p>

<div class="sources">
  数据来源：
  <a href="https://www.jemoic.com/products_6/820.html">JM1100 产品页</a> ·
  <a href="https://www.jemoic.com/products_6/723.html">JM9100M 产品页</a> ·
  <a href="https://www.jemoic.com/products_6/381.html">JM9100 产品页</a>
</div>

</body>
</html>
