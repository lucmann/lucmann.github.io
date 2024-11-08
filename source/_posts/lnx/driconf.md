---
title: DRI Configuration Infrastructure
date: 2024-11-06 14:44:56
tags: [mesa]
categories: linux
---

# Introduction

**driconf** 是一个基于 XML 的 DRI 驱动配置机制，它同时提供系统范围的和每用户的配置方式 (如果同时存在，后者覆盖前者)。driconf 提供统一的配置格式，并整理出驱动常见的配置选项，方便易用。

<!--more-->

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

# References
- [Common Configuration Options](https://dri.freedesktop.org/wiki/ConfigurationOptions/)
- [https://dri.freedesktop.org/wiki/ConfigurationInfrastructure/](https://dri.freedesktop.org/wiki/ConfigurationInfrastructure/)
- [GUI 工具 DriConf](https://dri.freedesktop.org/wiki/DriConf/)
