---
title: Regular Expression
date: 2022-06-19 22:18:16
tags: [tools]
categories: utilities
---

# Regular Expressions

- [POSIX Basic Regular Expression](https://www.regular-expressions.info/posix.html#bre)
    * grep

- [POSIX Extended Regular Expression](https://www.regular-expressions.info/gnu.html#ere)
    * egrep
    * awk
    * emacs

# GNU Extensions

## Shorthand Classes

| shorthand              | equivalence                         |
|:-----------------------|:------------------------------------|
| `\w`                   | `[[:alnum:]_]`                      |
| `\W`                   | `[^[:alnum:]_]`                     |
| `\s`                   | `[[:space:]]`                       |
| `\S`                   | `[^[:space:]]`                      |

## Word Boundaries

| shorthand              | matches                                 |
|:-----------------------|:----------------------------------------|
| `\b`                   | position at a word boundary             |
| `\B`                   | position not at a word boundary         |
| `\<`                   | position at the start of a word         |
| `\>`                   | position at the end of a word           |
| `` \` `` (backtick)    | position at the start of subject string |
| `\'` (single quote)    | position at the end of subject string   |

# Perl Compatiable Regular Expression (PCRE)

## `grep` multiline mode

- `-P`: 使用 Perl 正则表达式扩展
- `-z`: 让 grep 把输入的行看成是一个多个行的集合，一个整体
- `.*?`: 后面的 `?` 表示 `.*` 按 **non-greedy** 模式匹配，也就是尽可能少(短)的匹配
- `(?s)`: 让 `.` 匹配包括 `\n` 在内的任意字符，也就是所谓的 **dot all** flag.

## `rg` multiline mode

- `-U, --multiline` 使能多行匹配，允许正则表达式里包含 `\n` (普通模式下不允许)，但 `-U` 并不会改变 `.` 的语义，所以你仍然需要显式地给 `.` 指定 `(?s)` flag

    - `rg -U 'struct file_operations .*? = ?\{(?s).*?\.mmap = (?s).*?\};' -tc drivers/gpu`
        这个例子找出内核 GPU 驱动中所有重写了 `mmap` file operation 方法的c文件
