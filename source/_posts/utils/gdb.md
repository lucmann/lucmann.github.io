---
title: GNU Debugger - gdb
date: 2024-12-18 15:58:26
tags: [Debugger]
categories: utilities
---

# [GDB Cheat Sheet](https://darkdust.net/files/GDB%20Cheat%20Sheet.pdf)

<!--more-->

# GDB Init File

GDB Init File 包含一组 gdb 命令，每次启动 gdb 时自动执行，这个文件可以有 3 个位置

- `~/.config/gdb/gdbinit`
- `~/.gdbinit`
- `./.gdbinit`

# GDB Script File

将下面的 gdb 命令写入一个 script 文件里 `gdbscript.txt`，执行 `gdb glxgears -x gdbscript.txt`, gdb 会自动执行这个 script 文件里的命令。

```
# Avoid keyboard input
set pagination off

# Save log to file
set logging file llvmpipe.log
set logging enabled on

# Avoid prompt that "Make breakpoint pending on future shared library load?"
set breakpoint pending on

# Add breakpoint at llvmpipe_flush()
break llvmpipe_flush
    command 1
    shell date +"%T.%3N" >> llvmpipe.log # Append a timestamp to log file
    backtrace
    continue
end

# Start the program
run

continue

set logging off
quit

```

P.S. [llvmpipe.log](https://pastebin.com/bCEBrwAW)
