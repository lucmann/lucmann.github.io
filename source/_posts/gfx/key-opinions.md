---
title: "Replacing NIR with SPIR-V?"
date: 2023-09-16 16:09:13
tags: [compiler, intermediate representation]
categories: graphics
---

# 背景

## 文章的背景

这篇文章的题目来自 [mesa](https://www.mesa3d.org/) 开发者社区邮件列表(mesa-dev@lists.freedesktop.org) 里 2022 年 1 月 20 日的[一封邮件主题](https://www.mail-archive.com/mesa-dev@lists.freedesktop.org/msg224164.html)。首先向这些邮件参与者表示感谢！谢谢他们分享这些知识与见解。邮件讨论虽然不长，但涉及到了 GPU, shader 编译器等关键话题，里面的观点可能都是大牛们多年 hack GPU 积累的金玉良言，十分值得一读。

## 主题的背景

- 缩写

| 名词   |  全称                            | 解释                                                                                       |
|:-------|:---------------------------------|:-------------------------------------------------------------------------------------------|
| IR     | Intermediate Representation      | 编译器中使用的中间表示，一般是编译器优化 Pass 和后端的输入                                 |
| NIR    | New IR                           | Mesa 中专门为 Graphics shader 设计的一种 IR, 它可以作为特定后端的输入，如 LLVM AMDGPU 后端 |
| SPIR   | Standard Portable IR             | 一种用于并行计算和图形的中间语言（interchage language), 由 Khronos Group 在 2012 年引入    |
| SPIR-V | SPIR-Vulkan                      | 由 Khronos Group 在 2015 年引入，用来替代原来的 SPIR                                       |
| RISC   | Reduced Instruction Set Computer | 精简指令集， 与 CISC 相对                                                                  |
| RISC-V | RISC five                        | 由加州 Berkeley 大学开发的一种开放且模块化的 RISC 指令集架构                               |
| SIMD   | Single Instruction Multiple Data | 费林分类法（Flynn's Taxonomy) 中的一种，其它的还有 SISD, MIMD, MISD                        |
| SIMT   | Single Instruction Multi-Threads | SIMD 的一种子类型                                                                          |


（以下都是从邮件中摘取的片段，翻译水平有限，请读者谅解）

# 参考

- [Lightning Talk: Functional Gap between RISC-V V and SPIR-V: a Study Case on the Graphics Domain](https://www.youtube.com/watch?v=kM0lsWjqOaw)
- [SIMD in the GPU world](https://www.rastergrid.com/blog/gpu-tech/2022/02/simd-in-the-gpu-world/)

