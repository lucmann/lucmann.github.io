---
title: WSLg Has Come
date: 2021-04-25 11:53:40
tags: [wsl]
categories: sys
---

Microsoft在`Build 21364.co_release`版本内发布了[WSLg](https://github.com/microsoft/wslg)功能，该功能允许在**WSL**里运行**X11**和**Wayland**的客户端程序(GUI Application).

<!--more-->

如果你已经加入[Windows Insider Program](https://insider.windows.com/zh-cn/)计划并且也正在使用**WSL2**, 那么只需要如下操作即可激活**WSLg**功能。

```
wsl --update	# Administrator privilege
wsl --shutdown
```

# nautilus
```
apt install nautilus -y
```

{% asset_img wslg-nautilus.png "WSLg nautilus" %}

# glmark2

{% asset_img wslg-glmark2.png "WSLg glmark2" %}

