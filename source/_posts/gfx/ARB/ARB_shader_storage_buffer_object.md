---
title: GL_ARB_shader_storage_buffer_object
date: 2021-03-18 15:29:39
tags: [OpenGL]
categories: graphics
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

# [GL_ARB_uniform_buffer_object](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_uniform_buffer_object.txt)

**GL_ARB_uniform_buffer_object**引入了一个新的概念**uniform block**用来存放一组uniform变量，调用API将uinform blocks与buffer objects绑定。实际上相当于让shader可以访问buffer objects里的uniforms.

与 UBO 相比， SSBO 有以下特点

- 大容量数据存储
    - SSBO 的大小可以达到 128MB 甚至更多
- 读写操作
    - SSBO 支持读写操作，而 UBO 通常是**只读**的。这使用 SSBO 在需要频繁更新数据的场景中非常有用。
- 灵活的数据结构
    - SSBO 可以包含**可变长度**的数据，这在处理动态数据时非常方便

# 使用方法

- 声明 SSBO

```
layout(std430, binding = 0) buffer BufferName {
    vec4 data[];
};
```

- 创建和初始化 SSBO

```
GLuint ssbo;
glGenBuffers(1, &ssbo);
glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
glBufferData(GL_SHADER_STORAGE_BUFFER, bufferSize, NULL, GL_DYNAMIC_COPY);
glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingPoint, ssbo);
```

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


