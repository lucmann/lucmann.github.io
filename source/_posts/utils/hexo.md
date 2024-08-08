---
title: Blog using Hexo
date: 2021-01-25 20:42:05
tags: hexo
categories: utilities
---

# npm

hexo 依赖的很多 nodejs 包都过期了，但发现在Ubuntu 23.04 上很难更新(编译node-sass 要用的 python2 都需要自己源码编译安装)，索性先搁置吧。

<!--more-->

- `npm install` 慢
    - `npm config get registry`
    - `npm config set registry=https://registry.npmmirror.com`
- `FetchError: request to https://registry.npm.taobao.org/ansi-regex failed, reason: certificate has expired`
    - `npm config set strict-ssl false`
    - `npm cache clean --force`

