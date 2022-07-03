---
title: WSLg Has Come
date: 2021-04-25 11:53:40
tags: [WSL]
categories: utilities
---

Microsoft在`Build 21364.co_release`版本内发布了[WSLg](https://github.com/microsoft/wslg)功能，该功能允许在**WSL**里运行**X11**和**Wayland**的客户端程序(GUI Application).

<!--more-->

如果你已经加入[Windows Insider Program](https://insider.windows.com/zh-cn/)计划并且也正在使用**WSL2**, 那么只需要如下操作即可激活**WSLg**功能。

```
PS C:\Windows\system32> wsl --update
正在检查更新...
正在下载更新...
正在安装更新...
此更改将在 WSL 下次完全重启时生效。若要强制重启，请运行“wsl --shutdown”。
内核版本： 5.10.60.1
```

启动 WSL2 后，查看内核版本

```
➜  ~ uname -r
5.10.60.1-microsoft-standard-WSL2
```

升级 WSL2 后，在 `/mnt` 目录下会比原来多出一个 `wslg` 的目录

```
➜  ~ cd /mnt/
c/     wsl/   wslg/
```

# nautilus
```
apt install nautilus -y
```

{% asset_img wslg-nautilus.png "WSLg nautilus" %}

# glmark2

{% asset_img wslg-glmark2.png "WSLg glmark2" %}

