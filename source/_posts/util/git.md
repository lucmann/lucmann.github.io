---
title: git 学习笔记
date: 2021-01-25 20:42:05
tags: git
categories: utilities
---

# git-submodule

当克隆一个含有子模块的 git 仓库时，有 2 种方法可以连同它的子模块一起克隆下来。

<!--more-->

第一种方法是在克隆时，直接指示 git 将所有子模块都克隆下来

```
git clone --recurse-submodules
```

第二种方法是在克隆后，使用 `git submodule` 子命令再将所有子模块初始化，克隆下来

```
git submodule init
git submodule update
```

上面命令可以合并为一个命令

```
git submodule update --init
```

## 与子模块相关的 git 配置文件和目录

- `.gitmodules`

    记录了子模块的名字（路径）和 URL

- `.git/config`

    克隆子模块后，它的路径和 URL 会记录上这个文件中

- `.git/modules`

    克隆子模块后，它的 Index 会保存在这个目录

## 克隆时出现未完全克隆的子模块

`git clone --recurse-submodules` 实际上先从远端把所有子模块拉取下来，然后再把每个子模块检出到你项目里所列的合适的提交，但 git-clone 允许它们失败，当拉取某个子模块时失败了，这个子

![git-submodule-update](git-submodule-update.png)

# gitignore

## 记几个与gitignore相关的命令：

```
git check-ignore -v file # 输出file是写在哪个.gitignore文件中
git status -s --ignored # 查看所有被ignored的文件
git ls-tree -r --name-only master # 查看所有被tracked的文件
git rm --cached file # 永久撤销对file文件的跟踪
```

## 记几个与gitignore相关的要点：

- git支持嵌套的`.gitignore`文件，就是说可以在repo下的某个子目录里写一个`.gitignore`文件，但是这个`.gitignore`只能影响这个目录下的内容
- `.gitignore`文件里的记录*只影响untracked*的文件，就是说如果想ignore一个之前已经跟踪了的文件，就必须先对这个文件执行`git rm --cached`，否则即使把这个文件写到`.gitignore`中也不生效。而且会产生疑惑，明明已经把某个文件加到`.gitignore`中了，为什么仍然被跟踪? 这时最好使用`git ls-tree -r --name-only master`命令确认那个文件是否还在被跟踪。
