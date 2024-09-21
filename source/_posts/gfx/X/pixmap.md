---
title: Pixmap in X
date: 2020-09-27 22:13:07
tags: [X11]
categories: graphics
---

# XYPixmap
The data for a XYPixmap is organized as a set of bitmaps representing individual bit planes, with the planes appearing from MSB to LSB in bit order just as demonstrated below. Every pixel value is 0x55 (b'01010101) in this example. 

<!--more-->

<pre>
                              ┌────────────────────────────┐
                              │1111111111111111111111111111│        
                            ┌─┴──────────────────────────┐1│
                            │0000000000000000000000000000│1│        
                          ┌─┴──────────────────────────┐0│1│
                          │1111111111111111111111111111│0│1│        
                        ┌─┴──────────────────────────┐1│0│1│
                        │0000000000000000000000000000│1│0│1│        
                      ┌─┴──────────────────────────┐0│1│0│1│
                      │1111111111111111111111111111│0│1│0│1│        
                    ┌─┴──────────────────────────┐1│0│1│0│1│◀────────────────── plane_7 
                    │0000000000000000000000000000│1│0│1│0│─┘        
                  ┌─┴──────────────────────────┐0│1│0│1│0│◀────────────────── plane_6 
                  │1111111111111111111111111111│0│1│0│1│─┘       
                ┌─┴──────────────────────────┐1│0│1│0│1│◀────────────────── plane_5 
                │0000000000000000000000000000│1│0│1│0│─┘      
                │0000000000000000000000000000│1│0│1│0│◀────────────────── plane_4       
                │0000000000000000000000000000│1│0│1│─┘      
                │0000000000000000000000000000│1│0│1│◀────────────────── plane_3       
                │0000000000000000000000000000│1│0│─┘      
                │0000000000000000000000000000│1│0│◀────────────────── plane_2  
                │0000000000000000000000000000│1│─┘       
                │0000000000000000000000000000│1│◀────────────────── plane_1        
                │0000000000000000000000000000│─┘       
                │0000000000000000000000000000│◀────────────────── plane_0       
                └────────────────────────────┘
</pre>

# Pixel buffer
As a core feature of GLX & EGL, pixel buffer or pBuffer is allowed for off-screen rendering. Pixel buffer is essentially a renderable area allocated by OpenGL itself (most likely a framebuffer) and is bound to an OpenGL rendering context.</b>
With respect to GLX, `glXCreatePbuffer` creates a pixel buffer and return its `XID`. In the implementation of Mesa, it calls `XCreatPixmap` to get an `XID` which is bound to the pixel buffer.
