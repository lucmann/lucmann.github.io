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
  h1 { font-size: 24px; text-align: center; }
  .subtitle {
    text-align: center;
    color: #888;
    font-size: 14px;
    margin-bottom: 24px;
  }
  a { color: #2c5f9e; text-decoration: none; }
  a:hover { text-decoration: underline; }
  .comparison { overflow-x: auto; }
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
  /* 标题行（第一行）：蓝底白字，与 jm 表一致 */
  .comparison tr:first-child th,
  .comparison tr:first-child td {
    background: #2c5f9e;
    border: 1px solid #2c5f9e;
    text-align: center;
  }
  .comparison tr:first-child a { color: #fff; }
  /* 第一列对比项：灰底加粗 */
  .comparison th {
    background: #f5f7fa;
    font-weight: bold;
    width: 130px;
  }
  /* 有差异的行：黄色高亮 */
  tr.diff td { background: #fff8e6; }
  tr.diff th { background: #fdeec9; }
  .comparison img {
    border-radius: 4px;
    max-width: 120px;
    height: auto;
  }
  .comparison .boardname {
    display: block;
    margin-top: 4px;
    font-size: 1.05em;
    color: #fff;
  }
  .note { font-size: 0.85em; color: #888; }
  footer.foot {
    margin-top: 24px;
    font-size: 13px;
    color: #666;
  }
</style>
<base target="_blank">
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

      <tr class="diff">
        <th>Price<br>参考价格</th>
        <td class="">
          $ 299.99<br><span class="note">上市预售价，现已停产</span>
        </td>
        <td class="">
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

      <tr class="diff">
        <th>NPU<br>AI 加速</th>
        <td class="">
          专用 NPU（HiAI 架构）
        </td>
        <td class="">
          4 TOPS @ INT8（主频 1 GHz）<br>
          <span class="note">支持 TensorFlow / ONNX / Caffe</span>
        </td>
      </tr>

      <tr class="diff">
        <th>RAM<br>内存</th>
        <td class="">
          6.0 GiB LPDDR4X @ 1866 MHz
        </td>
        <td class="">
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

      <tr class="diff">
        <th>USB</th>
        <td class="">
          2 &times; USB 3.0 Type-A host<br>
          2 &times; USB Type-C（1 &times; OTG + 1 &times; host）
        </td>
        <td class="">
          4 &times; USB 3.0 Type-A host<br>
          1 &times; USB 2.0 Type-C（仅用于烧录）
        </td>
      </tr>

      <tr class="diff">
        <th>Ethernet<br>以太网</th>
        <td class="">
          <div>1 &times; 1&nbsp;Gbps</div>
        </td>
        <td class="">
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

      <tr class="diff">
        <th>Video<br>视频编解码</th>
        <td class="">
          H.264 / H.265 解码<br>
          HDMI 输出 1080p@60
        </td>
        <td class="">
          解码最高 4K@75fps<br>
          编码最高 4K@40fps（H.264 / H.265）
        </td>
      </tr>

      <tr class="diff">
        <th>Camera<br>摄像头</th>
        <td class="">
          1 &times; 4-lane MIPI-CSI<br>
          1 &times; 2-lane MIPI-CSI（60-pin HS 接口）
        </td>
        <td class="">
          1 &times; 4-lane MIPI-CSI<br>
          2 &times; 2-lane MIPI-CSI
        </td>
      </tr>

      <tr class="diff">
        <th>Display<br>显示接口</th>
        <td class="">
          1 &times; HDMI（1080p@60）<br>
          4-lane MIPI-DSI（HS 接口）
        </td>
        <td class="">
          1 &times; HDMI 2.0（最高 4K@60）<br>
          1 &times; 4-lane MIPI-DSI（最高 4K@50）
        </td>
      </tr>

      <tr class="diff">
        <th>Audio<br>音频</th>
        <td class="">
          3.5 mm 耳机/麦克风<br>
          HDMI 音频<br>
          1 &times; I2S
        </td>
        <td class="">
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

      <tr class="diff">
        <th>Dimensions<br>尺寸</th>
        <td class="">
          105.26 mm x 100 mm
        </td>
        <td class="">
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

      <tr class="diff">
        <th>Software<br>软件支持</th>
        <td class="">
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
