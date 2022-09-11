---
title: LLVM & llvm-project
date: 2022-09-09 15:58:26
tags: LLVM
categories: programming
---

# LLVM & llvm-project

LLVM 是 Low-Level Virtual Machine 的简写，但事实上它与虚拟机关系不大。我们更熟悉它是一套工具链，包括 clang `/'klæŋ/`, lld, lldb 等等。

<!--more-->

llvm-project 是 2003 年开源的。2022 年初，llvm-project 的源码库和 bug tracker 被移到了 [GitHub](https://github.com/llvm/llvm-project). 一开始，LLVM 的社区交流和项目沟通主要方式是 Mailing Lists 和 [IRC](https://lucmann.github.io/misc/english/), 在 2019 年，LLVM 社区转向 [Discourse](https://www.discourse.org/) 这个开源社区交流平台。

# Building LLVM

```
cmake -S llvm -B build -G Ninja \
      -DCMAKE_BUILD_TYPE=Debug  \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=On \
      -DCMAKE_INSTALL_PREFIX=~/.local/llvm \
      -DLLVM_ENABLE_PROJECTS="clang;lld"   \
      -DLLVM_ENABLE_RUNTIMES="libcxx;libcxxabi"
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

即使只是构建 X86 Target，编译过程也很占系统资源，尤其当 `COMPILE_JOBS > 1` 时，OOM killer happened!

```
FAILED: lib/CodeGen/CMakeFiles/LLVMCodeGen.dir/CodeGenPrepare.cpp.o
/usr/bin/c++  -DGTEST_HAS_RTTI=0 -D_DEBUG -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -Ilib/CodeGen -I/home/luc/gh/llvm-project/llvm/lib/CodeGen -Iinclude -I/home/luc/gh/llvm-project/llvm/include -fPIC -fno-semantic-interposition -fvisibility-inlines-hidden -Werror=date-time -Wall -Wextra -Wno-unused-parameter -Wwrite-strings -Wcast-qual -Wno-missing-field-initializers -pedantic -Wno-long-long -Wimplicit-fallthrough -Wno-maybe-uninitialized -Wno-class-memaccess -Wno-redundant-move -Wno-pessimizing-move -Wno-noexcept-type -Wdelete-non-virtual-dtor -Wsuggest-override -Wno-comment -Wmisleading-indentation -fdiagnostics-color -g -fPIC    -fno-exceptions -fno-rtti -std=c++14 -MD -MT lib/CodeGen/CMakeFiles/LLVMCodeGen.dir/CodeGenPrepare.cpp.o -MF lib/CodeGen/CMakeFiles/LLVMCodeGen.dir/CodeGenPrepare.cpp.o.d -o lib/CodeGen/CMakeFiles/LLVMCodeGen.dir/CodeGenPrepare.cpp.o -c /home/luc/gh/llvm-project/llvm/lib/CodeGen/CodeGenPrepare.cpp
c++: fatal error: Killed signal terminated program cc1plus
compilation terminated.
[502/3706] Building CXX object lib/CodeGen/CMakeFiles/LLVMCodeGen.dir/LLVMTargetMachine.cpp.o
ninja: build stopped: subcommand failed.
```

```
[540657.136627] [   8541]  1000  8541      654        0    40960       25             0 sh
[540657.151290] [   8542]  1000  8542     1746        0    49152       50             0 c++
[540657.154864] [   8543]  1000  8543   130904   100511  1081344    19437             0 cc1plus
[540657.159068] [   8545]  1000  8545      654        0    40960       25             0 sh
[540657.160778] [   8546]  1000  8546     1746        1    53248       48             0 c++
[540657.162707] [   8547]  1000  8547   124573   104596  1028096     8343             0 cc1plus
[540657.165039] [   8549]  1000  8549      654        0    45056       24             0 sh
[540657.166994] [   8550]  1000  8550     1746        0    53248       49             0 c++
[540657.168885] [   8551]  1000  8551   122111   106441  1007616     4695             0 cc1plus
[540657.172484] [   8558]  1000  8558      654        0    45056       24             0 sh
[540657.174632] [   8559]  1000  8559     1746        0    49152       49             0 c++
[540657.177954] [   8560]  1000  8560   120940   106252   987136     2969             0 cc1plus
[540657.180532] [   8561]  1000  8561      654        0    45056       25             0 sh
[540657.182656] [   8562]  1000  8562     1746       32    49152       17             0 c++
[540657.185691] [   8563]  1000  8563   109267    96518   909312      922             0 cc1plus
[540657.188119] [   8565]  1000  8565      654       17    40960        1             0 sh
[540657.190289] [   8566]  1000  8566     1746       48    53248        1             0 c++
[540657.201087] [   8567]  1000  8567    89082    77087   729088      226             0 cc1plus
[540657.203247] [   8569]  1000  8569      654       17    45056        1             0 sh
[540657.206951] [   8570]  1000  8570     1746       48    49152        1             0 c++
[540657.209564] [   8571]  1000  8571    92682    80523   765952      301             0 cc1plus
[540657.216663] [   8573]  1000  8573      654       17    45056        1             0 sh
[540657.218833] [   8574]  1000  8574     1746       49    45056        1             0 c++
[540657.223173] [   8575]  1000  8575    81953    70017   671744      225             0 cc1plus
[540657.228410] oom-kill:constraint=CONSTRAINT_NONE,nodemask=(null),cpuset=/,mems_allowed=0,global_oom,task_memcg=/,task=cc1plus,pid=8216,uid=1000
[540657.242256] Out of memory: Killed process 8216 (cc1plus) total-vm:791824kB, anon-rss:130328kB, file-rss:0kB, shmem-rss:0kB, UID:1000 pgtables:1584kB oom_score_adj:0
```

遇到这种情况，除了换高性能机器，也可以考虑让编译和链接都变成单线程的

```
-DLLVM_PARALLEL_COMPILE_JOBS=1 -DLLVM_PARALLEL_LINK_JOBS=1
```

# References

[1][Improving LLVM Infrastructure - Part 1: Mailing lists](https://blog.llvm.org/posts/2022-01-07-moving-to-discourse/)
