---
title: 词汇: nightly
date: 2021-11-19 15:00:41
tags:
categories: misc
---

最近在两个地方看到**nightly**这个词：

[drm-tip](https://cgit.freedesktop.org/drm-tip/)或[drm/drm-tip](https://cgit.freedesktop.org/drm/drm-tip/) (我也不知道这两个仓库有什么区别，除了它们的Owner不同)

DRM current development and **nightly** trees

<!--more-->

另外一个地方是Rust的版本发布策略，它采用[train schedule](https://doc.rust-lang.org/book/appendix-07-nightly-rust.html). Rust总共有3个Release通道:

- Nightly
- Beta
- Stable

从上到下，各个Release版本的更新程度(包含新特性的多少)依次减少，稳定程序依次增加，也就是说，对于大多数普通的开发者，会选择安装Stable, 而对于那些想尝试Rust语言最新的特性的开发者，则会选择安装Nightly或Beta版本。

你可以使用rust toolchain管理工具`rustup`来安装和切换不同的Release版本，例如

```bash
rustup default nightly
```

将安装最新的Rust nightly版本(这篇博客写于Fri Nov 19 15:28:25 CST 2021)

```
nightly-x86_64-unknown-linux-gnu installed - rustc 1.58.0-nightly (cc946fcd3 2021-11-18)
```

从上面两个Nightly的用法看，它基本上代表最新的，最近的，让我想到以前项目的“每日构建版本”。

