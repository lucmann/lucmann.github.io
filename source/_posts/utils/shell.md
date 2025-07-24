---
title: Advanced Shell
date: 2022-06-19 22:18:16
tags: shell
categories: utilities
---

# $SHELL - 日常操作 

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

# awk - 文本处理

Linux 下的文本处理三剑客: grep, sed, awk, 除了它们其实还有一些小巧的命令，如 `tr`, `cut` 也可以帮助我们快速处理和格式化文本。
下面以一个例子为例。

**在 Linux 内核源码目录下，搜索 `drivers/gpu/drm` 下所有的 `DRIVER_NAME` 定义，并排序后格式化输出**

命令如下:

```bash
rg '#define DRIVER_NAME' drivers/gpu/drm --no-heading \
    | tr -s '\t' | tr '\t' ' ' | tr -s ' ' \
    | awk -F':' '{printf("%-52s%-40s\n",$1,$2)}' \
    | sort -k4
```

- `rg` ([ripgrep](https://github.com/BurntSushi/ripgrep)) 比 grep 更快，更强大
- `tr` 在不带任何选项时，默认执行替换，例子是中将 tab 替换成 空格, `-s` 表示 `squeeze-repeats`, 就是去掉重复的字符，例如多个空格只保留一个
- `awk` 天生支持 C-Style printf

输出结果:

- 格式化之前:

```
drivers/gpu/drm/solomon/ssd130x.c:#define DRIVER_NAME	"ssd130x"
drivers/gpu/drm/solomon/ssd130x-i2c.c:#define DRIVER_NAME	"ssd130x-i2c"
drivers/gpu/drm/solomon/ssd130x-spi.c:#define DRIVER_NAME	"ssd130x-spi"
drivers/gpu/drm/exynos/exynos_drm_drv.c:#define DRIVER_NAME	"exynos"
drivers/gpu/drm/bridge/imx/imx8qxp-pixel-link.c:#define DRIVER_NAME		"imx8qxp-display-pixel-link"
drivers/gpu/drm/bridge/imx/imx8qxp-pxl2dpi.c:#define DRIVER_NAME	"imx8qxp-pxl2dpi"
drivers/gpu/drm/bridge/imx/imx8qxp-pixel-combiner.c:#define DRIVER_NAME			"imx8qxp-pixel-combiner"
drivers/gpu/drm/bridge/imx/imx8qxp-ldb-drv.c:#define DRIVER_NAME		"imx8qxp-ldb"
drivers/gpu/drm/bridge/imx/imx8qm-ldb-drv.c:#define DRIVER_NAME			"imx8qm-ldb"
drivers/gpu/drm/bridge/synopsys/dw-hdmi-gp-audio.c:#define DRIVER_NAME "dw-hdmi-gp-audio"
drivers/gpu/drm/bridge/synopsys/dw-hdmi-ahb-audio.c:#define DRIVER_NAME "dw-hdmi-ahb-audio"
drivers/gpu/drm/bridge/synopsys/dw-hdmi-i2s-audio.c:#define DRIVER_NAME "dw-hdmi-i2s-audio"
drivers/gpu/drm/meson/meson_dw_hdmi.c:#define DRIVER_NAME "meson-dw-hdmi"
drivers/gpu/drm/meson/meson_drv.c:#define DRIVER_NAME "meson"
drivers/gpu/drm/omapdrm/omap_drv.c:#define DRIVER_NAME		MODULE_NAME
drivers/gpu/drm/sprd/sprd_drm.c:#define DRIVER_NAME	"sprd"
drivers/gpu/drm/savage/savage_drv.h:#define DRIVER_NAME	"savage"
drivers/gpu/drm/i810/i810_drv.h:#define DRIVER_NAME		"i810"
drivers/gpu/drm/mediatek/mtk_drm_drv.c:#define DRIVER_NAME "mediatek"
drivers/gpu/drm/mga/mga_drv.h:#define DRIVER_NAME		"mga"
drivers/gpu/drm/amd/amdgpu/amdgpu_drv.h:#define DRIVER_NAME		"amdgpu"
drivers/gpu/drm/vboxvideo/vbox_drv.h:#define DRIVER_NAME         "vboxvideo"
drivers/gpu/drm/vkms/vkms_drv.c:#define DRIVER_NAME	"vkms"
drivers/gpu/drm/via/via_dri1.c:#define DRIVER_NAME		"via"
drivers/gpu/drm/sti/sti_drv.c:#define DRIVER_NAME	"sti"
drivers/gpu/drm/imx/ipuv3/imx-ldb.c:#define DRIVER_NAME "imx-ldb"
drivers/gpu/drm/udl/udl_drv.h:#define DRIVER_NAME		"udl"
drivers/gpu/drm/nouveau/nouveau_drv.h:#define DRIVER_NAME		"nouveau"
drivers/gpu/drm/mgag200/mgag200_drv.h:#define DRIVER_NAME		"mgag200"
drivers/gpu/drm/vgem/vgem_drv.c:#define DRIVER_NAME	"vgem"
drivers/gpu/drm/qxl/qxl_drv.h:#define DRIVER_NAME		"qxl"
drivers/gpu/drm/radeon/radeon_drv.h:#define DRIVER_NAME		"radeon"
drivers/gpu/drm/rockchip/rockchip_drm_drv.c:#define DRIVER_NAME	"rockchip"
drivers/gpu/drm/virtio/virtgpu_drv.h:#define DRIVER_NAME "virtio_gpu"
drivers/gpu/drm/tiny/gm12u320.c:#define DRIVER_NAME		"gm12u320"
drivers/gpu/drm/tiny/simpledrm.c:#define DRIVER_NAME	"simpledrm"
drivers/gpu/drm/tiny/ofdrm.c:#define DRIVER_NAME	"ofdrm"
drivers/gpu/drm/v3d/v3d_drv.c:#define DRIVER_NAME "v3d"
drivers/gpu/drm/gma500/psb_drv.h:#define DRIVER_NAME "gma500"
drivers/gpu/drm/hyperv/hyperv_drm_drv.c:#define DRIVER_NAME "hyperv_drm"
drivers/gpu/drm/sis/sis_drv.h:#define DRIVER_NAME		"sis"
drivers/gpu/drm/tdfx/tdfx_drv.h:#define DRIVER_NAME		"tdfx"
drivers/gpu/drm/i915/i915_driver.h:#define DRIVER_NAME		"i915"
drivers/gpu/drm/r128/r128_drv.h:#define DRIVER_NAME		"r128"
drivers/gpu/drm/tiny/cirrus.c:#define DRIVER_NAME "cirrus"
drivers/gpu/drm/tegra/drm.c:#define DRIVER_NAME "tegra"
drivers/gpu/drm/ast/ast_drv.h:#define DRIVER_NAME		"ast"
drivers/gpu/drm/vc4/vc4_drv.c:#define DRIVER_NAME "vc4"
```

- 格式化之后:

```
drivers/gpu/drm/amd/amdgpu/amdgpu_drv.h             #define DRIVER_NAME "amdgpu"            
drivers/gpu/drm/ast/ast_drv.h                       #define DRIVER_NAME "ast"               
drivers/gpu/drm/tiny/cirrus.c                       #define DRIVER_NAME "cirrus"            
drivers/gpu/drm/bridge/synopsys/dw-hdmi-ahb-audio.c #define DRIVER_NAME "dw-hdmi-ahb-audio" 
drivers/gpu/drm/bridge/synopsys/dw-hdmi-gp-audio.c  #define DRIVER_NAME "dw-hdmi-gp-audio"  
drivers/gpu/drm/bridge/synopsys/dw-hdmi-i2s-audio.c #define DRIVER_NAME "dw-hdmi-i2s-audio" 
drivers/gpu/drm/exynos/exynos_drm_drv.c             #define DRIVER_NAME "exynos"            
drivers/gpu/drm/tiny/gm12u320.c                     #define DRIVER_NAME "gm12u320"          
drivers/gpu/drm/gma500/psb_drv.h                    #define DRIVER_NAME "gma500"            
drivers/gpu/drm/hyperv/hyperv_drm_drv.c             #define DRIVER_NAME "hyperv_drm"        
drivers/gpu/drm/i810/i810_drv.h                     #define DRIVER_NAME "i810"              
drivers/gpu/drm/i915/i915_driver.h                  #define DRIVER_NAME "i915"              
drivers/gpu/drm/imx/ipuv3/imx-ldb.c                 #define DRIVER_NAME "imx-ldb"           
drivers/gpu/drm/bridge/imx/imx8qm-ldb-drv.c         #define DRIVER_NAME "imx8qm-ldb"        
drivers/gpu/drm/bridge/imx/imx8qxp-pixel-link.c     #define DRIVER_NAME "imx8qxp-display-pixel-link"
drivers/gpu/drm/bridge/imx/imx8qxp-ldb-drv.c        #define DRIVER_NAME "imx8qxp-ldb"       
drivers/gpu/drm/bridge/imx/imx8qxp-pixel-combiner.c #define DRIVER_NAME "imx8qxp-pixel-combiner"
drivers/gpu/drm/bridge/imx/imx8qxp-pxl2dpi.c        #define DRIVER_NAME "imx8qxp-pxl2dpi"   
drivers/gpu/drm/mediatek/mtk_drm_drv.c              #define DRIVER_NAME "mediatek"          
drivers/gpu/drm/meson/meson_drv.c                   #define DRIVER_NAME "meson"             
drivers/gpu/drm/meson/meson_dw_hdmi.c               #define DRIVER_NAME "meson-dw-hdmi"     
drivers/gpu/drm/mga/mga_drv.h                       #define DRIVER_NAME "mga"               
drivers/gpu/drm/mgag200/mgag200_drv.h               #define DRIVER_NAME "mgag200"           
drivers/gpu/drm/nouveau/nouveau_drv.h               #define DRIVER_NAME "nouveau"           
drivers/gpu/drm/tiny/ofdrm.c                        #define DRIVER_NAME "ofdrm"             
drivers/gpu/drm/qxl/qxl_drv.h                       #define DRIVER_NAME "qxl"               
drivers/gpu/drm/r128/r128_drv.h                     #define DRIVER_NAME "r128"              
drivers/gpu/drm/radeon/radeon_drv.h                 #define DRIVER_NAME "radeon"            
drivers/gpu/drm/rockchip/rockchip_drm_drv.c         #define DRIVER_NAME "rockchip"          
drivers/gpu/drm/savage/savage_drv.h                 #define DRIVER_NAME "savage"            
drivers/gpu/drm/tiny/simpledrm.c                    #define DRIVER_NAME "simpledrm"         
drivers/gpu/drm/sis/sis_drv.h                       #define DRIVER_NAME "sis"               
drivers/gpu/drm/sprd/sprd_drm.c                     #define DRIVER_NAME "sprd"              
drivers/gpu/drm/solomon/ssd130x.c                   #define DRIVER_NAME "ssd130x"           
drivers/gpu/drm/solomon/ssd130x-i2c.c               #define DRIVER_NAME "ssd130x-i2c"       
drivers/gpu/drm/solomon/ssd130x-spi.c               #define DRIVER_NAME "ssd130x-spi"       
drivers/gpu/drm/sti/sti_drv.c                       #define DRIVER_NAME "sti"               
drivers/gpu/drm/tdfx/tdfx_drv.h                     #define DRIVER_NAME "tdfx"              
drivers/gpu/drm/tegra/drm.c                         #define DRIVER_NAME "tegra"             
drivers/gpu/drm/udl/udl_drv.h                       #define DRIVER_NAME "udl"               
drivers/gpu/drm/v3d/v3d_drv.c                       #define DRIVER_NAME "v3d"               
drivers/gpu/drm/vboxvideo/vbox_drv.h                #define DRIVER_NAME "vboxvideo"         
drivers/gpu/drm/vc4/vc4_drv.c                       #define DRIVER_NAME "vc4"               
drivers/gpu/drm/vgem/vgem_drv.c                     #define DRIVER_NAME "vgem"              
drivers/gpu/drm/via/via_dri1.c                      #define DRIVER_NAME "via"               
drivers/gpu/drm/virtio/virtgpu_drv.h                #define DRIVER_NAME "virtio_gpu"        
drivers/gpu/drm/vkms/vkms_drv.c                     #define DRIVER_NAME "vkms"              
drivers/gpu/drm/omapdrm/omap_drv.c                  #define DRIVER_NAME MODULE_NAME         
```

# crontab - 定时任务

`crontab -e` (添加定时任务)， 不光可以添加周期性的定时任务，也可以添加开机时一次性任务

```bash
@reboot /home/luc/mystart.sh
```

crontab 是每用户的， 就是说当前用户设定的任务，只有当前用户的权限，所以如果有些情况下任务执行需要 root 权限，就需要切换到 root 用户后 `crontab -e`

# exec

`exec` 命令有 2 个特点：
- 直接覆盖当前进程，就是说进程 PID 不变，但执行的代码被更换了
- 原来的 shell 环境被销毁，这样当前的代码结束后，也就不会返回原 shell(没得返回)， 直接退出

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
