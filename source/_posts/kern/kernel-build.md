---
title: Building Linux kernel under Ubuntu
date: 2022-11-06 11:38:43
tags:
categories: kernel
---

# Environment

| configuration             | version                            |
|:--------------------------|:-----------------------------------|
| distro                    | Ubuntu 22.04 Jammy Jellyfish       |
| gcc                       | 11.2.0                             |
| original kernel           | 5.15.0-43-generic                  |
| building kernel           | v6.0                               |


# Should Know Something About Kernel Building

- 配置内核最简单的方法是 `make olddefconfig`
- 内核配置保存在 .config
- 内核源码树里包含命令行修改 .config 的脚本 scripts/config
- 内核构建依赖 flex, bison 词法分析程序


# Compile Errors 

|  Errors                                                        | Resolution                                             |
|:---------------------------------------------------------------|:-------------------------------------------------------|
| gelf.h: No such file or directory                              | apt install libelf-dev                                 |
| <openssl/opensslv.h>: No such file or directory                | apt install libssl-dev                                 |
| No rule to make target 'debian/canonical-certs.pem'            | scripts/config --disable SYSTEM_TRUSTED_KEYS           |
| No rule to make target 'debian/canonical-revoked-certs.pem'    | scripts/config --disable SYSTEM_REVOCATION_KEYS        |
