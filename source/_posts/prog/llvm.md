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

即使只是构建 X86 Target，编译过程也很占系统资源，尤其当 `COMPILE_JOBS > 1` 时，oom-kill happened!

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

不仅编译器 `cc1plus` 会触发 oom-kill, 链接时 ld 也可能触发 oom-kill

```
[28979.019282] [    195]     0   195      638        2    40960       28             0 init
[28979.020947] [    196]  1000   196     1206        1    45056      637             0 zsh
[28979.022730] [    197]  1000   197    73414        5   290816     2576             0 Xwayland
[28979.024469] [    290]  1000   290     3201        0    65536       96             0 tmux: client
[28979.026134] [    293]  1000   293     3052      121    53248      112             0 tmux
[28979.027407] [    298]  1000   298     3644      236    69632      311             0 tmux: server
[28979.028840] [    542]  1000   542     1202        1    49152      632             0 zsh
[28979.030290] [    645]  1000   645     1202        1    45056      631             0 zsh
[28979.031645] [   1034]  1000  1034     1208        1    45056      641             0 zsh
[28979.033006] [  23315]  1000 23315     1206        1    45056      637             0 zsh
[28979.034279] [  30425]  1000 30425    15813        0   110592      492             0 cmake
[28979.035546] [  30426]  1000 30426    18109     4579   188416    11596             0 ninja
[28979.036793] [  30442]  1000 30442      736        1    45056       92             0 sh
[28979.038095] [  30443]  1000 30443     1800        0    49152      105             0 c++
[28979.039321] [  30444]  1000 30444     1590        1    49152       36             0 collect2
[28979.041184] [  30445]  1000 30445  1037992   799439  8359936   236319             0 ld
[28979.042881] [  30459]  1000 30459      654       19    40960        0             0 sh
[28979.044266] [  30460]  1000 30460     1746       47    49152        0             0 c++
[28979.045621] [  30461]  1000 30461    96922    84529   794624        0             0 cc1plus
[28979.046945] oom-kill:constraint=CONSTRAINT_NONE,nodemask=(null),cpuset=/,mems_allowed=0,global_oom,task_memcg=/,task=ld,pid=30445,uid=1000
[28979.049044] Out of memory: Killed process 30445 (ld) total-vm:4151968kB, anon-rss:3197756kB, file-rss:0kB, shmem-rss:0kB, UID:1000 pgtables:8164kB oom_score_adj:0
```

实际上一般 Linux 系统 (e.g. Ubuntu 20.04), 默认安装有两个链接器

```bash
lrwxrwxrwx 1   19 Oct 20  2021 /usr/bin/ld -> x86_64-linux-gnu-ld
lrwxrwxrwx 1   23 Oct 20  2021 /usr/bin/ld.bfd -> x86_64-linux-gnu-ld.bfd
lrwxrwxrwx 1   24 Oct 20  2021 /usr/bin/ld.gold -> x86_64-linux-gnu-ld.gold
```

其中，

