---
title: git 学习笔记
date: 2021-01-25 20:42:05
tags: git
categories: utilities
---

# git-submodule

当克隆一个含有子模块的 git 仓库时，有 2 种方法可以连同它的子模块一起克隆下来。

<!--more-->

第一种方法是在克隆时，直接指示 git 将所有子模块都克隆下来

```
git clone --recurse-submodules
```

第二种方法是在克隆后，使用 `git submodule` 子命令再将所有子模块初始化，克隆下来

```
git submodule init
git submodule update
```

上面命令可以合并为一个命令

```
git submodule update --init
```

## 与子模块相关的 git 配置文件和目录

- `.gitmodules`

    记录了子模块的名字（路径）和 URL

- `.git/config`

    克隆子模块后，它的路径和 URL 会记录上这个文件中

- `.git/modules`

    克隆子模块后，它的 Index 会保存在这个目录

## 克隆时出现未完全克隆的子模块

`git clone --recurse-submodules` 对于子模块的拉取分为 3 步

- register, 也就是初始化
- clone, 提取所有代码
- checkout, 检出上层项目中记录的合适提交

```
➜  gh gcl git@github.com:KhronosGroup/Vulkan-Samples.git
Cloning into 'Vulkan-Samples'...
remote: Enumerating objects: 4855, done.
remote: Counting objects: 100% (229/229), done.
remote: Compressing objects: 100% (173/173), done.
remote: Total 4855 (delta 80), reused 136 (delta 42), pack-reused 4626
Receiving objects: 100% (4855/4855), 40.22 MiB | 3.97 MiB/s, done.
Resolving deltas: 100% (3048/3048), done.
Submodule 'assets' (https://github.com/KhronosGroup/Vulkan-Samples-Assets) registered for path 'assets'
Submodule 'third_party/CTPL' (https://github.com/vit-vit/CTPL) registered for path 'third_party/CTPL'
Submodule 'third_party/astc' (https://github.com/ARM-software/astc-encoder) registered for path 'third_party/astc'
Submodule 'third_party/cli11' (https://github.com/CLIUtils/CLI11.git) registered for path 'third_party/cli11'
Submodule 'third_party/glfw' (https://github.com/glfw/glfw) registered for path 'third_party/glfw'
Submodule 'third_party/glm' (https://github.com/g-truc/glm) registered for path 'third_party/glm'
Submodule 'third_party/glslang' (https://github.com/KhronosGroup/glslang) registered for path 'third_party/glslang'
Submodule 'third_party/hwcpipe' (https://github.com/ARM-software/HWCPipe) registered for path 'third_party/hwcpipe'
Submodule 'third_party/imgui' (https://github.com/ocornut/imgui) registered for path 'third_party/imgui'
Submodule 'third_party/ktx' (https://github.com/KhronosGroup/KTX-Software) registered for path 'third_party/ktx'
Submodule 'third_party/opencl' (https://github.com/KhronosGroup/OpenCL-Headers.git) registered for path 'third_party/opencl'
Submodule 'third_party/spdlog' (https://github.com/gabime/spdlog) registered for path 'third_party/spdlog'
Submodule 'third_party/spirv-cross' (https://github.com/KhronosGroup/SPIRV-Cross) registered for path 'third_party/spirv-cross'
Submodule 'third_party/stb' (https://github.com/nothings/stb) registered for path 'third_party/stb'
Submodule 'third_party/tinygltf' (https://github.com/syoyo/tinygltf) registered for path 'third_party/tinygltf'
Submodule 'third_party/vma' (https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) registered for path 'third_party/vma'
Submodule 'third_party/volk' (https://github.com/zeux/volk) registered for path 'third_party/volk'
Submodule 'third_party/vulkan' (https://github.com/KhronosGroup/Vulkan-Headers) registered for path 'third_party/vulkan'
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/CTPL'...
fatal: unable to access 'https://github.com/vit-vit/CTPL/': GnuTLS recv error (-110): The TLS connection was non-properly terminated.
fatal: clone of 'https://github.com/vit-vit/CTPL' into submodule path '/home/luc/gh/Vulkan-Samples/third_party/CTPL' failed
Failed to clone 'third_party/CTPL'. Retry scheduled
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/astc'...
remote: Enumerating objects: 10565, done.
remote: Counting objects: 100% (1165/1165), done.
remote: Compressing objects: 100% (498/498), done.
remote: Total 10565 (delta 829), reused 972 (delta 667), pack-reused 9400
Receiving objects: 100% (10565/10565), 48.43 MiB | 11.07 MiB/s, done.
Resolving deltas: 100% (8036/8036), done.
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/cli11'...
remote: Enumerating objects: 8623, done.
remote: Counting objects: 100% (1400/1400), done.
remote: Compressing objects: 100% (630/630), done.
remote: Total 8623 (delta 936), reused 1073 (delta 694), pack-reused 7223
Receiving objects: 100% (8623/8623), 4.56 MiB | 3.09 MiB/s, done.
Resolving deltas: 100% (5865/5865), done.
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/glfw'...
remote: Enumerating objects: 30428, done.
remote: Counting objects: 100% (49/49), done.
remote: Compressing objects: 100% (39/39), done.
remote: Total 30428 (delta 19), reused 28 (delta 8), pack-reused 30379
Receiving objects: 100% (30428/30428), 14.88 MiB | 5.22 MiB/s, done.
Resolving deltas: 100% (21562/21562), done.
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/glm'...
remote: Enumerating objects: 56725, done.
remote: Counting objects: 100% (435/435), done.
remote: Compressing objects: 100% (185/185), done.
remote: Total 56725 (delta 273), reused 323 (delta 250), pack-reused 56290
Receiving objects: 100% (56725/56725), 69.17 MiB | 2.08 MiB/s, done.
Resolving deltas: 100% (42792/42792), done.
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/glslang'...
remote: Enumerating objects: 51510, done.
remote: Counting objects: 100% (157/157), done.
remote: Compressing objects: 100% (95/95), done.
remote: Total 51510 (delta 76), reused 117 (delta 58), pack-reused 51353
Receiving objects: 100% (51510/51510), 65.58 MiB | 3.88 MiB/s, done.
Resolving deltas: 100% (43266/43266), done.
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/hwcpipe'...
remote: Enumerating objects: 382, done.
remote: Counting objects: 100% (102/102), done.
remote: Compressing objects: 100% (56/56), done.
remote: Total 382 (delta 50), reused 78 (delta 40), pack-reused 280
Receiving objects: 100% (382/382), 332.12 KiB | 57.00 KiB/s, done.
Resolving deltas: 100% (192/192), done.
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/imgui'...
fatal: unable to access 'https://github.com/ocornut/imgui/': GnuTLS recv error (-54): Error in the pull function.
fatal: clone of 'https://github.com/ocornut/imgui' into submodule path '/home/luc/gh/Vulkan-Samples/third_party/imgui' failed
Failed to clone 'third_party/imgui'. Retry scheduled
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/ktx'...
fatal: unable to access 'https://github.com/KhronosGroup/KTX-Software/': GnuTLS recv error (-110): The TLS connection was non-properly terminated.
fatal: clone of 'https://github.com/KhronosGroup/KTX-Software' into submodule path '/home/luc/gh/Vulkan-Samples/third_party/ktx' failed
Failed to clone 'third_party/ktx'. Retry scheduled
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/opencl'...
fatal: unable to access 'https://github.com/KhronosGroup/OpenCL-Headers.git/': GnuTLS recv error (-54): Error in the pull function.
fatal: clone of 'https://github.com/KhronosGroup/OpenCL-Headers.git' into submodule path '/home/luc/gh/Vulkan-Samples/third_party/opencl' failed
Failed to clone 'third_party/opencl'. Retry scheduled
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/spdlog'...
fatal: unable to access 'https://github.com/gabime/spdlog/': GnuTLS recv error (-54): Error in the pull function.
fatal: clone of 'https://github.com/gabime/spdlog' into submodule path '/home/luc/gh/Vulkan-Samples/third_party/spdlog' failed
Failed to clone 'third_party/spdlog'. Retry scheduled
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/spirv-cross'...
remote: Enumerating objects: 31917, done.
remote: Counting objects: 100% (838/838), done.
remote: Compressing objects: 100% (367/367), done.
remote: Total 31917 (delta 507), reused 758 (delta 466), pack-reused 31079
Receiving objects: 100% (31917/31917), 14.56 MiB | 1.34 MiB/s, done.
Resolving deltas: 100% (22523/22523), done.
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/stb'...
remote: Enumerating objects: 7844, done.
remote: Counting objects: 100% (573/573), done.
remote: Compressing objects: 100% (209/209), done.
remote: Total 7844 (delta 372), reused 544 (delta 364), pack-reused 7271
Receiving objects: 100% (7844/7844), 5.45 MiB | 3.33 MiB/s, done.
Resolving deltas: 100% (5217/5217), done.
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/tinygltf'...
remote: Enumerating objects: 4676, done.
remote: Counting objects: 100% (231/231), done.
remote: Compressing objects: 100% (128/128), done.
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/volk'...
fatal: unable to access 'https://github.com/zeux/volk/': GnuTLS recv error (-110): The TLS connection was non-properly terminated.
fatal: clone of 'https://github.com/zeux/volk' into submodule path '/home/luc/gh/Vulkan-Samples/third_party/volk' failed
Failed to clone 'third_party/volk'. Retry scheduled
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/vulkan'...
remote: Enumerating objects: 2910, done.
remote: Counting objects: 100% (1056/1056), done.
remote: Compressing objects: 100% (309/309), done.
remote: Total 2910 (delta 777), reused 942 (delta 733), pack-reused 1854
Receiving objects: 100% (2910/2910), 24.31 MiB | 1.83 MiB/s, done.
Resolving deltas: 100% (1862/1862), done.
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/CTPL'...
remote: Enumerating objects: 82, done.
remote: Counting objects: 100% (27/27), done.
remote: Compressing objects: 100% (4/4), done.
remote: Total 82 (delta 24), reused 23 (delta 23), pack-reused 55
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/imgui'...
remote: Enumerating objects: 45039, done.
remote: Counting objects: 100% (191/191), done.
remote: Compressing objects: 100% (74/74), done.
remote: Total 45039 (delta 138), reused 169 (delta 117), pack-reused 44848
Receiving objects: 100% (45039/45039), 83.37 MiB | 3.75 MiB/s, done.
Resolving deltas: 100% (34322/34322), done.
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/ktx'...
remote: Enumerating objects: 29538, done.
remote: Counting objects: 100% (1183/1183), done.
remote: Compressing objects: 100% (208/208), done.
```

