---
title: Make Cheatsheet
date: Thu Jul 31 09:59:05 CST 2025
tags: [tools]
categories: utilities
---


<!--more-->

# Make 常用变量

## MAKECMDGOALS

表示执行 make 命令时用户给入的 targets, 这里*执行 make 命令时* 不仅包括从终端命令行执行 make, 也包括在 Makefile 规则里执行 `make -f ...`

# Make 常用函数

## info, error, warning

- 原型
  ```
  $(info text...)
  $(error text...)
  $(warning text...)
  ```
  * 这3个函数被放在 GNU Make 官方文件 [8.13 Functions That Control Make](https://www.gnu.org/software/make/manual/html_node/Make-Control-Functions.html) 章节，这里把它们放在常用函数最前面，是因为它们对于验证调试 Makefile 非常有用
  * `$(info text...)` 相当于 `printf()`
  * `$(error text...)` 相当于 `assert(0)`
  * info 和 warning 的区别是前者打印在 **stdout**, 后者打印在 **stderr**
  * info, warning, error 函数打印调试 makefile 之所以比在规则里使用 `echo` 方便，是因为它们作为内置函数可以在 makefile 的任何地方单独出现，而不像 echo 只能在规则里出现
  * make 里的函数也会**展开 expansion**, 相当于 make 函数都有返回值，info, warning 函数展开后都是空字串

## dir, notdir

- 原型
  ```
  $(dir names...)
  $(notdir names...)
  ```
  * 这两个内置函数被放在 GNU Make 官方文档 [8.3 Functions for File Names](https://www.gnu.org/software/make/manual/html_node/File-Name-Functions.html) 章节的头两个，可见它们的使用之多。这一章节的函数都是专门用来**拆分**文件路径的。
  * `$(dir names...)` 只取出后面文件路径(或文件路径列表)的目录部分
  * `$(notdir names...)` 只取出后面文件路径(或文件路径列表)的文件名部分

## foreach

- 原型
  ```
  $(foreach var, list, text)
  ```
  * expansion 时机：`var`, `list` 在一开始就展开，而 `text` 是在后面才展开
  * `list` 是一个空白分隔的字符串
  * `text` 的多次展开也被**空白**连接成一整个长字符串

