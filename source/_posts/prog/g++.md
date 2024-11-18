---
title: g++ 那些事儿
date: 2021-04-15 15:58:26
tags: [Compiler]
categories: programming
---

# Installation on the Ubuntu

除了安装 C++ 标准库实现 `libstdc++-9`, `g++-9` 这些软件包外，还要安装 `g++`

`dpkg-deb --contents /var/cache/apt/archives/g++_4%3a9.3.0-1ubuntu2_amd64.deb`

```
drwxr-xr-x root/root         0 2020-03-20 21:52 ./
drwxr-xr-x root/root         0 2020-03-20 21:52 ./usr/
drwxr-xr-x root/root         0 2020-03-20 21:52 ./usr/bin/
drwxr-xr-x root/root         0 2020-03-20 21:52 ./usr/share/
drwxr-xr-x root/root         0 2020-03-20 21:52 ./usr/share/doc/
drwxr-xr-x root/root         0 2020-03-20 21:52 ./usr/share/man/
drwxr-xr-x root/root         0 2020-03-20 21:52 ./usr/share/man/man1/
lrwxrwxrwx root/root         0 2020-03-20 21:52 ./usr/bin/g++ -> g++-9
lrwxrwxrwx root/root         0 2020-03-20 21:52 ./usr/bin/x86_64-linux-gnu-g++ -> g++-9
lrwxrwxrwx root/root         0 2020-03-20 21:52 ./usr/share/doc/g++ -> cpp
lrwxrwxrwx root/root         0 2020-03-20 21:52 ./usr/share/man/man1/g++.1.gz -> g++-9.1.gz
lrwxrwxrwx root/root         0 2020-03-20 21:52 ./usr/share/man/man1/x86_64-linux-gnu-g++.1.gz -> g++-9.1.gz
```
