---
title: glmark2-desktop
date: 2021-03-23 16:08:16
tags: [GL]
categories: bin
---

# glmark2
[glmark2](https://github.com/glmark2/glmark2)是一个OpenGL 2.0及OpenGL ES 2.0的测试集。它包含了17个测试场景

- buffer
- build
- bump
- clear
- conditionals
- desktop
- effect2d
- function
- ideas
- jellyfish
- loop
- pulsar
- refract
- shading
- shadow
- terrain
- texture

# desktop
**desktop**这个场景在fragment shader中使用了**convolution**计算高斯模糊效果, 因为某种原因，我认为这个用例是一个比较特别的用例，所以在这里记录一下在**Mesa llvmpipe**下以及`glmark2 -b desktop`不同的选项下的测试结果。

``` ascii "glmark2 --list | awk '/\[Scene\] desktop/, /\[Scene\] effect2d/ { print }'"
[Scene] desktop
  [Option] blur-radius
    Description  : the blur effect radius (in pixels)
    Default Value: 5
  [Option] duration
    Description  : The duration of each benchmark in seconds
    Default Value: 10.0
  [Option] effect
    Description  : The effect to use
    Default Value: blur
    Acceptable Values: blur,shadow
  [Option] fps-pos
    Description  : The position on screen where to show FPS
    Default Value: -1.0,-1.0
  [Option] fps-size
    Description  : The width of each glyph for the FPS
    Default Value: 0.03
  [Option] fragment-precision
    Description  : The precision values for the fragment shader ("int,float,sampler2d,samplercube")
    Default Value: default,default,default,default
  [Option] nframes
    Description  : The number of frames to render
    Default Value: 
  [Option] passes
    Description  : the number of effect passes (effect dependent)
    Default Value: 1
  [Option] separable
    Description  : use separable convolution for the blur effect
    Default Value: true
    Acceptable Values: false,true
  [Option] shadow-size
    Description  : the size of the shadow (in pixels)
    Default Value: 20
  [Option] show-fps
    Description  : Show live FPS counter
    Default Value: false
    Acceptable Values: false,true
  [Option] title
    Description  : The scene title to show
    Default Value: 
  [Option] title-pos
    Description  : The position on screen where to show the title
    Default Value: -0.7,-1.0
  [Option] title-size
    Description  : The width of each glyph in the title
    Default Value: 0.03
  [Option] vertex-precision
    Description  : The precision values for the vertex shader ("int,float,sampler2d,samplercube")
    Default Value: default,default,default,default
  [Option] window-size
    Description  : the window size as a percentage of the minimum screen dimension [0.0 - 0.5]
    Default Value: 0.35
  [Option] windows
    Description  : the number of windows
    Default Value: 4
```


