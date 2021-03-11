---
title: GLX in Mesa
date: 2021-03-11 14:44:56
tags: [X]
categories: lib
---

# GLX

{% blockquote %}
GLX (Initialism for "OpenGL Extension to the X Window System") is an extension to the X Window System core protocol providing an interface between OpenGL and the X Window System as well as extensions to OpenGL itself. (From WiKi)
{% endblockquote %}

Mesa的GLX实现有3种
- xlib
- gallium-xlib
- dri

```mermaid
erDiagram
    A ||--|| B : dep
    A ||--|| C : dep
    A ||--|| D : dep
```
