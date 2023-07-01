---
title: OpenGL Transform Feedback
date: 2021-05-08 09:04:30
author: luc@sietium.com
tags: [OpenGL]
categories: graphics
toc-depth: 1
---

# [Transform Feedback](https://www.khronos.org/opengl/wiki/Transform_Feedback) 引入

Transform Feedback首先是DirectX3D引入的，但在DirectX3D中它不叫TF, 而是叫**Stream Output Stage**. 它第一次被引入OpenGL是在2006年，但当时是以扩展的形式[GL_EXT_transform_feedback](https://www.khronos.org/registry/OpenGL/extensions/EXT/EXT_transform_feedback.txt)引入的，它正式成为OpenGL Required Core Features是在OpenGL 3.0(2008), 在这之后，OpenGL 4.0, 4.2, 4.6又分别引入了一些TF相关的新特性。

<!--more-->

| Extensions                               | OpenGL Version    |
|:-----------------------------------------|:------------------|
| GL_EXT_transform_feedback                | 3.0               |
| GL_ARB_transform_feedback2               | 4.0               |
| GL_ARB_transform_feedback3               | 4.0               |
| GL_ARB_transform_feedback_instanced      | 4.2               |
| GL_ARB_transform_feedback_overflow_query | 4.6               |

# Transform Feedback 是什么

TF是将Vertex_Processing阶段产生的图元的顶点属性(vertex attributes)保存(capture/record)在**Transform Feedback Buffer**, 保存之后你可以:

- CPU侧读取这些数据重新应用到你的渲染引擎
- GPU直接将这些数据反馈到另一个Draw Call

某种意义上讲，TF是OpenGL第一种compute shader的形式，在TF之前，唯一能从shader的输出读取数据的方式是`glReadPixels`和其同族API.

![OpenGL 3.1 Pipeline](tf-gles-31.jpg)

## [Vertex Processing](https://www.khronos.org/opengl/wiki/Vertex_Processing)

TF的输入来自Vertex Processing, 所以首先要确定Vertex Processing阶段都做什么，输出什么。Vertex Processing的输出是图元(Primitives), 它至少包含一个vertex shader.

| 着色器       | 是否必需   |
|:-------------|:-----------|
| VERTEX       | Y          |
| TESSELLATION | N          |
| GEOMETRY     | N          |

## Capturing

当Vertex Processing最后的shader是geometry或tessellation evaluation时，则所记录的图元就是geometry或tessellation evaluation输出的图元，否则那些顶点数据来自vertex shader. 另外所记录的顶点数据也不一定都保存在同一个buffer里，这即为`bufferMode`的含义。

### Capturing Setting

```
void glTransformFeedbackVaryings(GLuint program,
				 GLsizei count,
				 const char **varyings,
				 GLenum bufferMode);
```

这个API设置:

- 哪些program的输出变量被**captured**
- capturing模式， capturing模式有两种

    - `GL_INTERLEAVED_ATTRIBS`
    - `GL_SEPARATE_ATTRIBS`

- interleaved mode

这种模式下，所有captured的输出被保存在同一个buffer object.

- separate mode

这种模式下，输出未必被captured到不同的buffer object, 它仅仅是指输出被captured到不同的**buffer binding points**. 因为同一个buffer object的不同区域也可以被绑定到不同的**buffer binding points**.

### Captured Output Variables

那么可以记录的顶点属性有多少个呢？大小如何？

### 多少顶点属性或总共多少数据可以被记录

- separate capture

| Limitation                                       | Minimal Value |
|:-------------------------------------------------|:-------------:|
| GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS       | 4             |
| GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS    | 4             |

- interleaved capture

| Limitation                                       | Minimal Value |
|:-------------------------------------------------|:-------------:|
| GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS | 64            |

- advanced interleaved capture (不同的变量放在不同的buffers)

| Limitation                                       | Minimal Value |
|:-------------------------------------------------|:-------------:|
| GL_MAX_TRANSFORM_FEEDBACK_BUFFERS                | 64            |

注意到在interleaved capture mode下，没有顶点属性个数的限制，原因是在interleaved mode下，所有顶点属性都是在同一个buffer中，所以限制了总的components的个数后，无论你记录多少个属性，这些属性总的components个数不能超过这个最大值, 而在separate mode下，每个列出的变量是保存在各自不同的buffer中，所以除了对components设限外，还要对属性个数设限。所以这里`GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS`的一个更好的名字应该是`GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_BUFFERS`.

### 顶点属性的数据类型是什么

- structs
- arrays
- members of interface blocks

## Varyings

# Transform Feedback 处理过程

## Feedback Buffer Binding

当设置好要capturing program中的哪些输出变量，以何种模式captured后，接下来就要设置(申请)保存captured的数据的buffer objects.

```
void glBindBufferRange(GLenum target,
		       GLuint index,
		       GLuint buffer,
		       GLintptr offset,
		       GLsizeiptr size);
```

这里`target`就是`GL_TRANSFORM_FEEDBACK_BUFFER`. `offset`必须是4字节对齐，如果存进buffer的值包含**double-precision**, 则需要8字节对齐。

## Feedback Begin/End

Capture设置好，Buffer绑定好之后，就可以开始**Recording** Vertex Processing产出的数据了。通过调用下面的API开始Feedback:

```
void glBeginTransformFeedback(GLenum primitiveMode);
```

这里`primitiveMode`只能是以下3种之一:

- GL_POINTS
- GL_LINES
- GL_TRIANGLES

Transform Feedback模式被激活后，在没有调用`glPauseTransformFeedback`或`glEndTransformFeedback`之前，以下行为不被允许:

- 修改`GL_TRANSFORM_FEEDBACK_BUFFER`的bindings
- 任何对这些绑定的`GL_TRANSFORM_FEEDBACK_BUFFER`的读写操作
- 重新申请这些绑定的`GL_TRANSFORM_FEEDBACK_BUFFER`的存储
- 修改当前使用的program, 也就是不能调用`glUseProgram`或`glBindProgramPipeline`, 还有`glUseProgramStages`

退出Transform Feedback模式调用:

```
void glEndTransformFeedback();
```

## Feedback Objects

跟其它OpenGL的功能一样，Transform Feedback也有一大堆state需要track, 所以这些state被封装进**Transform Feedback Objects**, 这些Feedback Objects和其它OpenGL Object一样需要`Gen`, `Delete`和`Bind`:

```
void glGenTransformFeedbacks(GLsizei n, GLuint *ids);
void glDeleteTransformFeedbacks(GLsizei n, const GLuint *ids);
void glBindTransformFeedback(GLenum target, GLuint id);
```

NOTE:

- `glBindTransformFeedback`的`target`永远是`GL_TRANSFORM_FEEDBACK`
- 不能绑定一个Feedback Object，如果当前的Feedback Object还是active或没有被paused.

那么Feedback Object里封装了哪些Transform Feedback state呢？

- generic buffer bindings, 即调用`glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, ...)`绑定的那些buffers
- indexed buffer bindings, 即调用`glBindBufferRange(GL_TRANSFORM_FEEDBACK, ...)`绑定的那些buffers
- transform feedback是否是active还是paused
- 被当前的feedback操作记录的count of primitives等等

## Feedback Rendering

OK, 我们现在获得了Vertex Processing的结果，并记录了这次Transform Feedback操作的状态到Transform Feedback Objects(例如captured count of primitives, the number of vertices). 但是我们如何利用这些captured数据呢？

```
void glDrawTransformFeedback(GLenum mode,
			     GLuint id);

void glDrawTransformFeedbackInstanced(GLenum mode,
				      GLuint id,
				      GLsizei instancecount);
void glDrawTransformFeedbackStream(GLenum mode,
				   GLuint id,
				   GLuint stream);
void glDrawTransformFeedbackStreamInstanced(GLenum mode,
					    GLuint id,
					    GLuint stream,
					    GLsizei instancecount);
```

NOTE:

- 在调用这些`Draw`命令前，一定要先调用`glEndTransformFeedback`, 因为只有当调用了`glEndTransformFeedback`后，OpenGL状态机才知道一次完整的Transform Feedback操作完成了。

# Transform Feedback 应用

TF最常见的应用是粒子系统(Particle System), 即包含许许多多小颗粒的场景，如烟，雾，火等，这些场景的共同点是包含大量顶点，而且顶点属性在不断变化，如果没有TF, 会大大增加CPU向GPU传送顶点数据的开销，不论是总线的带宽还是CPU的使用率都可能无法承受。

## 例子 springmass

- 两个Shader programs
	- `m_update_program`, 用来更新(重新计算)顶点属性
	- `m_render_program`, 用来渲染最后的画面

```
void render(double t)
{
    int i;
    glUseProgram(m_update_program);

    glEnable(GL_RASTERIZER_DISCARD);

    for (i = iterations_per_frame; i != 0; --i)
    {
        glBindVertexArray(m_vao[m_iteration_index & 1]);
        glBindTexture(GL_TEXTURE_BUFFER, m_pos_tbo[m_iteration_index & 1]);
        m_iteration_index++;
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[POSITION_A + (m_iteration_index & 1)]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, m_vbo[VELOCITY_A + (m_iteration_index & 1)]);
        glBeginTransformFeedback(GL_POINTS);
        glDrawArrays(GL_POINTS, 0, POINTS_TOTAL);
        glEndTransformFeedback();
    }

    glDisable(GL_RASTERIZER_DISCARD);

    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    glViewport(0, 0, info.windowWidth, info.windowHeight);
    glClearBufferfv(GL_COLOR, 0, black);

    glUseProgram(m_render_program);

    if (draw_points)
    {
        glPointSize(4.0f);
        glDrawArrays(GL_POINTS, 0, POINTS_TOTAL);
    }

    if (draw_lines)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
        glDrawElements(GL_LINES, CONNECTIONS_TOTAL * 2, GL_UNSIGNED_INT, NULL);
    }
}
```

上面这段`render()`函数的前半段是一个双缓冲交替的模式, 用来倒换TFB和Vertex Array Buffer的数据。后半段是简单的`DrawArrays()`或`DrawElements()`。我们简单说下前半段的for-loop.

首先定义了3个Buffer数组：分别用作Vertex Array Buffer, Transform Feedback Buffer和Texture Buffer

- `m_vao[2] = {1, 2}` 
- `m_vbo[5] = {1, 2, 3, 4, 5}`
- `m_pos_tbo[2] = {1, 2}`

循环展开后:

```c
// 0
glBindVertexArray(1);
glBindTexture(GL_TEXTURE_BUFFER, 1);
m_iteration_index++;
glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 2);
glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, 4);
glBeginTransformFeedback(GL_POINTS);
glDrawArrays(GL_POINTS, 0，POINTS_TOTAL);
glEndTransformFeedback();

// 1
glBindVertexArray(2);
glBindTexture(GL_TEXTURE_BUFFER, 2);
m_iteration_index++;
glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 1);
glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, 3);
glBeginTransformFeedback(GL_POINTS);
glDrawArrays(GL_POINTS, 0，POINTS_TOTAL);
glEndTransformFeedback();

// 2
glBindVertexArray(1);
glBindTexture(GL_TEXTURE_BUFFER, 1);
m_iteration_index++;
glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 2);
glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, 4);
glBeginTransformFeedback(GL_POINTS);
glDrawArrays(GL_POINTS, 0，POINTS_TOTAL);
glEndTransformFeedback();
```

![ompparticles](ompparticles.png)

[ompparticles.cpp](https://github.com/lwtbn1/OpenGL_Superbible_7th/blob/master/src/ompparticles/ompparticles.cpp)

# 参考

- [Transform Feedback Is Terrible, So Why Are We Doing It](http://jason-blog.jlekstrand.net/2018/10/transform-feedback-is-terrible-so-why.html)
- [Transform Feedback的前世今生(一)](https://community.arm.com/cn/b/blog/posts/transform-feedback-1547554897)
- [Particle System using Transform Feedback](https://ogldev.org/www/tutorial28/tutorial28.html)

