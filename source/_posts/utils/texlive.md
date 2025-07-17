---
title: TeX/LaTex
date: 2022-04-17 08:00:02
tags: [tools]
categories: utilities
---

# TeX/LaTeX

TeX 是高德纳(Donald Knuth)发明的编程式排版语言，而 LaTeX 是莱斯利·兰波特(Leslie Lamport)在 TeX 的基础上开发的 TeX 的扩展宏集。

## 生成 PDF 的工具

- [pandoc](https://github.com/jgm/pandoc) 标记语言转换领域的"瑞士军刀"
  * 将 Markdown 生成 PDF
- [TexLive](https://ftp.ntou.edu.tw/ctan/systems/texlive/Images/) 开发最为活跃的 TeX 发行版之一
  * TexLive 是跨平台的，但一般在 Linux 下安装，MacOS 下是 MacTeX, Windows 下是 MiKTeX.
- [eisvogel](https://github.com/Wandmalfarbe/pandoc-latex-template) 简洁的 pandoc LaTex 模板
  * eisvogel 要求一个完整的 TexLive 的安装环境，但通常像 CentOS 这样的 Linux 发行版自带的 TexLive 并不完整。

# How to Install [`texlive.iso`](https://www.tug.org/texlive/acquire-iso.html)

## [Quick Install](https://www.tug.org/texlive/quickinstall.html)

将 TexLive 的镜像下载并挂载后，通过顶层目录下的 `install-tl` 这个 Perl 脚本程序安装，整个安装过程要注意以下几点。

## mount ISO image

假设在 Linux 下，通过 root 用户或 `sudo` 挂载镜像文件

```bash
mkdir -p /mnt/iso
mount /path/to/texlive.iso /mnt/iso -o loop
```
因为 ISO 文件系统是只读文件系统，所以如果想在 `/mnt/iso` 写入时，还需要借助 Linux 的 **OverlayFS** 进行再次挂载，之后在目录 `/mnt/merged` 里就有一份可写版的镜像了

```bash
mkdir -p /mnt/{upper,work,merged}
mount -t overlay overlay -o lowerdir=/mnt/iso,upperdir=/mnt/upper,workdir=/mnt/work /mnt/merged
```

## run installer

TexLive 的 installer 是一个 perl script, 提供 text mode 和 GUI mode 两种安装模式.

```
======================> TeX Live installation procedure <=====================

======>   Letters/digits in <angle brackets> indicate   <=======
======>   menu items for actions or customizations      <=======
= help>   https://tug.org/texlive/doc/install-tl.html   <=======

 Detected platform: GNU/Linux on x86_64

 <B> set binary platforms: 1 out of 6

 <S> set installation scheme: scheme-full

 <C> set installation collections:
     40 collections out of 41, disk space required: 7262 MB (free: 168365 MB)

 <D> set directories:
   TEXDIR (the main TeX directory):
     !! default location: /usr/local/texlive/2022
     !! is not writable or not allowed, please select a different one!
   TEXMFLOCAL (directory for site-wide local files):
     /usr/local/texlive/texmf-local
   TEXMFSYSVAR (directory for variable and automatically generated data):
     /usr/local/texlive/2022/texmf-var
   TEXMFSYSCONFIG (directory for local config):
     /usr/local/texlive/2022/texmf-config
   TEXMFVAR (personal directory for variable and automatically generated data):
     ~/.texlive2022/texmf-var
   TEXMFCONFIG (personal directory for local config):
     ~/.texlive2022/texmf-config
   TEXMFHOME (directory for user-specific files):
     ~/texmf

 <O> options:
   [ ] use letter size instead of A4 by default
   [X] allow execution of restricted list of programs via \write18
   [X] create all format files
   [X] install macro/font doc tree
   [X] install macro/font source tree
   [ ] create symlinks to standard directories
   [X] after install, set CTAN as source for package updates

 <V> set up for portable installation

Actions:
 <I> start installation to hard disk
 <P> save installation profile to 'texlive.profile' and exit
 <Q> quit

Enter command:
```

- **`D`**: 设置安装路径，相当于指定 `--prefix`
- **`P`**: 保存安装的配置到文件 `texlive.profile`, 之后可以直接编辑这个文件，运行 `install-tl` 时通过 `--profile` 可以直接读取这个文件中的安装配置

## create symlinks to standard directories

```
Options customization:

 <P> use letter size instead of A4 by default: [ ]
 <E> execution of restricted list of programs: [X]
 <F> create all format files:                  [X]
 <D> install font/macro doc tree:              [X]
 <S> install font/macro source tree:           [X]
 <L> create symlinks in standard directories:  [X]
            binaries to: /usr/bin
            manpages to: /usr/man
                info to: /usr/info
 <Y> after install, set CTAN as source for package updates:
                                               [X]
```

最好勾选此选项，避免使用 TexLive 时各种 `Not Found`, 原因是 TexLive 默认安装路径并不是标准 Linux 可执行程序的路径(如 `/usr/bin`), 而是 `/usr/local/texlive`, 如在 x86_64 安装 `texlive2022.iso`, 则可执行程序被安装在 `/usr/local/texlive/2022/bin/x86_64-linux`. 假如不想因为该路径没有在 `PATH` 里，最好就是在安装时直接在 `/usr/bin` 下创建相应符号链接。

## Non-interactive Installation

当你有一份 `texlive.profile` 时，就可以执行非交互式安装，`install-tl` 会直接按照 `texlive.profile` 文件里的配置安装，无需干预

```bash
./install-tl --no-interaction --profile=texlive.profile
```

# texlive-full

在使用 xelatex, pdflatex 等 latex 程序时，可以常常遇到类似

`! LaTeX Error: File 'fontawesome.sty' not found`

xxx.sty 这样的报错，一般是缺少某个 texlive 包，但 texlive 的包太多了，如果懒得一个一个安装，可以

`sudo apt install texlive-full`
