---
title: Vulkan&Zink
date: 2021-04-02 21:21:27
tags: vulkan
categories: graphics
---

# [Zink](https://docs.mesa3d.org/drivers/zink.html)

Zink 不是某个具体 GPU 的驱动，而是一个将OpenGL API 调用翻译成 Vulkan API, 以在只有Vulkan 驱动的平台上提供完整OpenGL支持。

# [Vulkan-Samples](https://github.com/KhronosGroup/Vulkan-Samples)

```
➜  Vulkan-Samples git:(master) ./build/app/bin/Debug/x86_64/vulkan_samples sample hello_triangle
[info] Logger initialized
[info] Initializing vulkan instance.
[info] Enabled Validation Layers:
[info] Found GPU: llvmpipe (LLVM 14.0.5, 256 bits)
[info] Found GPU: llvmpipe (LLVM 14.0.5, 256 bits)
[info] Initializing vulkan device.
WARNING: lavapipe is not a conformant vulkan implementation, testing use only.
```

我们可以获取一个 samples 的列表

```
[info]  id: compute_nbody
[info]  id: dynamic_uniform_buffers
[info]  id: hdr
[info]  id: hello_triangle
[info]  id: hlsl_shaders
[info]  id: hpp_compute_nbody
[info]  id: hpp_dynamic_uniform_buffers
[info]  id: hpp_hdr
[info]  id: hpp_hello_triangle
[info]  id: hpp_instancing
[info]  id: hpp_terrain_tessellation
[info]  id: hpp_texture_loading
[info]  id: instancing
[info]  id: separate_image_sampler
[info]  id: terrain_tessellation
[info]  id: texture_loading
[info]  id: texture_mipmap_generation
[info]  id: buffer_device_address
[info]  id: conditional_rendering
[info]  id: conservative_rasterization
[info]  id: debug_utils
[info]  id: descriptor_indexing
[info]  id: dynamic_rendering
[info]  id: fragment_shading_rate
[info]  id: fragment_shading_rate_dynamic
[info]  id: graphics_pipeline_library
[info]  id: open_gl_interop
[info]  id: portability
[info]  id: push_descriptors
[info]  id: ray_queries
[info]  id: ray_tracing_reflection
[info]  id: raytracing_basic
[info]  id: raytracing_extended
[info]  id: synchronization_2
[info]  id: timeline_semaphore
[info]  id: 16bit_arithmetic
[info]  id: 16bit_storage_input_output
[info]  id: afbc
[info]  id: async_compute
[info]  id: command_buffer_usage
[info]  id: constant_data
[info]  id: descriptor_management
[info]  id: layout_transitions
[info]  id: msaa
[info]  id: multi_draw_indirect
[info]  id: multithreading_render_passes
[info]  id: pipeline_barriers
[info]  id: pipeline_cache
[info]  id: render_passes
[info]  id: specialization_constants
[info]  id: subpasses
[info]  id: surface_rotation
[info]  id: swapchain_images
[info]  id: texture_compression_basisu
[info]  id: texture_compression_comparison
[info]  id: wait_idle
[info]  id: profiles
```

![hello_triangle](hello_triangle.png)
![instancing](instancing.png)
![compute n-body simulation](n-body.png)
![VK_EXT_Debug_Utils](debug_utils.png)
![uniform_buffers](uniform_buffers.png)
![HDR](hdr.png)
![MSAA](msaa.png)
![push descriptors](push_descriptors.png)
![Basic Universal texture compress](texture_compress.png)
![Texture mipmap generation](mipmap_generation.png)

# [Vulkan-Docs](https://github.com/KhronosGroup/Vulkan-Docs)

包含了**Vulkan API**技术文档和相关工具的使用说明。

| Vulkan Version | Last Updated Tag   |
|:--------------:|:-------------------|
| 1.0            | v1.0.69-core       |
| 1.1            | v1.1.130           |
| 1.2            | v1.2.174 (Current) | 

<!--more-->

## Build
### Testing Environment
- Windows Subsystem for Linux 2.0
- Ubuntu 20.04.2 LTS

### Requirement
Vulkan-Docs的构建需要`ruby`环境。主要是因为它用到一个`asciidoctor-mathematical`的工具包, 而它又依赖`mathematical`, 所以正确的安装顺序是

{% codeblock "ruby development environment" %}
apt-get -qq -y install ruby-dev
{% endcodeblock %}

{% codeblock "required libraries by mathematical" %}
apt-get -qq -y install bison flex libffi-dev libxml2-dev libgdk-pixbuf2.0-dev libcairo2-dev libpango1.0-dev
{% endcodeblock %}

{% codeblock %}
gem install asciidoctor-mathematical
{% endcodeblock %}

除此之外，还需要安装

{% codeblock %}
gem install coderay
{% endcodeblock %}

### Scripts
所有的**HTML**，**PDF**都是通过脚本基于`xml/vk.xml`生成的。比方要生成所有**extensions**相关的环境变量需要的脚本是`scripts/make_ext_dependency.py`.

### Targets
Vulkan-Docs的构建系统基本上是`make` + `shell` + `python`. 它的顶层目录包含一个`Makefile`, 几乎包含了所有构建对象，其它的构建脚本基本上是`make VAR="$VALUE" TARGET $*`的调用形式。 这个Makefile里定义的目标大概有以下这些:

- all
- **allchecks**
- alldocs
- allman
- allspecs
- apiinc
- buildmanpages
- chunked
- clean
- clean_checks
- clean_generated
- clean_html
- clean_man
- clean_pdf
- clean_validusage
- config/extDependency.sh
- diff_html
- extinc
- generated
- hostsyncinc
- **html**
- katexinst
- manhtml
- **manhtmlpages**
- manpdf
- **pdf**
- reflow
- registry
- **styleguide**
- validinc
- validusage

