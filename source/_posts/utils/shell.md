---
title: Advanced Shell
date: 2022-06-19 22:18:16
tags: shell
categories: utilities
---

# 基础知识 - 变量和函数

Linux 下的 Shell 有很多， sh, bash, csh, zsh 等, 这里主要记录一下 bash 和 zsh 的一些不同之处

## Parameter Expansion

### Use an alternate value

```bash
${var:+WORD}
${var+WORD}
```

如果 `var` 没有设置或为空，则这个变量展开为 Nothing (注意：不是空 empty, 是 nothing), 如果被设置了（不包括被设置成空），它展开为 `+` 后面的 WORD.

如果冒号被省略，则 `var` 即使被设置为空，它也展开为 `+` 后面的 WORD


### Indirect

```bash
${!var}
```

如果 `var` 的值是 `MESA_DEBUG`, 那么这个形式展开后是变量 `MESA_DEBUG` 的值，例如 `export MESA_DEBUG=1`, `var=MESA_DEBUG`, 则最后的展开结果是 `1`

### Quoted

```bash
${var@Q}
```

带 `@Q` 指变量展开后的值被单引号引起来，例如 `export ABC=abc`, `echo "ABC=${ABC@Q}"` 的结果是 `ABC='abc'`

## 反斜杠 backslash \

`\` 在 shell 中是用来转义字符的，就是说 `echo "\\\\"` 显示的实际只有一个 `\`, 而且 `while read var` 时要注意加 `-r` 选项，读入原始字串

# 经典命令 

## exec

`exec` 命令有 2 个特点：
- 直接覆盖当前进程，就是说进程 PID 不变，但执行的代码被更换了
- 原来的 shell 环境被销毁，这样当前的代码结束后，也就不会返回原 shell(没得返回)， 直接退出, 所以**循环中慎用 exec**
- 可以把它想像成系统调用 `execve()`

下面是 linux kernel 安装 bzImage 的一段代码，其中就使用了 `exec`, 保证列出的 4 个安装脚本中，只执行第 1 个存在的，不会重复安装

```shell
# User/arch may have a custom install script
for file in "${HOME}/bin/${INSTALLKERNEL}"		\
	    "/sbin/${INSTALLKERNEL}"			\
	    "${srctree}/arch/${SRCARCH}/install.sh"	\
	    "${srctree}/arch/${SRCARCH}/boot/install.sh"
do
	if [ ! -x "${file}" ]; then
		continue
	fi

	# installkernel(8) says the parameters are like follows:
	#
	#   installkernel version zImage System.map [directory]
	exec "${file}" "${KERNELRELEASE}" "${KBUILD_IMAGE}" System.map "${INSTALL_PATH}"
