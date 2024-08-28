---
title: vim 速查手册
date: 2020-02-29 21:58:03
tags: vim
categories: utilities
---

# Visual Block Selection

- 选择 `{}`(curly braces) 之间的行(包括 `{}`)

    ```
    v%
    ```

- 选择 `{}`(curly braces) 之间的行(不包括 `{}`)

    ```
    vi{
    ```

注意：光标必须放在 `{` 或者 `}`

# Copy & Paste

## Copy to clipboard

- Normal 模式
    - `"+y`

## Paste from clipboard

X11 window system 有 3 个 selections:

- PRIMARY 表示当前的可见 selection
- SECONDARY
- CLIPBOARD 通常所说的剪贴板，用来完成剪贴，复制和粘贴操作

Vim 有两个专门的寄存器分别与 PRIMARY selection 和 CLIPBOARD selection 对应

- `"*` (quotestar) PRIMARY
- `"+` (quoteplus) CLIPBOARD

如果要在 Vim 内部剪贴，复制/粘贴就使用 **quotestar**, 如果要将内容剪贴，复制/粘贴到系统剪贴板，就使用 **quoteplus**

举个例子，在 WSL Ubuntu 里如果想粘贴 Windows 剪贴板里的内容到 vim (最好是 neovim, 因为 vim 可能未使能 clipboard)

```
echo "Hello, world!" | clip.exe
```

- Normal 模式
    - `"+p`

如果想粘贴 Ubuntu 剪贴板里的内容

```
echo "Hello, world!" | xclip
```

- Normal 模式
    - `"*p`

## Copy from above or below

在 Insert 模式下

- `<ctrl-y>` copies the character from the line above
- `<ctrl-e>` copies the character from the line below (本行就是这样输入的)

# Search & Replace

## 删除多行 C-style 注释

- `s;/\*\_.\{-}\*/;;`

    * `;` 因为要匹配 `/`, 为了省去转义 `/` 的麻烦，将 search 命令的分隔符由 `/` 改为 `;`
    * `/\*` 匹配开始的 `/*`
    * `\_.` 匹配任意字符，包括 `\n`, 所以常用在多行匹配
    * `\{-}` 指非贪婪 (non-greedy) 匹配，即匹配最短的字串，默认是贪婪匹配，匹配最长的字串
    * `\*/` 匹配结尾的 `*/`
    * `;;` 指使用空替换匹配结果，即删除

    * 以下是 vim 中在单行中使用的通配符 (wildcard), 这些通配符一般不能匹配换行 `\n`

        * `.` 匹配除了 `\n` 的任意字符
        * `^` 行首，即锚点 (anchor)
        * `$` 行尾，即锚点
        * `\s` 匹配 space, tab, 但不匹配 `\n`

    * 以上通配符加上 `\_` 后可以在多行匹配中使用，即也可以匹配 `\n`

        * `\_.` 匹配任意字符，包括 `\n`
        * `\_^` 多行中第一行行首
        * `\_$` 多行中最后一行行尾
        * `\_s` 匹配 space, tab 和 `\n`

## 删除空白行

- `g/^\s*$/d`

    * `g` 指所有行搜索，而不是默认的光标所在行
    * `^\s*$` 匹配行首到行尾之间任意个空白字符，即空白行
    * `d` 删除命令 (delete)

## 搜索不匹配某模式的行，即反向搜索

- `/^\(\(^# .*$\)\@!.\)*$`

    * `/` 查找命令 (search) 的提示符
    * `^\(\(The_Regex\)\@!.\)*$` 反向查找的命令固定模式，本例中 `The_Regex` 是 `^# .*$` 即以 `# ` 开头的行，所以整个表达式匹配的就是不以 `# ` 开头的所有行

- `:v/^# .*$/p`

    * 与上面的反向查找命令功能相同, 但会将匹配结果显示在 Visual 模式下

## 大小写转换

- `s/\(^# .*$\)/\L\1/`

    * `\(The_Regex\)` 为了**向前索引**，即后面 `\1` 所指的部分，本例中 `The_Regex` 是 `^# .*$`, 意思同上
    * `\L\1` `\L` 指 **L**owercase, 即将 `\1` 匹配的结果中的所有字母都换成小写

- `s/\(^# .*$\)/\U\1/`

    * 与上面的表达式功能相反，即将 `\1` 匹配的结果中的所有字母都换成大写 (**U**ppercase)
