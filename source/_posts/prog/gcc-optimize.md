---
title: gcc 优化选项
date: 2023-11-06 15:58:26
tags: gcc
categories: programming
---

# -O1 vs -Og

-O 和 -O1 是等价的。 -O 会对目标文件大小和执行时间进行优化，但不会进行非常耗时的优化，以下是 -O 打开的优化: (其中红色的是 -Og 关闭的)

- -fauto-inc-dec
- <span style="color:red">-fbranch-count-reg</span>
- -fcombine-stack-adjustments
- -fcompare-elim
- -fcprop-registers
- -fdce
- -fdefer-pop
- <span style="color:red">-fdelayed-branch</span>
- <span style="color:red">-fdse</span>
- -fforward-propagate
- -fguess-branch-probability
- <span style="color:red">-fif-conversion</span>
- <span style="color:red">-fif-conversion2</span>
- <span style="color:red">-finline-functions-called-once</span>
- -fipa-modref
- -fipa-profile
- -fipa-reference
- -fipa-reference-addressable
- -fmerge-constants
- <span style="color:red">-fmove-loop-invariants</span>
- <span style="color:red">-fmove-loop-stores</span>
- -fomit-frame-pointer
- -freorder-blocks
- -fshrink-wrap
- -fshrink-wrap-separate
- -fsplit-wide-types
- -fssa-backprop
- <span style="color:red">-fssa-phiopt</span>
- <span style="color:red">-ftree-bit-ccp</span>
- -ftree-ccp
- -ftree-ch
- -ftree-coalesce-vars
- -ftree-copy-prop
- -ftree-dce
- -ftree-dominator-opts
- <span style="color:red">-ftree-dse</span>
- -ftree-forwprop
- -ftree-fre
- -ftree-phiprop
- <span style="color:red">-ftree-pta</span>
- -ftree-scev-cprop
- -ftree-sink
- -ftree-slsr
- <span style="color:red">-ftree-sra</span>
- -ftree-ter
- -funit-at-a-time

