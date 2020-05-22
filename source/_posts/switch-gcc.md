---
title: gcc switch 
date: 2020-05-23 02:17:06
tags:
---

## 什么时候需要切换gcc的版本
当系统上安装了两个以上版本的gcc时，为了适应不同项目对编译器版本的特定要求，我们可能需要随时切换gcc的版本。

## 如何做
在Ubuntu(其它Linux发行版应该也有)系统，`update-alternatives`命令可以帮助我们快速简单地完成切换gcc版本。该命令的主要功能是维护`/etc/
alternatives`目录下的symbolic links，以此来决定系统某些命令(例如gcc)最终是执行哪个可执行程序。

命令原型
- 没有symbolic link时
```shell script
update-alternatives [options] --install link name path priority
```

- 更改已有symbolic link时
```shell script
update-alternatives [options] --set name path
```

举例 
- 切换gcc的版本从6.5到9.2
```shell script
sudo update-alternatives --set gcc /usr/bin/gcc-9
```

- 切换g++的版本从9.2到6.5
```shell script
sudo update-alternatives --set g++ /usr/bin/g++-6
```

NOTE: 在执行上面命令时，要保证是在已经安装了相应gcc/g++版本和其symbolic link(通过`--install`子命令)的前提下。

