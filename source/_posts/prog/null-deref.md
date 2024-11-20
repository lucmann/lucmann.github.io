---
title: 空指针解引用一定段错误吗？
date: 2024-11-20 16:07:50
tags: [C/C++]
categories: programming
---

# Segfault ?

下面的程序会段错误吗？

```c
#include <stdio.h>

struct bar {
    int a;
};

struct foo {
    struct bar b;
};

void func(struct foo *pf)
{
    printf("%p\n", &pf->b);
}

int main(int argc, char *argv[])
{
    struct foo *f = NULL;

    printf("%p\n", &f->b);

    func(f);
}
```

<!--more-->

# Segfault

段错误是指程序试图**访问一个不存在或没有权限访问的内存位置**时操作系统发出的信号，一般都会使程序中止。所以上面的程序是不是会段错误，就要看 CPU 是不是**访问**了 `f (NULL)` 这个 0 地址。

## MOV vs LEA
