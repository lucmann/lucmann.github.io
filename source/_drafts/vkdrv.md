---
title: Vulkan Driver 的实现
date: 2024-11-22 11:21:27
tags: Vulkan
categories: graphics
---

# dispatch table

mesa 代码库中使用 [`src/vulkan/util/vk_dispatch_table_gen.py`](https://gitlab.freedesktop.org/mesa/mesa/-/blob/main/src/vulkan/util/vk_dispatch_table_gen.py) 生成 vulkan 各种 **dispatch tables**

- vk_instance_dispatch_table
- vk_physical_device_dispatch_table
- vk_device_dispatch_table

这个脚本的入参和输出分别是 

- 参数:
    - `src/vulkan/registry/vk.xml`
- 输出:
    - `vk_dispatch_table.h`
    - `vk_dispatch_table.c`

```python src/vulkan/util/meson.build
vk_dispatch_table = custom_target(
  'vk_dispatch_table',
  input : ['vk_dispatch_table_gen.py', vk_api_xml],
  output : ['vk_dispatch_table.c', 'vk_dispatch_table.h'],
  command : [
    prog_python, '@INPUT0@', '--xml', '@INPUT1@',
    '--out-c', '@OUTPUT0@', '--out-h', '@OUTPUT1@',
    '--beta', with_vulkan_beta.to_string()
  ],
  depend_files : vk_dispatch_table_gen_depend_files,
)
```