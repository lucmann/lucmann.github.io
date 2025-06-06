---
title: find
date: 2022-03-19 22:18:16
tags: [tools]
categories: utilities
---

# `find` 命令

`find` 命令用来在目录树里查找文件。本文主要介绍 GNU find. 它从每个 starting-point 开始递归搜索，通过求值 expression 的真值来确定输出结果。

# `find` 的命令行组成

![example-of-find](/images/find/find.drawio.png)

- Options
- Starting-point
- Expression
    * Tests
    * Actions
    * Global options
    * Position options
    * Operators

# Examples

- 只在当前目录搜索（不递归）不以 f 或 g 或 h 开头的目录
    - `find . -maxdepth 1 -name '[^fgh]*' -type d`
        - `-type d`: 找目录
        - `-type f`: 找文件(不包括 symbolic link)
        - `-type l`: 找符号链接文件

- 排除/proc 和 /tmp 这两个目录
    - `find / -path /proc -prune -o -path /tmp -prune -o -name "README.md"`
        - `-prune` 告诉 find 跳过前面的目录，也可以用 `\( -o \)` 将多个 `-path` 合并，只保留一个 `-prune` 
- 只在当前目录查找除了指定及隐藏目录以外的所有目录，打印并删除
    - `find -maxdepth 1 \( -path ./gh -o -path ./aaa -o -path ./mesa-install -o -path ./1.3.290.0 -o -path ./1.3.280.1 \) -prune -o -type d ! -name ".*" -print -exec rm -rf {} \;`
        - 注意匹配隐藏目录时用 `.*`, `.` 在 shell 里不是通配符
        - `-print`： 不让 `./gh`, `./aaa` 这些目录出现在 `find` 命令的输出结果中

- 查找 ~/gh 目录下只有文件属主(u)有执行权限(x)的文件 (精确匹配文件的 permission bits)
    - `find ~/gh -perm u=x -type f`

- 查找 ~/gh 目录下文件属主(u)有执行权限(x) 的文件（组用户(g)或其它用户(o)可能有或没有执行权限）
    - `find ~/gh -perm -u=x -type f`

