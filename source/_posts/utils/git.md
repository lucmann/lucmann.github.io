---
title: Git Cheat Sheet
date: 2021-01-25 20:42:05
tags: git
categories: utilities
---

# git am
- `git am foo.patch`
    - 应用补丁 foo.patch (注意与 `git apply` 的区别，`git am` 同时会将commit log 也应用上去)

<!--more-->

# git clone
- `git clone --recurse-submodules`
    - 克隆仓库时连同子仓库一同克隆

# git describe
- `git describe --all <commit>`
    - 找到离给定 `<commit>` 最近的 tag

# git log
- `git log -S<regex> --pickaxe-regex /path/to/a/file`
    - 查找匹配的字串在这个文件中出现的次数发生变化的 commit
- `git log -G<regex> /path/to/a/file`
    - 查找到涉及修改文件 `file` 的内容匹配正则表达式 <regex> 的 commit
- `git log --diff-filter=D --summary --oneline`
    - 找出有文件删除记录的 commit, 并显示出哪些文件被删除
    - filters 有:
        - A: Added
        - C: Copied
        - D: Deleted
        - M: Modified
        - R: Renamed
    - 当 filters 使用小写时，表示**不包含**这些变更类型
- `git log --oneline --grep "fix:"`
    - tig 类似的参数是 `--grep="fix:"`

# git shortlog
- `git shortlog -sne --author="Name Surname"
    - 统计某个贡献者的提交，像这样：`17  Luc Ma <luc@sietium.com>`

# git remote
- `git remote prune <remote>`
    - 将给定的 `<remote>` 远端不存在的**本地分支**全部删除(注意: rm -rf 警告，最好先 `git remote prune --dry-run <remote>`)

# git submodule
- `git submodule init`
    - 在主仓库初始化一个子仓库
- `git submodule update`
    - 更新所有子仓库


# .gitignore

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
