---
title: DRI Configuration Infrastructure
date: 2024-11-06 14:44:56
tags: [mesa]
categories: graphics
---

# Introduction

**driconf** 是一个基于 XML 的 DRI 驱动配置机制，它同时提供系统范围的和每用户的配置方式 (如果同时存在，后者覆盖前者)。driconf 提供统一的配置格式，并整理出驱动常见的配置选项，方便易用。 driconf 不仅可以针对指定设备，指定 screen, 指定驱动设置选项，而且可以针对每个应用设置选项。

<!--more-->

一个 drirc XML 文件的示例

```xml
<driconf>
  <device screen="0" driver="swrast">
    <application name="default">
      <!-- Default options for all applications -->
      <option name="pp_nogreen" value="0"/>
    </application>
    <application name="specific_application" executable="application_executable">
      <!-- Specific options for 'specific_application' -->
      <option name="option_name" value="option_value"/>
    </application>
    <!-- Add more application-specific settings here -->
  </device>
  <!-- Add more device-specific settings here -->
</driconf>
```

这个示例中用了 4 个 XML 元素，drirc 文件支持 5 个 XML 元素:
- `<driconf></driconf>`
- `<device></device>`
- `<application></application>`
- `<engine></engine>`
- `<option ... />`

Option 的值类型可以是以下 5 种, 但 bool 类型的值在 XML 文件中只能填 `true/false`, 不能填 `0/1`, 这应该和 XML Parser 有关。enum 和 int 类型在 XML 文件中都是填数字。
- bool
- enum
- int
- float
- string

```c
typedef union driOptionValue {
   unsigned char _bool; /**< \brief Boolean */
   int _int;      /**< \brief Integer or Enum */
   float _float;  /**< \brief Floating-point */
   char *_string;   /**< \brief String */
} driOptionValue;
```

# Per Application Setting

下面的例子展示将 glxgears 的红色通道禁用，而 glmark2 的绿色通道禁用的 `~/.drirc` 配置, 你可以同时在同一个驱动下运行这两个应用，对它们进行独立的驱动设置。

```xml
<driconf>
  <device screen="0" driver="swrast">
    <application name="glmark2" executable="glmark2">
      <option name="pp_nogreen" value="1"/>
    </application>
    <application name="glxgears" executable="glxgears">
      <option name="pp_nored" value="1"/>
    </application>
  </device>
  <!-- Add more device-specific settings here -->
</driconf>
```
![per-application driconf](/images/driconf/per-application-dri-conf.png)

更有意思的是 `application` 元素不仅提供 `executable` 这一种指定应用的方式，它还提供
- `executable_regexp`
- `sha1`
- `application_name_match`

多种方式指定一个或多个应用，甚至可以指定应用的版本 `application_versions`。

```c
   for (i = 0; attr[i]; i += 2) {
      if (!strcmp(attr[i], "name")) /* not needed here */;
      else if (!strcmp(attr[i], "executable")) exec = attr[i+1];
      else if (!strcmp(attr[i], "executable_regexp")) exec_regexp = attr[i+1];
      else if (!strcmp(attr[i], "sha1")) sha1 = attr[i+1];
      else if (!strcmp(attr[i], "application_name_match"))
         application_name_match = attr[i+1];
      else if (!strcmp(attr[i], "application_versions"))
         application_versions = attr[i+1];
      else XML_WARNING("unknown application attribute: %s.", attr[i]);
   }
```

# Mesa DriConf Implementation

## Configuration Override

**drirc** 配置文件一般在 3 个地方

- `$datadir`/drirc.d/* (e.g. `/usr/share/drirc.d/*` 或 `/usr/local/share/drirc.d/*`)
- `$sysconfdir`/drirc (e.g. `/etc/drirc`)
- `$HOME`/.drirc (e.g. `/home/luc/.drirc`)

Mesa 实现的读取和解析的顺序是从上到下，后解析的会覆盖前面的

## XML Config Helpers

Mesa 中 (`util/xmlconfig.c`) 提供了读取和解析 XML 文件的 Helper 函数，并在 `util/driconf.h` 中定义好了常见配置选项。

- `driParseOptionInfo`

```
void
driParseOptionInfo(driOptionCache *info,
                   const driOptionDescription *configOptions,
                   unsigned numOptions);
```

- `driParseConfigFiles`

```
void
driParseConfigFiles(driOptionCache *cache, const driOptionCache *info,
                    int screenNum, const char *driverName,
                    const char *kernelDriverName,
                    const char *deviceName,
                    const char *applicationName, uint32_t applicationVersion,
                    const char *engineName, uint32_t engineVersion)
```

它们俩个都必须有两个 `driOptionCache` 作为入参，一个是调用者输入的 (const 修饰的)，驱动用它来初始化另一个 `driOptionCache`。调用者输入的 `driOptionCache` 是 Gallium **pipe-loader** 把从原始的 XML 文件解析出来的 Option 的描述 `driOptionDescription` 保存在 `struct drm_driver_descriptor` 结构体的 `.driconf` 成员中。

pipe-loader 甚至提供了一个 C 头文件 *src/gallium/auxiliary/pipe-loader/driinfo_gallium.h*， 里面包含了所有 Gallium 驱动都支持的 DRI 配置选项，就是说即使在系统中没有任何额外的 drirc XML 文件的情况下, 通过修改 *driinfo_gallium.h* 重新编译驱动，就能改变驱动的一些行为 (当然，系统路径下的 drirc 如果设置了相同的选项会覆盖 driinfo_gallium.h 的设置, 因为系统路径下的配置运行时才解析的)

```diff
diff --git a/src/gallium/auxiliary/pipe-loader/driinfo_gallium.h b/src/gallium/auxiliary/pipe-loader/driinfo_gallium.h
index 3b0ab726e8f..0e8184a6b95 100644
--- a/src/gallium/auxiliary/pipe-loader/driinfo_gallium.h
+++ b/src/gallium/auxiliary/pipe-loader/driinfo_gallium.h
@@ -9,7 +9,7 @@ DRI_CONF_SECTION_END
 DRI_CONF_SECTION_QUALITY
    DRI_CONF_PP_CELSHADE(0)
    DRI_CONF_PP_NORED(0)
-   DRI_CONF_PP_NOGREEN(0)
+   DRI_CONF_PP_NOGREEN(1)
    DRI_CONF_PP_NOBLUE(0)
    DRI_CONF_PP_JIMENEZMLAA(0, 0, 32)
    DRI_CONF_PP_JIMENEZMLAA_COLOR(0, 0, 32)
```

![glmark2 -bshadow](/images/driconf/scene-shadow-no-green.png)


## How to add driconf support in Gallium driver

一般是在 gallium xxx_device (或 xxx_screen) 结构体中添加一个包含 `driOptionCache` 结构体和一大堆配置选项变量的结构体，在 `xxx_screen_create()` 函数里调用 `driParseConfigFiles()` 或 `driParseOptionInfo()` 函数把 `/etc/drirc` 文件里的选项解析到 xxx_device 或 xxx_screen 结构体中

# References
- [Common Configuration Options](https://dri.freedesktop.org/wiki/ConfigurationOptions/)
- [https://dri.freedesktop.org/wiki/ConfigurationInfrastructure/](https://dri.freedesktop.org/wiki/ConfigurationInfrastructure/)
- [GUI 工具 DriConf](https://dri.freedesktop.org/wiki/DriConf/)
