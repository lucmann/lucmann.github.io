---
title: Vulkan-Samples
date: 2022-09-18 15:58:26
tags: vulkan
categories: graphics
---

# [KhronosGroup/Vulkan-Samples](https://github.com/KhronosGroup/Vulkan-Samples) with lavapipe

解决 LLVM 和 Mesa 的构建问题后，我们可以顺利构建并运行 Vulkan-Samples 的 `hello_triangle`

<!--more-->

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