有些子模块经过两次拉取，仍然没有成功，这时已经成功拉取的仓库也没有经过第 3 步检出，所以上层仓库变成 modified 状态

```diff
diff --git a/third_party/astc b/third_party/astc
index 4861c3d..c1e1247 160000
--- a/third_party/astc
+++ b/third_party/astc
@@ -1 +1 @@
-Subproject commit 4861c3db1ca50eb5f5767d95d1cd1bacb98ac8a5
+Subproject commit c1e1247819efb268044db58e05fde400b3c6532f
diff --git a/third_party/cli11 b/third_party/cli11
index 4af78be..faea921 160000
--- a/third_party/cli11
+++ b/third_party/cli11
@@ -1 +1 @@
-Subproject commit 4af78beef777e313814b4daff70e2da9171a385a
+Subproject commit faea921e4004af91763b8fde905de3baf24d3945
diff --git a/third_party/glfw b/third_party/glfw
index 6876cf8..6b57e08 160000
--- a/third_party/glfw
+++ b/third_party/glfw
@@ -1 +1 @@
-Subproject commit 6876cf8d7e0e70dc3e4d7b0224d08312c9f78099
+Subproject commit 6b57e08bb0078c9834889eab871bac2368198c15
diff --git a/third_party/glm b/third_party/glm
index fce2abd..cc98465 160000
--- a/third_party/glm
+++ b/third_party/glm
@@ -1 +1 @@
-Subproject commit fce2abd01ce21063bd25ba67c9318be83bf48813
+Subproject commit cc98465e3508535ba8c7f6208df934c156a018dc
diff --git a/third_party/glslang b/third_party/glslang
index 6baa85f..10423ec 160000
--- a/third_party/glslang
+++ b/third_party/glslang
@@ -1 +1 @@
-Subproject commit 6baa85f6ceec5fddd2e13b43dd1edc944dd930af
+Subproject commit 10423ec659d301a0ff2daac8bbf38980abf27590
```

