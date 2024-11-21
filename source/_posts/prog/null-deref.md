---
title: "LEA: Load Effective Address"
date: 2024-11-20 16:07:50
tags: [C/C++]
categories: programming
---

# Segfault ?

下面的程序会段错误吗？

```c
#include <stdio.h>

struct foo {
    long a;
};

int main(int argc, char *argv[])
{
    struct foo *f = NULL;

    printf("0x%016lx\n", &f->a);
}
```

<!--more-->

# Segfault

段错误是指程序试图**访问一个不存在或没有权限访问的内存位置**时操作系统发出的信号，一般都会使程序中止。如果上面的程序的 printf 改为

```c
    printf("0x%016lx\n", f->a);
```

则一定段错误，看看 CPU 执行的指令

```asm
0000000000001149 <main>:
    1149:       f3 0f 1e fa             endbr64
    114d:       55                      push   %rbp
    114e:       48 89 e5                mov    %rsp,%rbp
    1151:       48 83 ec 20             sub    $0x20,%rsp
    1155:       89 7d ec                mov    %edi,-0x14(%rbp)
    1158:       48 89 75 e0             mov    %rsi,-0x20(%rbp)
    115c:       48 c7 45 f8 00 00 00    movq   $0x0,-0x8(%rbp)
    1163:       00
    1164:       48 8b 45 f8             mov    -0x8(%rbp),%rax
    1168:       48 8b 00                mov    (%rax),%rax
    116b:       48 89 c6                mov    %rax,%rsi
    116e:       48 8d 05 8f 0e 00 00    lea    0xe8f(%rip),%rax        # 2004 <_IO_stdin_used+0x4>
    1175:       48 89 c7                mov    %rax,%rdi
    1178:       b8 00 00 00 00          mov    $0x0,%eax
    117d:       e8 ce fe ff ff          call   1050 <printf@plt>
    1182:       b8 00 00 00 00          mov    $0x0,%eax
    1187:       c9                      leave
    1188:       c3                      ret
```
触发段错误的是第 11 行的 `mov (%rax),%rax`, 因为 `rax` 寄存器的值是 0， 这条 MOV 指令里的 `(%rax)` 实际上就是去内存地址 0 取数据。再看看上面不会段错误的程序的指令

```asm
0000000000001149 <main>:
    1149:       f3 0f 1e fa             endbr64
    114d:       55                      push   %rbp
    114e:       48 89 e5                mov    %rsp,%rbp
    1151:       48 83 ec 20             sub    $0x20,%rsp
    1155:       89 7d ec                mov    %edi,-0x14(%rbp)
    1158:       48 89 75 e0             mov    %rsi,-0x20(%rbp)
    115c:       48 c7 45 f8 00 00 00    movq   $0x0,-0x8(%rbp)
    1163:       00
    1164:       48 8b 45 f8             mov    -0x8(%rbp),%rax
    1168:       48 89 c6                mov    %rax,%rsi
    116b:       48 8d 05 92 0e 00 00    lea    0xe92(%rip),%rax        # 2004 <_IO_stdin_used+0x4>
    1172:       48 89 c7                mov    %rax,%rdi
    1175:       b8 00 00 00 00          mov    $0x0,%eax
    117a:       e8 d1 fe ff ff          call   1050 <printf@plt>
    117f:       b8 00 00 00 00          mov    $0x0,%eax
    1184:       c9                      leave
    1185:       c3                      ret
```

原来的第 11 行 `mov (%rax),%rax` 变成了

```asm
    1168:       48 89 c6                mov    %rax,%rsi
    116b:       48 8d 05 92 0e 00 00    lea    0xe92(%rip),%rax        # 2004 <_IO_stdin_used+0x4>
```

它并没有直接去内存地址 0 去取数据，而是将指令寄存器 `rip` 的当前值加上 `0xe92` 后将结果存入 `rax` 寄存器。

所以上面的程序是不是会段错误，就要看 CPU 是不是**访问**了 `f (NULL)` 这个 0 地址。

## MOV vs LEA

# References
- [What's the purpose of the LEA instruction?](https://stackoverflow.com/questions/1658294/whats-the-purpose-of-the-lea-instruction)
- [Understanding the x86 code models](https://eli.thegreenplace.net/2012/01/03/understanding-the-x64-code-models)
