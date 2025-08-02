---
title: Make Cheatsheet
date: Thu Jul 31 09:59:05 CST 2025
tags: [tools]
categories: utilities
---


<!--more-->
# Make 常用参数

## -d, --debug=FLAGS

- 所有支持的 FLAGS (多个 flags 用`,` 分开)
- Debug Level Specification (GNU Make 4.3)

| 标志 |名称	|功能描述|
|:----|:-----|:------|
| a	| All	     |启用所有调试输出（等效于单独使用 -d）|
| b	| Basic	   |基本调试：显示目标重建决策、过期检查等核心信息|
| v	| Verbose	 |详细模式：比 b 更详细的执行信息|
| i	| Implicit |隐式规则：显示隐式规则搜索/应用过程|
| j	| Jobs	   |作业控制：输出子进程执行细节（命令/PID/退出码）|
| m	| Makefile |Makefile 处理：跟踪 include/重解析过程|
| r	| Remaking |重制检查：显示 makefile 自身的重制决策|
| n	| None	   |禁用所有调试（特殊用途） |

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
  * 原样打印，不支持转义字符如 `\n`
  * info, warning, error 函数打印调试 makefile 之所以比在规则里使用 `echo` 方便，是因为它们作为内置函数可以在 makefile 的任何地方单独出现，而不像 echo 只能在规则里出现
  * make 里的函数也会**展开 expansion**, 相当于 make 函数都有返回值，info, warning 函数展开后都是空字串

## dir, notdir

- 原型
  ```
  $(dir names...)
  $(notdir names...)
  ```
  * 这两个内置函数被放在 GNU Make 官方文档 [8.3 Functions for File Names](https://www.gnu.org/software/make/manual/html_node/File-Name-Functions.html) 章节的头两个，可见它们的使用之多。这一章节的函数都是专门用来**拆分**文件路径的。
  * `$(dir names...)` 只取出后面文件路径(或文件路径列表)的目录部分, 相当于 shell 里的 `dirname`
  * `$(notdir names...)` 只取出后面文件路径(或文件路径列表)的文件名部分, 相当于 shell 里的 `basename`

## foreach

- 原型
  ```
  $(foreach var, list, text)
  ```
  * expansion 时机：`var`, `list` 在一开始就展开，而 `text` 是在后面才展开
  * `list` 是一个空白分隔的字符串
  * `text` 的多次展开也被**空白**连接成一整个长字符串

## if

- 原型
  ```
  $(if condition, then-part[, else-part])
  ```
  * 如果 *condition* 展开后是 non-empty string, 那么条件为真
  * *then-part* 和 *else-part* 永远只能有一个被求值 (evaluated)
  * if 函数同样有返回值，条件真时，返回 *then-part* 的求值结果; 条件假时，返回 *else-part* 的求值结果, 如果没有 *else-part*, 返回空字串

## or, and

- 原型
  ```
  $(or condition1[,condition2[,condition3...]]）
  $(and condition1[,condition2[,condition3...]]）
  ```
  * **短路或(short-circuiting OR)**(不是骂人🐶), 即一个挨一个展开 *condition*, 只要遇到一个展开为**非空字串**，就停止，这个函数返回的就是展开的那个字串，如果所有 *condition* 展开都是空，则返回空字串
  * **短路与(short-circuiting AND)**, 即一个挨一个展开 *condition*, 只要遇到一个展开为**空字串**，就停止，这个函数返回的就是空字串，如果所有 *condition* 展开都是非空字串，返回**最后一个字串**
  * *condition* 可以有 1 到 n 个
      
## origin

- 原型
  ```
  $(origin variable)
  ```
  * *variable* 不需要加 `$` 符号
  * 它的功能类似 shell 里的 `which` 或 `type` (告诉你一个命令是不是 shell 内置命令)
  * 它的返回值有以下这些(都是字串)
    - undefined
    - default
    - environment
    - environment override
    - file
    - command line
    - override
    - automatic
  
# Make 用户手册

- [Make Manual](https://www.gnu.org/software/make/manual/make.html)
