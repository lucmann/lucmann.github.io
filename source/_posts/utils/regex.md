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