done
```

## timeout

`timeout` 命令用来给一个 *COMMAND* 设定一个 *DURATION*， 这在自动化中很有用，比如

```shell
for i in `fd --type x`; do timeout -k 0.1 10 ./$i; done
```
假如当前在 [VulkanExamples](https://github.com/SaschaWillems/Vulkan) 的 bin 目录下，上面的命令表示让每个 demo 执行 **10s** 后结束(`timeout` 给它发 `TERM` 信号), 如果发了 `TERM` 信号后，又经过 **0.1s (`-k 0.1`)**，这个 demo 还未能退出，就再发 `KILL` 信号，强制结束它

## tree

`tree` 像一个简单的文件浏览器，但它并不是 shell 内置的命令，`apt install tree` 或 `pacman -S tree` 都可以安装。有时一个目录中包含太多的文件，tree 的默认输出就不太好浏览，这时可以只打印目录，并限制搜索深度

```
tree -L 2 -d
```

## crontab

`crontab -e` (添加定时任务)， 不光可以添加周期性的定时任务，也可以添加开机时一次性任务

```bash
@reboot /home/luc/mystart.sh
```

crontab 是每用户的， 就是说当前用户设定的任务，只有当前用户的权限，所以如果有些情况下任务执行需要 root 权限，就需要切换到 root 用户后 `crontab -e`

# 文本处理 - awk, sed, grep(rg) 三剑客

Linux 下的文本处理三剑客: grep, sed, awk, 除了它们其实还有一些小巧的命令，如 `tr`, `cut` 也可以帮助我们快速处理和格式化文本。
下面以一个例子为例。

## awk

```
awk 'program' inputfile1 inputfile2 ...
```

awk 的调用形式就是上面这样的，而其中的 *program* 由若干条 rules 组成，而一条 *rule* 由一个 *pattern* 和一个 *action* 组成

```
pattern { action }
pattern { action }
...
```

- `awk '{$1=$1};1'`
	- 将一个字段前后的空白(空格和 Tab) 全部清除

## sed

sed 和 awk 一样，都是按行处理文本的。

- `sed -n '2 {s/^/#/; p; q}' file`
	* sed 默认会将每一行都打印出来， `-n` 取消这一行为
	* sed 可以在操作的前面指定位置和范围， 如
		- 行号
		- 正则表达式 `/^foo/`
		- 两个正则表达式锁定范围 `/^foo/, /bar$/`
	* 如果 `-n` 后，完全都不打印了，但如果又想将处理后的行打印出来，使用 `p` 命令
	* `q` 命令的作用是**立即退出**，sed 的默认行为是对第2行处理完后，虽然后面的行都不需要处理，但 sed 仍然会继续将后面的每行往**模式空间**加载。

## grep/ripgrep

- **在 Linux 内核源码目录下，搜索 `drivers/gpu/drm` 下所有的 `DRIVER_NAME` 定义，并排序后格式化输出**
	```bash
	rg '#define DRIVER_NAME' drivers/gpu/drm --no-heading \
		| tr -s '\t' | tr '\t' ' ' | tr -s ' ' \
		| awk -F':' '{printf("%-52s%-40s\n",$1,$2)}' \
		| sort -k4
	```
	- `rg` ([ripgrep](https://github.com/BurntSushi/ripgrep)) 比 grep 更快，更强大
	- `tr` 在不带任何选项时，默认执行替换，例子是中将 tab 替换成 空格, `-s` 表示 `squeeze-repeats`, 就是去掉重复的字符，例如多个空格只保留一个
	- `awk` 天生支持 C-Style printf
	
- `rg '\[package\]' -ttoml --glob '!Cargo.lock'`
	- 在一个 rust 项目顶层目录，只搜索 .toml 文件中的 `[package]`, 而忽略所有 Cargo.lock 文件
	
## 引号

当混合使用 awk 和 sed 时，比较便利地处理引号的方法是定义 awk 变量 `-v Q="'"`

- awk 的 `printf()` 函数第一个参数必须使用双引号 `printf("%s: %s%d%s\n", $1, Q, $2, Q)`
	* awk 中的用户自定义变量和内置变量，使用时都不需要加 `$`， 如 `NR`, `NF`
- awk 和 sed 的命令字串必须用引号括起来，当用 awk 生成 sed 命令时，将单引号定义为 awk 变量尤其方便，可读性也强
	
## 行范围 (Line range)

sed, awk 都支持行范围，如 `sed -n '3,5p'`, `awk 'NR >= 3 && NR <= 5'`，它们的效果是一样的，都是只打印给定文件的 3 到 5 行， 除了**行号**， sed, awk 还支持通过**正则匹配**来指定行位置

```
sed -n '/foo/,$p'
```

- `$` 表示最后一行
- `p` 是 sed `print` 命令的缩写

```
awk '/foo/ {f=1} f'
```

- awk 实现行范围的方式与 sed 稍有不同，它在满足匹配 `foo` 这个条件时，将**布尔**变量 `f` 设置为 1，相当于从这一行开始，开启 `print` 打印这个默认操作
- awk 的一条 rule 里，可以省略 *pattern*, 或者省略 *action*, 但**不能同时两个都省略**；如果省略 *pattern*, 则对每个输入行都执行 *action*, 如果省略 *action*, 则对每个匹配行执行 `print` *action*。这个例子里面， awk 的 *program* 包括两条 *rules*
	* `/foo/ {f=1}` 这条 *rule* 什么都没有省略
	* `f` 这条 *rule* 省略了 *action*, 所以对每个输入行执行 `print` *action*

它们两个的效果也是一样的，都是从第一个匹配 `foo` 的行开始，一直打印到文件结束。

## 正则匹配 (Regex match)

awk 中一个 *rule* 中的 *pattern* 部分可以只是一条 `/` 括起来的正则表达式，默认这个正则表达式的**匹配**对象是 `$0`, 也就是**整行**， 如

```
awk '/foo/'
```

它实际上是

```
awk '$0~/foo/ {print $0}'
```

它意思是只要当前行里包含 `foo` 这个字串，就打印这一行。所以 awk 的正则 *pattern*, 准确来说是用来**搜索**的

默认情况下是 `$0~/foo/`, 那么不默认情况下，可以指定哪个 `field` 去“匹配”， 如

```
awk '$1~/foo/ && $3!~/bar/ {print}'
```
它意思是当前这条记录，如果第1个字段包含 `foo` **且** 第3个字段**不包含** `bar`, 那么就打印整条记录
