---
title: find
date: 2022-03-19 22:18:16
tags: shell
categories: utilities
---

# `find` 命令

`find` 命令用来在目录树里查找文件。本文主要介绍 GNU find. 它从每个 starting-point 开始递归搜索，通过求值 expression 的真值来确定输出结果。

# `find` 的命令行组成

![example-of-find](find.drawio.png)

- Options
- Starting-point
- Expression
    * Tests
    * Actions
    * Global options
    * Position options
    * Operators

# Examples

## 只在当前目录搜索（不递归）不以 f 或 g 或 h 开头的目录
- `find . -maxdepth 1 -name '[^fgh]*' -type d`

## 排除/proc 和 /tmp 这两个目录
- `find / -path '/proc' -prune -o -path '/tmp' -prune -o -name 'README.md'`

## 查找 ~/gh 目录下只有文件属主(u)有执行权限(x)的文件 (精确匹配文件的 permission bits)
- `find ~/gh -perm u=x -type f`

## 查找 ~/gh 目录下文件属主(u)有执行权限(x) 的文件（组用户(g)或其它用户(o)可能有或没有执行权限）
- `find ~/gh -perm -u=x -type f`

