---
title: LLVM & llvm-project
date: 2022-09-09 15:58:26
tags: LLVM
categories: programming
---

# LLVM

LLVM 是 Low-Level Virtual Machine 的简写，但事实上它与虚拟机关系不大。我们更熟悉它是一套工具链，包括 clang `/'klæŋ/`, lld, lldb 等等。

# Building LLVM

```
cmake -S llvm -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=On -DCMAKE_INSTALL_PREFIX=~/.local/llvm -DLLVM_ENABLE_PROJECTS="clang;lld" -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi"
```

如果是这样构建，会出现多种 `Targets`, 也就是 LLVM 支持的后端或者说 ISA, 但是你真的需要这么多吗？这会减慢构建速度，况且编译 LLVM 本身就很慢

```
-- Linker detection: GNU ld
-- Performing Test HAS_WERROR_GLOBAL_CTORS
-- Performing Test HAS_WERROR_GLOBAL_CTORS - Failed
-- Looking for __x86_64__
-- Looking for __x86_64__ - found
-- Looking for __aarch64__
-- Looking for __aarch64__ - not found
-- Found Git: /usr/bin/git (found version "2.25.1")
-- Targeting AArch64
-- Targeting AMDGPU
-- Targeting ARM
-- Targeting AVR
-- Targeting BPF
-- Targeting Hexagon
-- Targeting Lanai
-- Targeting Mips
-- Targeting MSP430
-- Targeting NVPTX
-- Targeting PowerPC
-- Targeting RISCV
-- Targeting Sparc
-- Targeting SystemZ
-- Targeting VE
-- Targeting WebAssembly
-- Targeting X86
-- Targeting XCore
```

可以考虑在构建配置加上

```
-DLLVM_TARGETS_TO_BUILD="host"
```

此外，如果你是 64 位系统，考虑加上

```
-DLLVM_LIBDIR_SUFFIX="64"
```

如果想以动态库形式构建，加上

```
-DLLVM_BUILD_LLVM_DYLIB=On -DBUILD_SHARED_LIBS=On
```

现在看看配置结果

```
-- Linker detection: GNU ld
-- Performing Test HAS_WERROR_GLOBAL_CTORS
-- Performing Test HAS_WERROR_GLOBAL_CTORS - Failed
-- Looking for __x86_64__
-- Looking for __x86_64__ - found
-- Looking for __aarch64__
-- Looking for __aarch64__ - not found
-- Found Git: /usr/bin/git (found version "2.25.1")
-- Targeting X86
-- Looking for sys/resource.h
-- Looking for sys/resource.h - found
-- Clang version: 15.0.0
```
