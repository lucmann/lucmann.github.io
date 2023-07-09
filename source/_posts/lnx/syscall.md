---
title: Linux System Calls 
date: 2023-06-13 18:30:15
tags: syscall
categories: linux
---

系统调用是Linux 用户态程序与内核通信的接口。 每个特定的文件系统都会在自己的 `file_operations`里提供各种文件操作接口，这些接口包括像 open, close, ioctl 等等


| Syscall  signature                               | Function description                                    |
|:-------------------------------------------------|:--------------------------------------------------------|
| int dup(int oldfd);                              |                                                         |
| int dup2(int oldfd, int newfd);                  |                                                         |
| uid_t getuid(void);                              |                                                         |
| uid_t geteuid(void);                             |                                                         |
| pid_t getpid(void);                              | return process ID of calling process                    |
| pid_t getppid(void);                             | return process ID of parent of calling process          |
| int ioctl(int fd, unsigned long request, ...);   | 操作底层设备参数，尤其像终端这种字符设备                |
|                                                  |                                                         |
|                                                  |                                                         |
|                                                  |                                                         |
|                                                  |                                                         |
|                                                  |                                                         |
|                                                  |                                                         |
|                                                  |                                                         |
|                                                  |                                                         |
|                                                  |                                                         |
|                                                  |                                                         |

# ioctl

![ioctl](ioctl-ioctl.drawio.png)

## ioctl 的返回值

ioctl 成功返回 0， 否则返回 -errno (有的 man ioctl 说ioctl不成功时返回 -1, 这种应该是过时的文档)。