这时，我们进入上层仓库，再把没有拉取成功的仓库，重新 `git submodule update`, 此时不需要再加 `--init` 选项，因为所有子模块都已经注册成功，可以通过查看 `.git/config` 文件确认

```bash
➜  Vulkan-Samples git:(master) ✗ git submodule update third_party/volk
Cloning into '/home/luc/gh/Vulkan-Samples/third_party/volk'...
Submodule path 'third_party/volk': checked out 'b18f3192886981f28d17f8f41c26c5a53ac3b977'
```

把没有执行检出的子模块重新 `git submodule update`, 检出到上层仓库记录的合适提交

```bash
➜  Vulkan-Samples git:(master) ✗ git submodule update
Submodule path 'third_party/astc': checked out '4861c3db1ca50eb5f5767d95d1cd1bacb98ac8a5'
Submodule path 'third_party/cli11': checked out '4af78beef777e313814b4daff70e2da9171a385a'
Submodule path 'third_party/glfw': checked out '6876cf8d7e0e70dc3e4d7b0224d08312c9f78099'
Submodule path 'third_party/glm': checked out 'fce2abd01ce21063bd25ba67c9318be83bf48813'
Submodule path 'third_party/glslang': checked out '6baa85f6ceec5fddd2e13b43dd1edc944dd930af'
Submodule path 'third_party/hwcpipe': checked out '5a0c3d0b527bea96154998f29177f3b449cebd3a'
Submodule path 'third_party/imgui': checked out 'c23a19c26fa9cc778f755e76799f0cafdcb5a13b'
Submodule path 'third_party/ktx': checked out '6a38a0694b4d73d22c5d1e22f865d03545e808ea'
Submodule path 'third_party/opencl': checked out '1aa1139b58a515877a923cce6b254e09d1b2fb2c'
Submodule path 'third_party/spdlog': checked out '7bf8f148795d2571b4ae4aa9c6d2a8111fbb7077'
Submodule path 'third_party/spirv-cross': checked out '84f56d0b43e5a6874de45e11be2bff9dc336fd7e'
Submodule path 'third_party/stb': checked out '2c2908f50515dcd939f24be261c3ccbcd277bb49'
Submodule path 'third_party/tinygltf': checked out 'c0d0251e2c7f2273a894aa1b125af9313a88eae4'
Submodule path 'third_party/vma': checked out 'f9921aefddee2437cc2e3303d3175bd8ef23e22c'
Submodule path 'third_party/vulkan': checked out '76f00ef6cbb1886eb1162d1fa39bee8b51e22ee8'
```

# gitignore

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
