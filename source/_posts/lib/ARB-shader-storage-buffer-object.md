---
title: ARB_shader_storage_buffer_object
date: 2021-03-18 15:29:39
tags: [GL]
categories: media
---

# [Overview](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_shader_storage_buffer_object.txt)
Numbers **ARB Extension #137**

<!--more-->

Requires **ARB_program_interface_query**

Interacts
- ARB_compute_shader
- ARB_program_interface_query

Since **OpenGL 4.3 Core Profile Specification**

这个扩展提供能让应用Shader Code对存储在buffer object中的变量进行随机的读写，原子存储操作的能力。这里的buffer objects即为**"shader storage buffers"**.

## Comparison with [ARB_uniform_buffer_object](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_uniform_buffer_object.txt)

### Similarity
**ARB_uniform_buffer_object**引入了一个新的概念**uniform block**用来存放一组uniform变量，调用API将uinform blocks与buffer objects绑定。实际上相当于让shader可以访问buffer objects里的uniforms.

类似地，这个扩展有**interface blocks**的概念，shader声明一组变量即**buffer variables**, 它们以interface blocks的形式组织(相当于一种内存布局)，同样调用API将interface blocks与buffer objects绑定。

### Difference
但是与uniform blocks最大的不同是，uniform blocks是只读的，而interface blocks也即**shader storage buffer**是可读可写的。
也即多个独立的shader可以读写同一块内存。所以这里同样有内存互斥访问的问题存在。OpenGL Specification和Shading Language都提供了一些机制来控制内存访问。

- OpenGL API
```
glMemoryBarrier()
```

- GLSL
```
memoryBarrier()
```


