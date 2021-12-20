---
title: regex in vi
date: 2020-02-29 21:58:03
tags: [vim]
categories: programming
---
## Multiline
Remove the multilined comment in the separated paragraphs

```c
struct gl_vertex_array_object
{
   GLuint Name;
   GLint RefCount;
   GLchar *Label;       
   /**
    * Has this array object been bound?
    */
   GLboolean EverBound;
   /**
    * Marked to true if the object is shared between contexts and immutable.
    * Then reference counting is done using atomics and thread safe.
    * Is used for dlist VAOs.
    */
   bool SharedAndImmutable;
   struct gl_array_attributes VertexAttrib[VERT_ATTRIB_MAX];
   struct gl_vertex_buffer_binding BufferBinding[VERT_ATTRIB_MAX];
   GLbitfield VertexAttribBufferMask;
   GLbitfield Enabled;
   /**
    * Mask of VERT_BIT_* enabled arrays past position/generic0 mapping
    *
    * The value is valid past calling _mesa_update_vao_derived_arrays.
    * Note that _mesa_update_vao_derived_arrays is called when binding
    * the VAO to Array._DrawVAO.
    */
   GLbitfield _EffEnabledVBO;
   gl_attribute_map_mode _AttributeMapMode;
   GLbitfield NewArrays;
   struct gl_buffer_object *IndexBufferObj;
};
```

Ex mode:

```
:%s;/\*\*\_.\{-}\*/;;
```

where `\{-}` means non-greedy match. See `:help non-greedy`.

A regular expression in vim often includes the following patterns to search text in a single line:

- `.` any character, except a newline
- `^` the beginning of the line
- `$` the end of the line
- `\s` a space or tab

The above patterns can be replaced with other patterns that also match the newline character, as follows:

- `\_.` any character, including a newline
- `\_^` the beginning of a line
- `\_$` the end of a line
- `\_s` a space, tab or newline character
- `\n` a newline character

## Empty Lines
Remove empty lines or lines which only contain whitespaces.

```
:g/^\s*$/d
```

## Case Transform

To lowercase

```
:s/^##\s\(.\)/## \L\1/
```

To uppercase

```
:s/^##\s\(.\)/## \U\1/
```
