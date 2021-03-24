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

{% asset_img "desktop.png" "glmark2 -b desktop" %}

**desktop**这个场景在fragment shader中使用了**convolution**计算高斯模糊效果, 因为某种原因，我认为这个用例是一个比较特别的用例，所以在这里记录一下在**Mesa llvmpipe**下以及`glmark2 -b desktop`不同的选项下的测试结果。

{% codeblock "glmark2 --list | awk '/\[Scene\] desktop/, /\[Scene\] effect2d/ { print }'" %}
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
{% endcodeblock %}

`glmark2 -b desktop`的默认选项值是`:effect=blur:blur-radius=5:separable=true`. 此时，**Fragment Shader**中应用高斯模糊的GLSL代码. 可以看到它有两段fragment shader，分别从水平和垂直方向对纹素进行了卷积计算。

{% codeblock "fragment shader" %}
#ifdef GL_ES
precision mediump float;
#endif
const float TextureStepX = 0.001250;
const float Kernel5 = 0.002659;
const float Kernel4 = 0.013437;
const float Kernel3 = 0.047370;
const float Kernel2 = 0.116512;
const float Kernel1 = 0.199935;
const float Kernel0 = 0.239365;
uniform sampler2D Texture0;

varying vec2 TextureCoord;

void main(void)
{
    vec4 result;

    result = 
texture2D(Texture0, TextureCoord + vec2(-5.0 * TextureStepX, 0.0)) * Kernel5 +
texture2D(Texture0, TextureCoord + vec2(-4.0 * TextureStepX, 0.0)) * Kernel4 +
texture2D(Texture0, TextureCoord + vec2(-3.0 * TextureStepX, 0.0)) * Kernel3 +
texture2D(Texture0, TextureCoord + vec2(-2.0 * TextureStepX, 0.0)) * Kernel2 +
texture2D(Texture0, TextureCoord + vec2(-1.0 * TextureStepX, 0.0)) * Kernel1 +
texture2D(Texture0, TextureCoord + vec2(0.0 * TextureStepX, 0.0)) * Kernel0 +
texture2D(Texture0, TextureCoord + vec2(1.0 * TextureStepX, 0.0)) * Kernel1 +
texture2D(Texture0, TextureCoord + vec2(2.0 * TextureStepX, 0.0)) * Kernel2 +
texture2D(Texture0, TextureCoord + vec2(3.0 * TextureStepX, 0.0)) * Kernel3 +
texture2D(Texture0, TextureCoord + vec2(4.0 * TextureStepX, 0.0)) * Kernel4 +
texture2D(Texture0, TextureCoord + vec2(5.0 * TextureStepX, 0.0)) * Kernel5 +
0.0 ;


    gl_FragColor = vec4(result.xyz, 1.0);
}

#ifdef GL_ES
precision mediump float;
#endif
const float TextureStepY = 0.001667;
const float Kernel5 = 0.002659;
const float Kernel4 = 0.013437;
const float Kernel3 = 0.047370;
const float Kernel2 = 0.116512;
const float Kernel1 = 0.199935;
const float Kernel0 = 0.239365;
uniform sampler2D Texture0;

varying vec2 TextureCoord;

void main(void)
{
    vec4 result;

    result = 
texture2D(Texture0, TextureCoord + vec2(0.0, -5.0 * TextureStepY)) * Kernel5 +
texture2D(Texture0, TextureCoord + vec2(0.0, -4.0 * TextureStepY)) * Kernel4 +
texture2D(Texture0, TextureCoord + vec2(0.0, -3.0 * TextureStepY)) * Kernel3 +
texture2D(Texture0, TextureCoord + vec2(0.0, -2.0 * TextureStepY)) * Kernel2 +
texture2D(Texture0, TextureCoord + vec2(0.0, -1.0 * TextureStepY)) * Kernel1 +
texture2D(Texture0, TextureCoord + vec2(0.0, 0.0 * TextureStepY)) * Kernel0 +
texture2D(Texture0, TextureCoord + vec2(0.0, 1.0 * TextureStepY)) * Kernel1 +
texture2D(Texture0, TextureCoord + vec2(0.0, 2.0 * TextureStepY)) * Kernel2 +
texture2D(Texture0, TextureCoord + vec2(0.0, 3.0 * TextureStepY)) * Kernel3 +
texture2D(Texture0, TextureCoord + vec2(0.0, 4.0 * TextureStepY)) * Kernel4 +
texture2D(Texture0, TextureCoord + vec2(0.0, 5.0 * TextureStepY)) * Kernel5 +
0.0 ;


    gl_FragColor = vec4(result.xyz, 1.0);
}
{% endcodeblock %}
