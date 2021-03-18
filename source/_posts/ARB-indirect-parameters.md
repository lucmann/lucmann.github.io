---
title: ARB_indirect_parameters
date: 2021-03-18 11:29:57
tags: [GL]
---

# [Overview](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_indirect_parameters.txt)

Numbering **ARB Extension #154**

Require **OpenGL 4.2**

Since **OpenGL 4.3 Core Profile**

<!--more-->

和它相关的OpenGL概念是**Indirect Draw**, 相关的扩展是[GL_ARB_draw_indirect](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_draw_indirect.txt)(since OpenGL 3.2 Compatible Profile and since OpenGL 4.0 Core Profile)和[GL_ARB_multi_draw_indirect](https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_multi_draw_indirect.txt)(since OpenGL 4.1 Core Profile). 这个扩展引入了"parameter buffer"的概念，可以让indirect draw的大量的参数集合存储在buffer object, 然后用一个API调用分发整个indirect draw列表，从而提高OpenGL管线的执行效率。

更确切地说，这个扩展新增了一个buffer target，专门用来存放某个indirect drawing command的参数。

```
PARAMETER_BUFFER_ARB    0x80EE
```

它可以作为下列命令的参数
- BindBuffer
- BufferData
- BufferSubData
- MapBuffer
- UnmapBuffer
- GetBufferSubData
- GetBufferPointerv
- MapBufferRange
- FlushMappedBufferRange
- GetBufferParameteriv
- CopyBufferSubData

新增的**MultiDraw*Indirect**命令的变种是

```
MultiDrawArraysIndirectCountARB
MultiDrawElementsIndirectCountARB
```