- `ld.bfd` 基于 [Binary File Descriptor library](https://en.wikipedia.org/wiki/Binary_File_Descriptor_library), 也即所谓的 GNU Linker (GLD)
- `ld.gold` 与 `ld.bfd` 不同，它不基于 BFD, 但它比 `ld.bfd` 更快，尤其是在构建大型 C++ 应用时。 

而默认使用的是 `ld.bfd`, 或许让 LLVM 的构建系统去使用 `ld.gold` 可以规避 ld 被 oom-kill 的尴尬

```
-DLLVM_USE_LINKER=gold
```

我们编译了 clang, 以及 C++ Runtimes: libcxx; libcxxabi. 可以看到当 clang 被构建成功后，后面的 libcxx, libcxxabi 都会使用它做为编译器，而链接器仍然是 gold

```
[3548/3706] Performing configure step for 'runtimes'
-- The C compiler identification is Clang 15.0.0
-- The CXX compiler identification is Clang 15.0.0
-- The ASM compiler identification is Clang
-- Found assembler: /home/luc/gh/llvm-project/build/./bin/clang
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Could NOT find LLVM (missing: LLVM_DIR)
-- Could NOT find Clang (missing: Clang_DIR)
-- Performing Test LLVM_RUNTIMES_LINKING_WORKS
-- Performing Test LLVM_RUNTIMES_LINKING_WORKS - Success
-- Performing Test CXX_SUPPORTS_NOSTDLIBXX_FLAG
-- Performing Test CXX_SUPPORTS_NOSTDLIBXX_FLAG - Success
-- Performing Test CXX_SUPPORTS_NOSTDINCXX_FLAG
-- Performing Test CXX_SUPPORTS_NOSTDINCXX_FLAG - Success
-- Linker detection: GNU Gold
-- Performing Test CXX_SUPPORTS_CUSTOM_LINKER
-- Performing Test CXX_SUPPORTS_CUSTOM_LINKER - Success
-- Performing Test SUPPORTS_FVISIBILITY_INLINES_HIDDEN_FLAG
-- Performing Test SUPPORTS_FVISIBILITY_INLINES_HIDDEN_FLAG - Success
-- Using libc++abi testing configuration: /home/luc/gh/llvm-project/libcxxabi/test/configs/llvm-libc++abi-shared.cfg.in
-- Looking for fopen in c
-- Looking for fopen in c - found
-- Looking for __gcc_personality_v0 in gcc_s
-- Looking for __gcc_personality_v0 in gcc_s - found
-- Looking for __aeabi_uldivmod in gcc
-- Looking for __aeabi_uldivmod in gcc - not found
-- Performing Test C_SUPPORTS_COMMENT_LIB_PRAGMA
-- Performing Test C_SUPPORTS_COMMENT_LIB_PRAGMA - Success
-- Looking for dladdr in dl
-- Looking for dladdr in dl - found
-- Looking for pthread_once in pthread
-- Looking for pthread_once in pthread - found
-- Looking for __cxa_thread_atexit_impl in c
-- Looking for __cxa_thread_atexit_impl in c - found
-- Looking for write in System
-- Looking for write in System - not found
-- Performing Test CXX_SUPPORTS_WERROR_EQ_RETURN_TYPE_FLAG
-- Performing Test CXX_SUPPORTS_WERROR_EQ_RETURN_TYPE_FLAG - Success
-- Performing Test CXX_SUPPORTS_W_FLAG
-- Performing Test CXX_SUPPORTS_W_FLAG - Success
-- Performing Test CXX_SUPPORTS_WALL_FLAG
-- Performing Test CXX_SUPPORTS_WALL_FLAG - Success
-- Performing Test CXX_SUPPORTS_WCHAR_SUBSCRIPTS_FLAG
-- Performing Test CXX_SUPPORTS_WCHAR_SUBSCRIPTS_FLAG - Success
-- Performing Test CXX_SUPPORTS_WCONVERSION_FLAG
-- Performing Test CXX_SUPPORTS_WCONVERSION_FLAG - Success
-- Performing Test CXX_SUPPORTS_WMISMATCHED_TAGS_FLAG
-- Performing Test CXX_SUPPORTS_WMISMATCHED_TAGS_FLAG - Success
-- Performing Test CXX_SUPPORTS_WMISSING_BRACES_FLAG
-- Performing Test CXX_SUPPORTS_WMISSING_BRACES_FLAG - Success
-- Performing Test CXX_SUPPORTS_WNEWLINE_EOF_FLAG
-- Performing Test CXX_SUPPORTS_WNEWLINE_EOF_FLAG - Success
-- Performing Test CXX_SUPPORTS_WUNUSED_FUNCTION_FLAG
-- Performing Test CXX_SUPPORTS_WUNUSED_FUNCTION_FLAG - Success
-- Performing Test CXX_SUPPORTS_WSHADOW_FLAG
-- Performing Test CXX_SUPPORTS_WSHADOW_FLAG - Success
-- Performing Test CXX_SUPPORTS_WSHORTEN_64_TO_32_FLAG
-- Performing Test CXX_SUPPORTS_WSHORTEN_64_TO_32_FLAG - Success
-- Performing Test CXX_SUPPORTS_WSIGN_COMPARE_FLAG
-- Performing Test CXX_SUPPORTS_WSIGN_COMPARE_FLAG - Success
-- Performing Test CXX_SUPPORTS_WSIGN_CONVERSION_FLAG
-- Performing Test CXX_SUPPORTS_WSIGN_CONVERSION_FLAG - Success
-- Performing Test CXX_SUPPORTS_WSTRICT_ALIASING_EQ_2_FLAG
-- Performing Test CXX_SUPPORTS_WSTRICT_ALIASING_EQ_2_FLAG - Success
-- Performing Test CXX_SUPPORTS_WSTRICT_OVERFLOW_EQ_4_FLAG
-- Performing Test CXX_SUPPORTS_WSTRICT_OVERFLOW_EQ_4_FLAG - Success
-- Performing Test CXX_SUPPORTS_WUNUSED_PARAMETER_FLAG
-- Performing Test CXX_SUPPORTS_WUNUSED_PARAMETER_FLAG - Success
-- Performing Test CXX_SUPPORTS_WUNUSED_VARIABLE_FLAG
-- Performing Test CXX_SUPPORTS_WUNUSED_VARIABLE_FLAG - Success
-- Performing Test CXX_SUPPORTS_WWRITE_STRINGS_FLAG
-- Performing Test CXX_SUPPORTS_WWRITE_STRINGS_FLAG - Success
-- Performing Test CXX_SUPPORTS_WUNDEF_FLAG
-- Performing Test CXX_SUPPORTS_WUNDEF_FLAG - Success
-- Performing Test CXX_SUPPORTS_WNO_SUGGEST_OVERRIDE_FLAG
-- Performing Test CXX_SUPPORTS_WNO_SUGGEST_OVERRIDE_FLAG - Success
-- Performing Test CXX_SUPPORTS_WNO_ERROR_FLAG
-- Performing Test CXX_SUPPORTS_WNO_ERROR_FLAG - Success
-- Performing Test CXX_SUPPORTS_WX_FLAG
-- Performing Test CXX_SUPPORTS_WX_FLAG - Failed
-- Performing Test CXX_SUPPORTS_PEDANTIC_FLAG
-- Performing Test CXX_SUPPORTS_PEDANTIC_FLAG - Success
-- Performing Test CXX_SUPPORTS_FSTRICT_ALIASING_FLAG
-- Performing Test CXX_SUPPORTS_FSTRICT_ALIASING_FLAG - Success
-- Performing Test CXX_SUPPORTS_EHSC_FLAG
-- Performing Test CXX_SUPPORTS_EHSC_FLAG - Failed
-- Performing Test C_SUPPORTS_FUNWIND_TABLES_FLAG
-- Performing Test C_SUPPORTS_FUNWIND_TABLES_FLAG - Success
-- Could not find ParallelSTL, libc++abi will not attempt to use it but the build may fail if the libc++ in use needs it to be available.
-- Using libc++ testing configuration: /home/luc/gh/llvm-project/libcxx/test/configs/llvm-libc++-shared.cfg.in
-- Performing Test CXX_SUPPORTS_UNWINDLIB_EQ_NONE_FLAG
-- Performing Test CXX_SUPPORTS_UNWINDLIB_EQ_NONE_FLAG - Success
-- Looking for fopen in c
-- Looking for fopen in c - found
-- Looking for __gcc_personality_v0 in gcc_s
-- Looking for __gcc_personality_v0 in gcc_s - found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
-- Looking for ccos in m
-- Looking for ccos in m - found
-- Looking for clock_gettime in rt
-- Looking for clock_gettime in rt - found
-- Looking for __atomic_fetch_add_8 in atomic
-- Looking for __atomic_fetch_add_8 in atomic - found
-- Performing Test CXX_SUPPORTS_FALIGNED_ALLOCATION_FLAG
-- Performing Test CXX_SUPPORTS_FALIGNED_ALLOCATION_FLAG - Success
-- Performing Test CXX_SUPPORTS_FVISIBILITY_INLINES_HIDDEN_FLAG
-- Performing Test CXX_SUPPORTS_FVISIBILITY_INLINES_HIDDEN_FLAG - Success
-- Performing Test CXX_SUPPORTS_FVISIBILITY_EQ_HIDDEN_FLAG
-- Performing Test CXX_SUPPORTS_FVISIBILITY_EQ_HIDDEN_FLAG - Success
-- Performing Test CXX_SUPPORTS_WEXTRA_FLAG
-- Performing Test CXX_SUPPORTS_WEXTRA_FLAG - Success
-- Performing Test CXX_SUPPORTS_WNO_UNUSED_PARAMETER_FLAG
-- Performing Test CXX_SUPPORTS_WNO_UNUSED_PARAMETER_FLAG - Success
-- Performing Test CXX_SUPPORTS_WNO_LONG_LONG_FLAG
-- Performing Test CXX_SUPPORTS_WNO_LONG_LONG_FLAG - Success
-- Performing Test CXX_SUPPORTS_WEXTRA_SEMI_FLAG
-- Performing Test CXX_SUPPORTS_WEXTRA_SEMI_FLAG - Success
-- Performing Test CXX_SUPPORTS_WFORMAT_NONLITERAL_FLAG
-- Performing Test CXX_SUPPORTS_WFORMAT_NONLITERAL_FLAG - Success
-- Performing Test CXX_SUPPORTS_WNO_USER_DEFINED_LITERALS_FLAG
-- Performing Test CXX_SUPPORTS_WNO_USER_DEFINED_LITERALS_FLAG - Success
-- Performing Test CXX_SUPPORTS_WNO_COVERED_SWITCH_DEFAULT_FLAG
-- Performing Test CXX_SUPPORTS_WNO_COVERED_SWITCH_DEFAULT_FLAG - Success
CMake Warning at /home/luc/gh/llvm-project/libcxx/benchmarks/CMakeLists.txt:2 (message):
  The libc++ benchmarks won't be available because the version of CMake is
  too old to support them.


-- Configuring done
-- Generating done
CMake Warning:
  Manually-specified variables were not used by the project:

    COMPILER_RT_BUILD_BUILTINS
    LLVM_BUILD_TOOLS
    LLVM_CONFIG_PATH
    LLVM_ENABLE_PROJECTS_USED
    PACKAGE_VERSION


-- Build files have been written to: /home/luc/gh/llvm-project/build/runtimes/runtimes-bins
[3556/3706] Performing build step for 'runtimes'
[854/854] Linking CXX static library lib/x86_64-unknown-linux-gnu/libc++experimental.a
[3612/3706] No install step for 'runtimes'
[3706/3706] Creating library symlink lib64/libbenchmark_main.so.1 lib64/libbenchmark_main.so
```

安装后，我们需要将 `bin` 目录加入 `PATH`, 并创建 `/etc/ld.so.conf.d/llvm.conf` 包含下面一行

```
/home/luc/.local/llvm/lib64
```

```
-- Up-to-date: /home/luc/.local/llvm/bin/lld
-- Creating lld-link
-- Creating ld.lld
-- Creating ld64.lld
-- Creating wasm-ld
-- Installing: /home/luc/.local/llvm/lib64/liblldCOFF.so.15git
-- Installing: /home/luc/.local/llvm/lib64/liblldCOFF.so
-- Installing: /home/luc/.local/llvm/lib64/liblldELF.so.15git
-- Installing: /home/luc/.local/llvm/lib64/liblldELF.so
-- Installing: /home/luc/.local/llvm/lib64/liblldMachO.so.15git
-- Installing: /home/luc/.local/llvm/lib64/liblldMachO.so
-- Installing: /home/luc/.local/llvm/lib64/liblldMinGW.so.15git
-- Installing: /home/luc/.local/llvm/lib64/liblldMinGW.so
-- Installing: /home/luc/.local/llvm/lib64/liblldWasm.so.15git
-- Installing: /home/luc/.local/llvm/lib64/liblldWasm.so
-- Installing: /home/luc/.local/llvm/lib64/cmake/lld/LLDTargets.cmake
-- Installing: /home/luc/.local/llvm/lib64/cmake/lld/LLDTargets-debug.cmake
-- Installing: /home/luc/.local/llvm/lib64/cmake/lld/LLDConfig.cmake
-- Installing: /home/luc/.local/llvm/bin/bugpoint
-- Installing: /home/luc/.local/llvm/bin/dsymutil
-- Installing: /home/luc/.local/llvm/bin/llc
-- Installing: /home/luc/.local/llvm/bin/lli
-- Installing: /home/luc/.local/llvm/bin/llvm-as
-- Installing: /home/luc/.local/llvm/bin/llvm-bcanalyzer
-- Installing: /home/luc/.local/llvm/bin/llvm-c-test
-- Installing: /home/luc/.local/llvm/bin/llvm-cat
-- Installing: /home/luc/.local/llvm/bin/llvm-cfi-verify
-- Installing: /home/luc/.local/llvm/lib64/libLLVMCFIVerify.so.15git
-- Installing: /home/luc/.local/llvm/lib64/libLLVMCFIVerify.so
```

```
➜  ~ clang --version
clang version 15.0.0 (git@github.com:lucmann/llvm-project.git b7d09557f6efcf47a905a0e12edf63162dd5e85f)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /home/luc/.local/llvm/bin
```

在编译安装完 LLVM 后，就可以会完成真正想要做的，Building Mesa

# Building Mesa llvmpipe and lavapipe

```
Checking for function "clock_gettime" : YES
Found pkg-config: /usr/bin/pkg-config (0.29.1)
Run-time dependency zlib found: YES 1.2.11
Found CMake: /usr/bin/cmake (3.16.3)
Run-time dependency libzstd found: NO (tried pkgconfig and cmake)
Run-time dependency threads found: YES
Checking for function "pthread_setaffinity_np" with dependency threads: YES
Run-time dependency expat found: YES 2.2.9
Library m found: YES
Run-time dependency libdrm found: YES 2.4.109
Run-time dependency libudev found: YES 245
llvm-config found: YES (/home/luc/.local/llvm/bin/llvm-config) 15.0.0
Run-time dependency LLVM (modules: bitwriter, core, engine, executionengine, instcombine, mcdisassembler, mcjit, native, scalaropts, transformutils, coroutines) found: YES 15.0.0
Run-time dependency valgrind found: NO (tried pkgconfig)
Program bison found: YES (/usr/bin/bison)
Program bison found: YES (/usr/bin/bison)
Program flex found: YES (/usr/bin/flex)
Run-time dependency libunwind found: NO (tried pkgconfig and cmake)
Run-time dependency OpenMP found: YES 4.5
Run-time dependency x11 found: YES 1.6.9
Run-time dependency xext found: YES 1.3.4
Run-time dependency xfixes found: YES 5.0.3
Run-time dependency xcb-glx found: NO (tried pkgconfig and cmake)
```

# References

[1][Improving LLVM Infrastructure - Part 1: Mailing lists](https://blog.llvm.org/posts/2022-01-07-moving-to-discourse/)
[2][ld.gold使用指南](https://blog.csdn.net/ayu_ag/article/details/78552801)
