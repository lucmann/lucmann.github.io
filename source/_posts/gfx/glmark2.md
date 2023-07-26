---
title: glmark2 Benchmark
date: 2021-03-23 16:08:16
tags: [OpenGL]
categories: graphics
---

# Something you should know about [glmark2](https://github.com/glmark2/glmark2)

1. glmark2 的命令行参数格式

```
glmark2 -b desktop:nframes=1000000:show-fps=true -b build:duration=100
```

<!--more-->

2. glmark2 每个场景 (Scene) 默认的运行时间 (duration) 是 10 秒

3. glmark2 提供一个公共选项 (Option) **show-fps**, 可以显示 Head-up Display (HUD) 形式的 FPS

![glmark2-desktop show-fps](glmark2-desktop-show-fps.png)

4. glmark2 计算 FPS 的时间精度 (resolution) 是 nanosecond. 它使用的是 `struct timespec`, 而不是 `struct timeval`

# desktop

{% asset_img "desktop.png" "glmark2 -b desktop" %}

**desktop**这个场景在fragment shader中使用了**convolution**计算高斯模糊效果, 这里我们比较一下所有17个场景的fragment shader, 并记录一下在**Mesa llvmpipe**下以及`glmark2 -b desktop`不同的选项下的测试结果。

## Fragment Shaders
### build
{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
varying vec4 Color;
varying vec2 TextureCoord;

void main(void)
{
    gl_FragColor = Color;
}
{% endcodeblock %}

### texture
{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D MaterialTexture0;

varying vec4 Color;
varying vec2 TextureCoord;

void main(void)
{
    vec4 texel = texture2D(MaterialTexture0, TextureCoord);
    gl_FragColor = texel * Color;
}
{% endcodeblock %}

### shading
{% codeblock "shading=gouraud" %}
#ifdef GL_ES
precision mediump float;
#endif
varying vec4 Color;
varying vec2 TextureCoord;

void main(void)
{
    gl_FragColor = Color;
}
{% endcodeblock %}

{% codeblock "shading=blinn-phong-inf" %}
#ifdef GL_ES
precision mediump float;
#endif
const vec3 LightSourceHalfVector = vec3(0.408248, 0.408248, 0.816497);
const vec4 LightSourcePosition = vec4(20.000000, 20.000000, 10.000000, 1.000000);
varying vec3 Normal;

void main(void)
{
    const vec4 LightSourceAmbient = vec4(0.1, 0.1, 0.1, 1.0);
    const vec4 LightSourceDiffuse = vec4(0.8, 0.8, 0.8, 1.0);
    const vec4 LightSourceSpecular = vec4(0.8, 0.8, 0.8, 1.0);
    const vec4 MaterialAmbient = vec4(1.0, 1.0, 1.0, 1.0);
    const vec4 MaterialDiffuse = vec4(0.0, 0.0, 1.0, 1.0);
    const vec4 MaterialSpecular = vec4(1.0, 1.0, 1.0, 1.0);
    const float MaterialShininess = 100.0;

    vec3 N = normalize(Normal);

    // In the lighting model we are using here (Blinn-Phong with light at
    // infinity, viewer at infinity), the light position/direction and the
    // half vector is constant for the all the fragments.
    vec3 L = normalize(LightSourcePosition.xyz);
    vec3 H = normalize(LightSourceHalfVector);

    // Calculate the diffuse color according to Lambertian reflectance
    vec4 diffuse = MaterialDiffuse * LightSourceDiffuse * max(dot(N, L), 0.0);

    // Calculate the ambient color
    vec4 ambient = MaterialAmbient * LightSourceAmbient;

    // Calculate the specular color according to the Blinn-Phong model
    vec4 specular = MaterialSpecular * LightSourceSpecular *
                    pow(max(dot(N,H), 0.0), MaterialShininess);

    // Calculate the final color
    gl_FragColor = vec4((ambient + specular + diffuse).xyz, 1.0);
}
{% endcodeblock %}

{% codeblock "shading=phong" %}
#ifdef GL_ES
precision mediump float;
#endif
#ifdef GL_ES
precision mediump float;
#endif
const vec4 MaterialDiffuse = vec4(0.000000, 0.000000, 1.000000, 1.000000);
const vec4 LightColor0 = vec4(0.800000, 0.800000, 0.800000, 1.000000);
const vec4 LightSourcePosition0 = vec4(0.000000, 1.000000, 0.000000, 1.000000);
varying vec3 vertex_normal;
varying vec4 vertex_position;

vec4
compute_color(vec4 light_position, vec4 diffuse_light_color)
{
    const vec4 lightAmbient = vec4(0.1, 0.1, 0.1, 1.0);
    const vec4 lightSpecular = vec4(0.8, 0.8, 0.8, 1.0);
    const vec4 matAmbient = vec4(0.2, 0.2, 0.2, 1.0);
    const vec4 matSpecular = vec4(1.0, 1.0, 1.0, 1.0);
    const float matShininess = 100.0;
    vec3 eye_direction = normalize(-vertex_position.xyz);
    vec3 light_direction = normalize(light_position.xyz/light_position.w -
                                     vertex_position.xyz/vertex_position.w);
    vec3 normalized_normal = normalize(vertex_normal);
    vec3 reflection = reflect(-light_direction, normalized_normal);
    float specularTerm = pow(max(0.0, dot(reflection, eye_direction)), matShininess);
    float diffuseTerm = max(0.0, dot(normalized_normal, light_direction));
    vec4 specular = (lightSpecular * matSpecular);
    vec4 ambient = (lightAmbient * matAmbient);
    vec4 diffuse = (diffuse_light_color * MaterialDiffuse);
    vec4 result = (specular * specularTerm) + ambient + (diffuse * diffuseTerm);
    return result;
}

void main(void)
{
    gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    gl_FragColor += compute_color(LightSourcePosition0, LightColor0);

}
{% endcodeblock %}

{% codeblock "shading=cel" %}
#ifdef GL_ES
precision mediump float;
#endif
varying vec3 vertex_normal;
varying vec4 vertex_position;

void main(void)
{
    const vec4 OutlineColor = vec4(0.0, 0.0, 0.0, 1.0);
    const vec2 OutlineThickness = vec2(0.1, 0.4);
    const vec4 BaseColor = vec4(0.0, 0.3, 0.0, 1.0);
    const vec4 LightColor = vec4(1.0, 1.0, 1.0, 1.0);
    const vec4 LightSourcePosition = vec4(4.0, 3.0, 1.0, 1.0);
    const vec4 DiffuseColor = vec4(0.0, 0.6, 0.0, 1.0);
    const vec4 SpecularColor = vec4(1.0, 1.0, 1.0, 0.7);
    const float DiffuseThreshold = 0.1;
    const float SpecularThreshold = 0.5;
    const float Shininess = 10.0;

    // Initialize the fragment color with an unlit value.
    vec4 fragColor = BaseColor;

    // Set up factors for computing diffuse illumination
    vec3 vertex_light = LightSourcePosition.xyz - vertex_position.xyz;
    vec3 N = normalize(vertex_normal);
    vec3 L = normalize(vertex_light);
    float NdotL = dot(N, L);
    float maxNdotL = max(NdotL, 0.0);
    float attenuation = length(LightSourcePosition) / length(vertex_light);

    // See if we have a diffuse contribution...
    // This will only be true if the interpolated normal and the light
    // are pointing in the "same" direction, and the attenuation due to
    // distance allows enough light for diffuse reflection.
    if (attenuation * maxNdotL >= DiffuseThreshold) {
        fragColor = LightColor * DiffuseColor;
    }

    // See if this fragment is part of the silhouette
    // If it is facing away from the viewer enough not to get any
    // diffuse illumination contribution, then it is close enough
    // to the silouhette to be painted with the outline color rather
    // than the unlit color.
    vec3 V = normalize(-vertex_position.xyz);
    if (dot(V, N) <
        mix(OutlineThickness.x, OutlineThickness.y, maxNdotL)) {
        fragColor = LightColor * OutlineColor;
    }

    // See if we have a specular contribution...
    // If the interpolated normal direction and the light direction
    // are facing the "same" direction, and the attenuated specular
    // intensity is strong enough, then we have a contribution.
    vec3 R = reflect(-L, N);
    float specularIntensity = pow(max(0.0, dot(R, V)), Shininess);
    if (NdotL > 0.0 && attenuation * specularIntensity > SpecularThreshold) {
        fragColor = SpecularColor.a * LightColor * SpecularColor +
            (1.0 - SpecularColor.a) * fragColor;
    }

    // Emit the final color
    gl_FragColor = vec4(fragColor.xyz, 1.0);
}
{% endcodeblock %}

### bump
{% codeblock "bump-render=high-poly" %}
#ifdef GL_ES
precision mediump float;
#endif
const vec3 LightSourceHalfVector = vec3(0.408248, 0.408248, 0.816497);
const vec4 LightSourcePosition = vec4(20.000000, 20.000000, 10.000000, 1.000000);
varying vec3 Normal;

void main(void)
{
    const vec4 LightSourceAmbient = vec4(0.1, 0.1, 0.1, 1.0);
    const vec4 LightSourceDiffuse = vec4(0.8, 0.8, 0.8, 1.0);
    const vec4 LightSourceSpecular = vec4(0.8, 0.8, 0.8, 1.0);
    const vec4 MaterialAmbient = vec4(1.0, 1.0, 1.0, 1.0);
    const vec4 MaterialDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
    const vec4 MaterialSpecular = vec4(0.2, 0.2, 0.2, 1.0);
    const float MaterialShininess = 100.0;

    vec3 N = normalize(Normal);

    // In the lighting model we are using here (Blinn-Phong with light at
    // infinity, viewer at infinity), the light position/direction and the
    // half vector is constant for the all the fragments.
    vec3 L = normalize(LightSourcePosition.xyz);
    vec3 H = normalize(LightSourceHalfVector);

    // Calculate the diffuse color according to Lambertian reflectance
    vec4 diffuse = MaterialDiffuse * LightSourceDiffuse * max(dot(N, L), 0.0);

    // Calculate the ambient color
    vec4 ambient = MaterialAmbient * LightSourceAmbient;

    // Calculate the specular color according to the Blinn-Phong model
    vec4 specular = MaterialSpecular * LightSourceSpecular *
                    pow(max(dot(N,H), 0.0), MaterialShininess);

    // Calculate the final color
    gl_FragColor = ambient + specular + diffuse;
}
{% endcodeblock %}

{% codeblock "bump-render=normals" %}
#ifdef GL_ES
precision mediump float;
#endif
const vec3 LightSourceHalfVector = vec3(0.408248, 0.408248, 0.816497);
const vec4 LightSourcePosition = vec4(20.000000, 20.000000, 10.000000, 1.000000);
uniform sampler2D NormalMap;
uniform mat4 NormalMatrix;

varying vec2 TextureCoord;

void main(void)
{
    const vec4 LightSourceAmbient = vec4(0.1, 0.1, 0.1, 1.0);
    const vec4 LightSourceDiffuse = vec4(0.8, 0.8, 0.8, 1.0);
    const vec4 LightSourceSpecular = vec4(0.8, 0.8, 0.8, 1.0);
    const vec4 MaterialAmbient = vec4(1.0, 1.0, 1.0, 1.0);
    const vec4 MaterialDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
    const vec4 MaterialSpecular = vec4(0.2, 0.2, 0.2, 1.0);
    const float MaterialShininess = 100.0;

    // Get the raw normal XYZ data from the normal map
    vec3 normal_raw = texture2D(NormalMap, TextureCoord).xyz;
    // Map "color" range [0, 1.0] to normal range [-1.0, 1.0]
    vec3 normal_scaled = normal_raw * 2.0 - 1.0;

    // Convert the normal to eye coordinates. Note that the normal map
    // we are using is using object coordinates (not tangent!) for the
    // normals, so we can multiply by the NormalMatrix as usual.
    vec3 N = normalize(vec3(NormalMatrix * vec4(normal_scaled, 1.0)));

    // In the lighting model we are using here (Blinn-Phong with light at
    // infinity, viewer at infinity), the light position/direction and the
    // half vector is constant for the all the fragments.
    vec3 L = normalize(LightSourcePosition.xyz);
    vec3 H = normalize(LightSourceHalfVector);

    // Calculate the diffuse color according to Lambertian reflectance
    vec4 diffuse = MaterialDiffuse * LightSourceDiffuse * max(dot(N, L), 0.0);

    // Calculate the ambient color
    vec4 ambient = MaterialAmbient * LightSourceAmbient;

    // Calculate the specular color according to the Blinn-Phong model
    vec4 specular = MaterialSpecular * LightSourceSpecular *
                    pow(max(dot(N,H), 0.0), MaterialShininess);

    // Calculate the final color
    gl_FragColor = ambient + specular + diffuse;
}
{% endcodeblock %}

{% codeblock "bump-render=height" %}
#ifdef GL_ES
precision mediump float;
#endif
#ifdef GL_ES
precision mediump float;
#endif
const float TextureStepY = 0.000977;
const float TextureStepX = 0.000977;
const vec3 LightSourceHalfVector = vec3(0.408248, 0.408248, 0.816497);
const vec4 LightSourcePosition = vec4(20.000000, 20.000000, 10.000000, 1.000000);

uniform sampler2D HeightMap;

#ifdef GL_FRAGMENT_PRECISION_HIGH
varying highp vec2 TextureCoord;
#else
varying vec2 TextureCoord;
#endif

varying vec3 NormalEye;
varying vec3 TangentEye;
varying vec3 BitangentEye;

void main(void)
{
    const vec4 LightSourceAmbient = vec4(0.1, 0.1, 0.1, 1.0);
    const vec4 LightSourceDiffuse = vec4(0.8, 0.8, 0.8, 1.0);
    const vec4 LightSourceSpecular = vec4(0.8, 0.8, 0.8, 1.0);
    const vec4 MaterialAmbient = vec4(1.0, 1.0, 1.0, 1.0);
    const vec4 MaterialDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
    const vec4 MaterialSpecular = vec4(0.2, 0.2, 0.2, 1.0);
    const float MaterialShininess = 100.0;
    const float height_factor = 13.0;

    // Get the data from the height map
    float height0 = texture2D(HeightMap, TextureCoord).x;
    float heightX = texture2D(HeightMap, TextureCoord + vec2(TextureStepX, 0.0)).x;
    float heightY = texture2D(HeightMap, TextureCoord + vec2(0.0, TextureStepY)).x;
    vec2 dh = vec2(heightX - height0, heightY - height0);

    // Adjust the normal based on the height map data
    vec3 N = NormalEye - height_factor * dh.x * TangentEye -
                         height_factor * dh.y * BitangentEye;
    N = normalize(N);

    // In the lighting model we are using here (Blinn-Phong with light at
    // infinity, viewer at infinity), the light position/direction and the
    // half vector is constant for the all the fragments.
    vec3 L = normalize(LightSourcePosition.xyz);
    vec3 H = normalize(LightSourceHalfVector);

    // Calculate the diffuse color according to Lambertian reflectance
    vec4 diffuse = MaterialDiffuse * LightSourceDiffuse * max(dot(N, L), 0.0);

    // Calculate the ambient color
    vec4 ambient = MaterialAmbient * LightSourceAmbient;

    // Calculate the specular color according to the Blinn-Phong model
    vec4 specular = MaterialSpecular * LightSourceSpecular *
                    pow(max(dot(N,H), 0.0), MaterialShininess);

    // Calculate the final color
    gl_FragColor = ambient + specular + diffuse;

    //gl_FragColor = vec4(height_diff_raw.xy, 0.0, 1.0);
    //gl_FragColor = vec4(height_diff_scaled.xy, 0.0, 1.0);
    //gl_FragColor = vec4(Tangent, 1.0);
}
{% endcodeblock %}

### effect2d
{% codeblock "kernel=0,1,0;1,-4,1;0,1,0;" %}
#ifdef GL_ES
precision mediump float;
#endif
#ifdef GL_ES
precision mediump float;
#endif
const float Kernel0 = 0.000000;
const float Kernel1 = 0.250000;
const float Kernel2 = 0.000000;
const float Kernel3 = 0.250000;
const float Kernel4 = -1.000000;
const float Kernel5 = 0.250000;
const float Kernel6 = 0.000000;
const float Kernel7 = 0.250000;
const float Kernel8 = 0.000000;
const float TextureStepY = 0.001667;
const float TextureStepX = 0.001250;
uniform sampler2D Texture0;
varying vec2 TextureCoord;

void main(void)
{
    vec4 result;

    result = texture2D(Texture0, TextureCoord + vec2(-1.0 * TextureStepX, 1.0 * TextureStepY)) * Kernel0 +
texture2D(Texture0, TextureCoord + vec2(0.0 * TextureStepX, 1.0 * TextureStepY)) * Kernel1 +
texture2D(Texture0, TextureCoord + vec2(1.0 * TextureStepX, 1.0 * TextureStepY)) * Kernel2 +
texture2D(Texture0, TextureCoord + vec2(-1.0 * TextureStepX, 0.0 * TextureStepY)) * Kernel3 +
texture2D(Texture0, TextureCoord + vec2(0.0 * TextureStepX, 0.0 * TextureStepY)) * Kernel4 +
texture2D(Texture0, TextureCoord + vec2(1.0 * TextureStepX, 0.0 * TextureStepY)) * Kernel5 +
texture2D(Texture0, TextureCoord + vec2(-1.0 * TextureStepX, -1.0 * TextureStepY)) * Kernel6 +
texture2D(Texture0, TextureCoord + vec2(0.0 * TextureStepX, -1.0 * TextureStepY)) * Kernel7 +
texture2D(Texture0, TextureCoord + vec2(1.0 * TextureStepX, -1.0 * TextureStepY)) * Kernel8;


    gl_FragColor = vec4(result.xyz, 1.0);
}
{% endcodeblock %}

{% codeblock "kernel=1,1,1,1,1;1,1,1,1,1;1,1,1,1,1;" %}
#ifdef GL_ES
precision mediump float;
#endif
#ifdef GL_ES
precision mediump float;
#endif
const float Kernel0 = 0.066667;
const float Kernel1 = 0.066667;
const float Kernel2 = 0.066667;
const float Kernel3 = 0.066667;
const float Kernel4 = 0.066667;
const float Kernel5 = 0.066667;
const float Kernel6 = 0.066667;
const float Kernel7 = 0.066667;
const float Kernel8 = 0.066667;
const float Kernel9 = 0.066667;
const float Kernel10 = 0.066667;
const float Kernel11 = 0.066667;
const float Kernel12 = 0.066667;
const float Kernel13 = 0.066667;
const float Kernel14 = 0.066667;
const float TextureStepY = 0.001667;
const float TextureStepX = 0.001250;
uniform sampler2D Texture0;
varying vec2 TextureCoord;

void main(void)
{
    vec4 result;

    result = texture2D(Texture0, TextureCoord + vec2(-2.0 * TextureStepX, 1.0 * TextureStepY)) * Kernel0 +
texture2D(Texture0, TextureCoord + vec2(-1.0 * TextureStepX, 1.0 * TextureStepY)) * Kernel1 +
texture2D(Texture0, TextureCoord + vec2(0.0 * TextureStepX, 1.0 * TextureStepY)) * Kernel2 +
texture2D(Texture0, TextureCoord + vec2(1.0 * TextureStepX, 1.0 * TextureStepY)) * Kernel3 +
texture2D(Texture0, TextureCoord + vec2(2.0 * TextureStepX, 1.0 * TextureStepY)) * Kernel4 +
texture2D(Texture0, TextureCoord + vec2(-2.0 * TextureStepX, 0.0 * TextureStepY)) * Kernel5 +
texture2D(Texture0, TextureCoord + vec2(-1.0 * TextureStepX, 0.0 * TextureStepY)) * Kernel6 +
texture2D(Texture0, TextureCoord + vec2(0.0 * TextureStepX, 0.0 * TextureStepY)) * Kernel7 +
texture2D(Texture0, TextureCoord + vec2(1.0 * TextureStepX, 0.0 * TextureStepY)) * Kernel8 +
texture2D(Texture0, TextureCoord + vec2(2.0 * TextureStepX, 0.0 * TextureStepY)) * Kernel9 +
texture2D(Texture0, TextureCoord + vec2(-2.0 * TextureStepX, -1.0 * TextureStepY)) * Kernel10 +
texture2D(Texture0, TextureCoord + vec2(-1.0 * TextureStepX, -1.0 * TextureStepY)) * Kernel11 +
texture2D(Texture0, TextureCoord + vec2(0.0 * TextureStepX, -1.0 * TextureStepY)) * Kernel12 +
texture2D(Texture0, TextureCoord + vec2(1.0 * TextureStepX, -1.0 * TextureStepY)) * Kernel13 +
texture2D(Texture0, TextureCoord + vec2(2.0 * TextureStepX, -1.0 * TextureStepY)) * Kernel14;


    gl_FragColor = vec4(result.xyz, 1.0);
}
{% endcodeblock %}

### pulsar
{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
varying vec4 Color;
varying vec2 TextureCoord;

void main(void)
{
    gl_FragColor = Color;
}
{% endcodeblock %}

### desktop
{% codeblock "blur-radius=5:effect=blur:passes=1:separable=true:windows=4" %}
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
{% endcodeblock %}

{% codeblock "blur-radius=5:effect=blur:passes=1:separable=true:windows=4" %}
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

{% codeblock "blur-radius=5:effect=blur:passes=1:separable=true:windows=4" %}
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
{% endcodeblock %}

{% codeblock "blur-radius=5:effect=blur:passes=1:separable=true:windows=4" %}
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

{% codeblock "blur-radius=5:effect=blur:passes=1:separable=true:windows=4" %}
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
{% endcodeblock %}

{% codeblock "blur-radius=5:effect=blur:passes=1:separable=true:windows=4" %}
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

{% codeblock "blur-radius=5:effect=blur:passes=1:separable=true:windows=4" %}
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
{% endcodeblock %}

{% codeblock "blur-radius=5:effect=blur:passes=1:separable=true:windows=4" %}
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

{% codeblock "effect=shadow:windows=4" %}
#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D MaterialTexture0;

varying vec2 TextureCoord;

void main(void)
{
    vec4 texel = texture2D(MaterialTexture0, TextureCoord);
    gl_FragColor = texel;
}
{% endcodeblock %}

### buffer
{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
varying vec4 dist;

const vec4 LINE_COLOR = vec4(1.0);
const vec4 TRIANGLE_COLOR = vec4(0.0, 0.5, 0.8, 0.8);

void main(void)
{
    // Get the minimum distance of this fragment from a triangle edge.
    // We need to multiply with dist.w to undo the workaround we had
    // to perform to get linear interpolation (instead of perspective correct).
    float d = min(dist.x * dist.w, min(dist.y * dist.w, dist.z * dist.w));

    // Get the intensity of the wireframe line
    float I = exp2(-2.0 * d * d);

    gl_FragColor = mix(TRIANGLE_COLOR, LINE_COLOR, I);
}
{% endcodeblock %}

### ideas
{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
varying vec4 color;

void main()
{
    gl_FragColor = color;
}
{% endcodeblock %}

{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
uniform vec4 light0Position;
varying vec3 vertex_normal;
varying vec4 vertex_position;
varying vec3 eye_direction;

vec3 unitvec(vec4 v1, vec4 v2)
{
    if (v1.w == 0.0 && v2.w == 0.0)
        return vec3(v2 - v1);
    if (v1.w == 0.0)
        return vec3(-v1);
    if (v2.w == 0.0)
        return vec3(v2);
    return v2.xyz/v2.w - v1.xyz/v1.w;
}

void main()
{
    vec4 lightAmbient = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 lightDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 lightSpecular = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 matAmbient = vec4(0.1, 0.1, 0.1, 1.0);
    vec4 matDiffuse = vec4(0.5, 0.4, 0.7, 1.0);
    vec4 matSpecular = vec4(1.0, 1.0, 1.0, 1.0);
    float matShininess = 30.0;
    vec3 light_direction = normalize(unitvec(vertex_position, light0Position));
    vec3 normalized_normal = normalize(vertex_normal);
    vec3 reflection = reflect(-light_direction, normalized_normal);
    float specularTerm = pow(max(0.0, dot(reflection, eye_direction)), matShininess);
    float diffuseTerm = max(0.0, dot(normalized_normal, light_direction));
    vec4 specular = (lightSpecular * matSpecular);
    vec4 ambient = (lightAmbient * matAmbient);
    vec4 diffuse = (lightDiffuse * matDiffuse);
    gl_FragColor = (specular * specularTerm) + ambient + (diffuse * diffuseTerm);
}
{% endcodeblock %}

{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D tex;

void main()
{
    vec2 curPos = gl_FragCoord.xy / 32.0;
    vec4 color = texture2D(tex, curPos);
    if (color.w < 0.5)
        discard;
    gl_FragColor = color;
}
{% endcodeblock %}

{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
struct LightSourceParameters
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position;
};
LightSourceParameters lightSource[3];
uniform vec4 light0Position;
uniform vec4 light1Position;
uniform vec4 light2Position;
varying vec3 vertex_normal;
varying vec4 vertex_position;
varying vec3 eye_direction;

vec3 unitvec(vec4 v1, vec4 v2)
{
    if (v1.w == 0.0 && v2.w == 0.0)
        return vec3(v2 - v1);
    if (v1.w == 0.0)
        return vec3(-v1);
    if (v2.w == 0.0)
        return vec3(v2);
    return v2.xyz/v2.w - v1.xyz/v1.w;
}

void main()
{
    lightSource[0] = LightSourceParameters(
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(0.0, 1.0, 0.0, 0.0)
    );
    lightSource[1] = LightSourceParameters(
        vec4(0.0, 0.0, 0.0, 1.0),
        vec4(0.3, 0.3, 0.5, 1.0),
        vec4(0.3, 0.3, 0.5, 1.0),
        vec4(-1.0, 0.0, 0.0, 0.0)
    );
    lightSource[2] = LightSourceParameters(
        vec4(0.2, 0.2, 0.2, 1.0),
        vec4(0.2, 0.2, 0.2, 1.0),
        vec4(0.2, 0.2, 0.2, 1.0),
        vec4(0.0, -1.0, 0.0, 0.0)
    );
    vec4 matAmbient = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 matDiffuse = vec4(1.0, 0.2, 0.2, 1.0);
    vec4 matSpecular = vec4(0.5, 0.5, 0.5, 1.0);
    float matShininess = 20.0;
    vec4 diffuseSum = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 specularSum = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 ambientSum = vec4(0.0, 0.0, 0.0, 0.0);
    vec3 normalized_normal = normalize(vertex_normal);
    lightSource[0].position = light0Position;
    lightSource[1].position = light1Position;
    lightSource[2].position = light2Position;
    for (int light = 0; light < 3; light++) {
        vec4 light_position = lightSource[light].position;
        vec3 light_direction = normalize(unitvec(vertex_position, light_position));
        vec3 reflection = reflect(-light_direction, normalized_normal);
        specularSum += pow(max(0.0, dot(reflection, eye_direction)), matShininess) * lightSource[light].specular;
        diffuseSum += max(0.0, dot(normalized_normal, light_direction)) * lightSource[light].diffuse;
        ambientSum += lightSource[light].ambient;
    }
    gl_FragColor = (matSpecular * specularSum) + (matAmbient * ambientSum) + (matDiffuse * diffuseSum);
}
{% endcodeblock %}

### terrain
{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
//
// Description : Array and textureless GLSL 3D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : ijm
//     Lastmod : 20110409 (stegu)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//

#ifdef GL_ES
#define MEDIUMP mediump
#else
#define MEDIUMP
#endif

uniform float time;
uniform MEDIUMP vec2 uvScale;
varying vec2 vUv;

#ifdef GL_FRAGMENT_PRECISION_HIGH
// x should be passed as highp since the intermediate multiplications can
// overflow with mediump
vec4 permute(highp vec4 x)
#else
vec4 permute(vec4 x)
#endif
{
    return mod(((x * 34.0) + 1.0) * x, 289.0);
}

vec4 taylorInvSqrt(vec4 r)
{
    return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
{
    const vec2 C = vec2(1.0 / 6.0, 1.0 / 3.0);
    const vec4 D = vec4(0.0, 0.5, 1.0, 2.0);

    // First corner
    vec3 i  = floor(v + dot(v, C.yyy));
    vec3 x0 = v - i + dot(i, C.xxx);

    // Other corners
    vec3 g = step(x0.yzx, x0.xyz);
    vec3 l = 1.0 - g;
    vec3 i1 = min(g.xyz, l.zxy);
    vec3 i2 = max(g.xyz, l.zxy);

    vec3 x1 = x0 - i1 + 1.0 * C.xxx;
    vec3 x2 = x0 - i2 + 2.0 * C.xxx;
    vec3 x3 = x0 - 1. + 3.0 * C.xxx;

    // Permutations
    i = mod(i, 289.0);
    vec4 p = permute(permute(permute(
                    i.z + vec4(0.0, i1.z, i2.z, 1.0))
                + i.y + vec4(0.0, i1.y, i2.y, 1.0))
            + i.x + vec4(0.0, i1.x, i2.x, 1.0));

    // Gradients
    // (N*N points uniformly over a square, mapped onto an octahedron.)

    float n_ = 1.0 / 7.0; // N=7

    vec3 ns = n_ * D.wyz - D.xzx;

    vec4 j = p - 49.0 * floor(p * ns.z *ns.z);  //  mod(p,N*N)

    vec4 x_ = floor(j * ns.z);
    vec4 y_ = floor(j - 7.0 * x_);    // mod(j,N)

    vec4 x = x_ *ns.x + ns.yyyy;
    vec4 y = y_ *ns.x + ns.yyyy;
    vec4 h = 1.0 - abs(x) - abs(y);

    vec4 b0 = vec4(x.xy, y.xy);
    vec4 b1 = vec4(x.zw, y.zw);


    vec4 s0 = floor(b0) * 2.0 + 1.0;
    vec4 s1 = floor(b1) * 2.0 + 1.0;
    vec4 sh = -step(h, vec4(0.0));

    vec4 a0 = b0.xzyw + s0.xzyw * sh.xxyy;
    vec4 a1 = b1.xzyw + s1.xzyw * sh.zzww;

    vec3 p0 = vec3(a0.xy, h.x);
    vec3 p1 = vec3(a0.zw, h.y);
    vec3 p2 = vec3(a1.xy, h.z);
    vec3 p3 = vec3(a1.zw, h.w);

    // Normalise gradients

    vec4 norm = taylorInvSqrt(vec4(dot(p0, p0), dot(p1, p1), dot(p2, p2), dot(p3, p3)));
    p0 *= norm.x;
    p1 *= norm.y;
    p2 *= norm.z;
    p3 *= norm.w;

    // Mix final noise value

    vec4 m = max(0.6 - vec4(dot(x0, x0), dot(x1, x1), dot(x2, x2), dot(x3, x3)), 0.0);
    m = m * m;
    return 42.0 * dot(m*m, vec4(dot(p0, x0), dot(p1, x1),
                dot(p2, x2), dot(p3, x3)));
}

float surface3(vec3 coord)
{
    float n = 0.0;

    n += 1.0 * abs(snoise(coord));
    n += 0.5 * abs(snoise(coord * 2.0));
    n += 0.25 * abs(snoise(coord * 4.0));
    n += 0.125 * abs(snoise(coord * 8.0));

    return n;
}

void main(void)
{
    vec3 coord = vec3(vUv.x, uvScale.y - vUv.y, -time);
    float n = surface3(coord);

    gl_FragColor = vec4(vec3(n, n, n), 1.0);
}
{% endcodeblock %}

{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
uniform float height;
uniform vec2 resolution;
uniform sampler2D heightMap;

varying vec2 vUv;

void main()
{
    float val = texture2D( heightMap, vUv ).x;

    float valU = texture2D( heightMap, vUv + vec2( 1.0 / resolution.x, 0.0 ) ).x;
    float valV = texture2D( heightMap, vUv + vec2( 0.0, 1.0 / resolution.y ) ).x;

    gl_FragColor = vec4( ( 0.5 * normalize( vec3( val - valU, val - valV, height  ) ) + 0.5 ), 1.0 );
}
{% endcodeblock %}

{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D tDiffuse;

varying vec2 vUv;

void main()
{
    vec4 texel = texture2D( tDiffuse, vUv );

    vec3 luma = vec3( 0.299, 0.587, 0.114 );

    float v = dot( texel.xyz, luma );

    gl_FragColor = vec4( v, v, v, texel.w );
}
#ifdef GL_ES
precision mediump float;
#endif
uniform vec3 uAmbientColor;
uniform vec3 uDiffuseColor;
uniform vec3 uSpecularColor;
uniform float uShininess;
uniform float uOpacity;
uniform sampler2D tDiffuse1;
uniform sampler2D tDiffuse2;
uniform sampler2D tDetail;
uniform sampler2D tNormal;
uniform sampler2D tSpecular;
uniform sampler2D tDisplacement;
uniform float uNormalScale;
uniform vec2 uRepeatOverlay;
uniform vec2 uOffset;
varying vec3 vTangent;
varying vec3 vBinormal;
varying vec3 vNormal;
varying vec2 vUv;
uniform vec3 ambientLightColor;
uniform mat4 viewMatrix;

#define MAX_POINT_LIGHTS 1
//#define USE_FOG
//#define FOG_EXP2

#if MAX_POINT_LIGHTS > 0
uniform vec3 pointLightColor[ MAX_POINT_LIGHTS ];
uniform vec3 pointLightPosition[ MAX_POINT_LIGHTS ];
uniform float pointLightDistance[ MAX_POINT_LIGHTS ];
#endif

varying vec3 vViewPosition;

// CHUNK: fog_pars_fragment
#ifdef USE_FOG
uniform vec3 fogColor;
#ifdef FOG_EXP2
uniform float fogDensity;
#else
uniform float fogNear;
uniform float fogFar;
#endif
#endif

void main() {
    gl_FragColor = vec4( vec3( 1.0 ), uOpacity );
    vec3 specularTex = vec3( 1.0 );
    vec2 uvOverlay = uRepeatOverlay * vUv + uOffset;
    vec3 normalTex = texture2D( tDetail, uvOverlay ).xyz * 2.0 - 1.0;
    normalTex.xy *= uNormalScale;
    normalTex = normalize( normalTex );

    vec4 colDiffuse1 = texture2D( tDiffuse1, uvOverlay );
    vec4 colDiffuse2 = texture2D( tDiffuse2, uvOverlay );
    gl_FragColor = gl_FragColor * mix ( colDiffuse1, colDiffuse2, 1.0 - texture2D( tDisplacement, vUv) );

    specularTex = texture2D( tSpecular, uvOverlay ).xyz;

    mat3 tbn= mat3( vTangent, vBinormal, vNormal );
    vec3 finalNormal = tbn * normalTex;
    vec3 normal = normalize( finalNormal );
    vec3 viewPosition = normalize( vViewPosition );

    // point lights
#if MAX_POINT_LIGHTS > 0
    vec3 pointDiffuse = vec3( 0.0 );
    vec3 pointSpecular = vec3( 0.0 );
    for ( int i = 0; i < MAX_POINT_LIGHTS; i ++ ) {
        vec4 lPosition = viewMatrix * vec4( pointLightPosition[ i ], 1.0 );
#ifdef GL_FRAGMENT_PRECISION_HIGH
        // should be highp for correct behaviour if mediump is implemented as fp16
        highp vec3 lVector = lPosition.xyz + vViewPosition.xyz;
#else
        vec3 lVector = lPosition.xyz + vViewPosition.xyz;
#endif
        float lDistance = 1.0;
        if ( pointLightDistance[ i ] > 0.0 )
            lDistance = 1.0 - min( ( length( lVector ) / pointLightDistance[ i ] ), 1.0 );
        lVector = normalize( lVector );
        vec3 pointHalfVector = normalize( lVector + viewPosition );
        float pointDistance = lDistance;
        float pointDotNormalHalf = max( dot( normal, pointHalfVector ), 0.0 );
        float pointDiffuseWeight = max( dot( normal, lVector ), 0.0 );
        float pointSpecularWeight = specularTex.r * pow( pointDotNormalHalf, uShininess );
        pointDiffuse += pointDistance * pointLightColor[ i ] * uDiffuseColor * pointDiffuseWeight;
        pointSpecular += pointDistance * pointLightColor[ i ] * uSpecularColor * pointSpecularWeight * pointDiffuseWeight;
    }
#endif

    // all lights contribution summation
    vec3 totalDiffuse = vec3( 0.0 );
    vec3 totalSpecular = vec3( 0.0 );


#if MAX_POINT_LIGHTS > 0
    totalDiffuse += pointDiffuse;
    totalSpecular += pointSpecular;
#endif

    gl_FragColor.xyz = gl_FragColor.xyz * ( totalDiffuse + ambientLightColor * uAmbientColor + totalSpecular );

    //CHUNK: fog_fragment
#ifdef USE_FOG
    float depth = gl_FragCoord.z / gl_FragCoord.w;
#ifdef FOG_EXP2
    const float LOG2 = 1.442695;
    float fogFactor = exp2( - fogDensity * fogDensity * depth * depth * LOG2 );
    fogFactor = 1.0 - clamp( fogFactor, 0.0, 1.0 );
#else
    float fogFactor = smoothstep( fogNear, fogFar, depth );
#endif
    gl_FragColor = mix( gl_FragColor, vec4( fogColor, gl_FragColor.w ), fogFactor );
#endif
}
{% endcodeblock %}

{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
const float TextureStepX = 0.001250;
const float TiltShift = 0.000000;
const float Kernel2 = 0.187627;
const float Kernel1 = 0.206068;
const float Kernel0 = 0.212609;
uniform sampler2D Texture0;

varying vec2 vUv;

void main(void)
{
    vec4 result;
    vec2 TextureCoord = vUv;

    float stepX = TextureStepX * abs(TiltShift - TextureCoord.y) / abs(1.0 - TiltShift);
result = 
texture2D(Texture0, TextureCoord + vec2(-2.0 * stepX, 0.0)) * Kernel2 +
texture2D(Texture0, TextureCoord + vec2(-1.0 * stepX, 0.0)) * Kernel1 +
texture2D(Texture0, TextureCoord + vec2(0.0 * stepX, 0.0)) * Kernel0 +
texture2D(Texture0, TextureCoord + vec2(1.0 * stepX, 0.0)) * Kernel1 +
texture2D(Texture0, TextureCoord + vec2(2.0 * stepX, 0.0)) * Kernel2 +
0.0 ;


    gl_FragColor = vec4(result.xyz, 1.0);
}
{% endcodeblock %}

{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
const float TextureStepY = 0.003906;
const float TiltShift = 0.000000;
const float Kernel2 = 0.187627;
const float Kernel1 = 0.206068;
const float Kernel0 = 0.212609;
uniform sampler2D Texture0;

varying vec2 vUv;

void main(void)
{
    vec4 result;
    vec2 TextureCoord = vUv;

    float stepY = TextureStepY * abs(TiltShift - TextureCoord.y) / abs(1.0 - TiltShift);
result = 
texture2D(Texture0, TextureCoord + vec2(0.0, -2.0 * stepY)) * Kernel2 +
texture2D(Texture0, TextureCoord + vec2(0.0, -1.0 * stepY)) * Kernel1 +
texture2D(Texture0, TextureCoord + vec2(0.0, 0.0 * stepY)) * Kernel0 +
texture2D(Texture0, TextureCoord + vec2(0.0, 1.0 * stepY)) * Kernel1 +
texture2D(Texture0, TextureCoord + vec2(0.0, 2.0 * stepY)) * Kernel2 +
0.0 ;


    gl_FragColor = vec4(result.xyz, 1.0);
}
{% endcodeblock %}

{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
uniform float opacity;
uniform sampler2D tDiffuse;

varying vec2 vUv;

void main()
{
    vec4 texel = texture2D(tDiffuse, vUv);
    gl_FragColor = opacity * texel;
}
{% endcodeblock %}

{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
const float TextureStepX = 0.001250;
const float TiltShift = 0.500000;
const float Kernel4 = 0.054409;
const float Kernel3 = 0.087939;
const float Kernel2 = 0.123913;
const float Kernel1 = 0.152223;
const float Kernel0 = 0.163030;
uniform sampler2D Texture0;

varying vec2 vUv;

void main(void)
{
    vec4 result;
    vec2 TextureCoord = vUv;

    float stepX = TextureStepX * abs(TiltShift - TextureCoord.y) / abs(1.0 - TiltShift);
result = 
texture2D(Texture0, TextureCoord + vec2(-4.0 * stepX, 0.0)) * Kernel4 +
texture2D(Texture0, TextureCoord + vec2(-3.0 * stepX, 0.0)) * Kernel3 +
texture2D(Texture0, TextureCoord + vec2(-2.0 * stepX, 0.0)) * Kernel2 +
texture2D(Texture0, TextureCoord + vec2(-1.0 * stepX, 0.0)) * Kernel1 +
texture2D(Texture0, TextureCoord + vec2(0.0 * stepX, 0.0)) * Kernel0 +
texture2D(Texture0, TextureCoord + vec2(1.0 * stepX, 0.0)) * Kernel1 +
texture2D(Texture0, TextureCoord + vec2(2.0 * stepX, 0.0)) * Kernel2 +
texture2D(Texture0, TextureCoord + vec2(3.0 * stepX, 0.0)) * Kernel3 +
texture2D(Texture0, TextureCoord + vec2(4.0 * stepX, 0.0)) * Kernel4 +
0.0 ;


    gl_FragColor = vec4(result.xyz, 1.0);
}
{% endcodeblock %}

{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
const float TextureStepY = 0.001667;
const float TiltShift = 0.500000;
const float Kernel4 = 0.054409;
const float Kernel3 = 0.087939;
const float Kernel2 = 0.123913;
const float Kernel1 = 0.152223;
const float Kernel0 = 0.163030;
uniform sampler2D Texture0;

varying vec2 vUv;

void main(void)
{
    vec4 result;
    vec2 TextureCoord = vUv;

    float stepY = TextureStepY * abs(TiltShift - TextureCoord.y) / abs(1.0 - TiltShift);
result = 
texture2D(Texture0, TextureCoord + vec2(0.0, -4.0 * stepY)) * Kernel4 +
texture2D(Texture0, TextureCoord + vec2(0.0, -3.0 * stepY)) * Kernel3 +
texture2D(Texture0, TextureCoord + vec2(0.0, -2.0 * stepY)) * Kernel2 +
texture2D(Texture0, TextureCoord + vec2(0.0, -1.0 * stepY)) * Kernel1 +
texture2D(Texture0, TextureCoord + vec2(0.0, 0.0 * stepY)) * Kernel0 +
texture2D(Texture0, TextureCoord + vec2(0.0, 1.0 * stepY)) * Kernel1 +
texture2D(Texture0, TextureCoord + vec2(0.0, 2.0 * stepY)) * Kernel2 +
texture2D(Texture0, TextureCoord + vec2(0.0, 3.0 * stepY)) * Kernel3 +
texture2D(Texture0, TextureCoord + vec2(0.0, 4.0 * stepY)) * Kernel4 +
0.0 ;


    gl_FragColor = vec4(result.xyz, 1.0);
}
{% endcodeblock %}

### jellyfish
{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
#ifdef GL_ES
precision highp float;
#endif
 
uniform sampler2D uSampler;
uniform sampler2D uSampler1;
uniform float uCurrentTime;
 
varying vec2 vTextureCoord;
varying vec4 vWorld;
varying vec3 vDiffuse;
varying vec3 vAmbient;
varying vec3 vFresnel;

void main(void)
{
    vec4 caustics = texture2D(uSampler1, vec2(vWorld.x / 24.0 + uCurrentTime / 20.0, (vWorld.z - vWorld.y)/48.0 + uCurrentTime / 40.0));
    vec4 colorMap = texture2D(uSampler, vTextureCoord);
    float transparency = colorMap.a + pow(vFresnel.r, 2.0) - 0.3;
    gl_FragColor = vec4(((vAmbient + vDiffuse + caustics.rgb) * colorMap.rgb), transparency);
}
{% endcodeblock %}

### loop
{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
varying vec4 dummy;
uniform int FragmentLoops;

void main(void)
{
#ifdef GL_FRAGMENT_PRECISION_HIGH
    // should be declared highp since the multiplication can overflow in
    // mediump, particularly if mediump is implemented as fp16
    highp vec2 FragCoord = gl_FragCoord.xy;
#else
    vec2 FragCoord = gl_FragCoord.xy;
#endif
    float d = fract(FragCoord.x * FragCoord.y * 0.0001);

    for (int i = 0; i < FragmentLoops; i++)
        d = fract(3.0 * d);


    gl_FragColor = vec4(d, d, d, 1.0);
}
{% endcodeblock %}

### refract
{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
const float RefractiveIndex = 1.200000;
const vec4 LightSourcePosition = vec4(1.000000, 1.000000, 2.000000, 1.000000);
const vec4 LightColor = vec4(0.400000, 0.400000, 0.400000, 1.000000);
uniform sampler2D DistanceMap;
uniform sampler2D NormalMap;
uniform sampler2D ImageMap;

varying vec3 vertex_normal;
varying vec4 vertex_position;
varying vec4 MapCoord;

void main()
{
    const vec4 lightSpecular = vec4(0.8, 0.8, 0.8, 1.0);
    const vec4 matSpecular = vec4(1.0, 1.0, 1.0, 1.0);
    const float matShininess = 100.0;
    const vec2 point_five = vec2(0.5);
    // Need the normalized eye direction and surface normal vectors to
    // compute the transmitted vector through the "front" surface of the object.
    vec3 eye_direction = normalize(-vertex_position.xyz);
    vec3 normalized_normal = normalize(vertex_normal);
    vec3 front_refraction = refract(eye_direction, normalized_normal, RefractiveIndex);
    // Find our best distance approximation through the object so we can
    // project the transmitted vector to the back of the object to find
    // the exit point.
    vec3 mc_perspective = (MapCoord.xyz / MapCoord.w) + front_refraction;
    vec2 dcoord = mc_perspective.st * point_five + point_five;
    vec4 distance_value = texture2D(DistanceMap, dcoord);
    vec3 back_position = vertex_position.xyz + front_refraction * distance_value.x;
    // Use the exit point to index the map of back-side normals, and use the
    // back-side position and normal to find the transmitted vector out of the
    // object.
    vec2 normcoord = back_position.st * point_five + point_five;
    vec3 back_normal = texture2D(NormalMap, normcoord).xyz;
    vec3 back_refraction = refract(back_position, back_normal, 1.0/RefractiveIndex);
    // Use the transmitted vector from the exit point to determine where
    // the vector would intersect the environment (in this case a background
    // image.
    vec2 imagecoord = back_refraction.st * point_five + point_five;
    vec4 texel = texture2D(ImageMap, imagecoord);
    // Add in specular reflection, and we have our fragment value.
    vec3 light_direction = normalize(vertex_position.xyz/vertex_position.w -
                                     LightSourcePosition.xyz/LightSourcePosition.w);
    vec3 reflection = reflect(light_direction, normalized_normal);
    float specularTerm = pow(max(0.0, dot(reflection, eye_direction)), matShininess);
    vec4 specular = (lightSpecular * matSpecular);
    gl_FragColor = (specular * specularTerm) + texel;
}
{% endcodeblock %}

### shadow
{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
uniform sampler2D ShadowMap;

varying vec4 Color;
varying vec4 ShadowCoord;

void main()
{
    vec4 sc_perspective = ShadowCoord / ShadowCoord.w;
    sc_perspective.z += 0.1505;
    vec4 shadow_value = texture2D(ShadowMap, sc_perspective.st);
    float light_distance = shadow_value.x;
    float shadow = 1.0;
    if (ShadowCoord.w > 0.0 && light_distance < sc_perspective.z) {
        shadow = 0.5;
    }
    gl_FragColor = vec4(shadow * Color.rgb, 1.0);
}
{% endcodeblock %}

### function
{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
varying vec4 dummy;

float process(float d)
{
    d = fract(3.0 * d);

    return d;
}

void main(void)
{
#ifdef GL_FRAGMENT_PRECISION_HIGH
    // should be declared highp since the multiplication can overflow in
    // mediump, particularly if mediump is implemented as fp16
    highp vec2 FragCoord = gl_FragCoord.xy;
#else
    vec2 FragCoord = gl_FragCoord.xy;
#endif
    float d = fract(FragCoord.x * FragCoord.y * 0.0001);

    d = process(d);


    gl_FragColor = vec4(d, d, d, 1.0);
}
{% endcodeblock %}

### conditionals
{% codeblock %}
#ifdef GL_ES
precision mediump float;
#endif
varying vec4 dummy;

void main(void)
{
#ifdef GL_FRAGMENT_PRECISION_HIGH
    // should be declared highp since the multiplication can overflow in
    // mediump, particularly if mediump is implemented as fp16
    highp vec2 FragCoord = gl_FragCoord.xy;
#else
    vec2 FragCoord = gl_FragCoord.xy;
#endif
    float d = fract(FragCoord.x * FragCoord.y * 0.0001);

    if (d >= 0.5)
        d = fract(2.0 * d);
    else
        d = fract(3.0 * d);


    gl_FragColor = vec4(d, d, d, 1.0);
}
{% endcodeblock %}

从上面这些场景的Fragment Shader来看，**desktop**和**clear**这两个场景比较特殊，clear没有shader, 而**desktop**有8个fragment shaders，而且每个fragment shader里都调用了多次`texture2D`这个纹理查询GLSL内置函数。

- 为什么有8个fragment shader?

    SceneDesktop这个场景除了主窗口外，默认还有4个小窗口，而这4个小窗口有两种特效: **blur**, **shadow**. blur效果是通过卷积实现的模糊效果，而且可以水平和垂直方向分开模糊，由选项`separable`控制，默认是`true`(水平和垂直各一个fragment shader).

- 为什么每个fragment shader里有那么多`texture2D`?

## SceneDesktop Test

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

