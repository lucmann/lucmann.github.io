---
title: Git Cheat Sheet
date: 2021-01-25 20:42:05
tags: [tools]
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
- `git describe <commit>`
    - 找到离 `<commit>` 最近的 tag

- `git describe --contains <commit>`
    - 找到**包含** `<commit>` 的 tag
    - 例：`git describe --contains a6149f03` 
      - `v6.8-rc1~111^2~23^2~81`
    - 它和不加 `--contains` 的区别是，`git describe` 仅仅是**离得最近**，从前往后都算，而 `git describe --contains` 是 **包含**，上面例子中命令的输出 `v6.8-rc1~111^2~23^2~81`， 表示在 v6.8 中一定包含 `a6149f03` 这个 commit, 而且它指出了具体的位置：从 `v6.8-rc1` 这个 tag 的 commit 往后第 111 个 commit (`^2` 表示它是一个 **merge commit**), 从这个 merge commit 再往后第 23 个 commit (仍然是一个 merge commit), 从这个 commit 再往后第 81 个 commit， 就是 `a6149f03`

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
- `fatal: remote error: upload-pack: not our ref e15ab44e64c70b54fc375c019c95bddc066a84cf
fatal: Fetched in submodule path 'submodules/ImGuiScope', but it did not contain e15ab44e64c70b54fc375c019c95bddc066a84cf. Direct fetching of that commit failed`
    - 如果你需要在一个 submodule 仓库里添加自己的修改，那么可能需要将 `.gitmodules` 文件里对应仓库的 `url` 变成你自己的**远程可写**仓库的 URL

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

# GitHub

## `ssh: connect to host github.com port 22: Connection timed out`

之前好好的，突然 `git pull`, `git clone` 等这个错误，两种解决方法

- 修改 git 配置

```shell
git config --global "url.ssh://git@ssh.github.com:443.insteadOf" git@github.com:
```

- 修改 ssh 配置

``` vim ~/.ssh/config
Host github.com
    Hostname ssh.github.com
    Port 443
```
