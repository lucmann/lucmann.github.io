---
title: Vulkan-Docs Build
date: 2021-04-02 21:21:27
tags: Vulkan
categories: graphics
---

# Introduction
[KhronosGroup/Vulkan-Docs](https://github.com/KhronosGroup/Vulkan-Docs)包含了**Vulkan API**技术文档和相关工具。

| Vulkan Version | Last Updated Tag   |
|:--------------:|:-------------------|
| 1.0            | v1.0.69-core       |
| 1.1            | v1.1.130           |
| 1.2            | v1.2.174 (Current) | 

<!--more-->

# Build
## Testing Environment
- Windows Subsystem for Linux 2.0
- Ubuntu 20.04.2 LTS

## Requirement
Vulkan-Docs的构建需要`ruby`环境。主要是因为它用到一个`asciidoctor-mathematical`的工具包, 而它又依赖`mathematical`, 所以正确的安装顺序是

{% codeblock "ruby development environment" %}
apt-get -qq -y install ruby-dev
{% endcodeblock %}

{% codeblock "required libraries by mathematical" %}
apt-get -qq -y install bison flex libffi-dev libxml2-dev libgdk-pixbuf2.0-dev libcairo2-dev libpango1.0-dev
{% endcodeblock %}

{% codeblock %}
gem install asciidoctor-mathematical
{% endcodeblock %}

除此之外，还需要安装

{% codeblock %}
gem install coderay
{% endcodeblock %}

## Scripts
所有的**HTML**，**PDF**都是通过脚本基于`xml/vk.xml`生成的。比方要生成所有**extensions**相关的环境变量需要的脚本是`scripts/make_ext_dependency.py`.

## Targets
Vulkan-Docs的构建系统基本上是`make` + `shell` + `python`. 它的顶层目录包含一个`Makefile`, 几乎包含了所有构建对象，其它的构建脚本基本上是`make VAR="$VALUE" TARGET $*`的调用形式。 这个Makefile里定义的目标大概有以下这些:

- all
- **allchecks**
- alldocs
- allman
- allspecs
- apiinc
- buildmanpages
- chunked
- clean
- clean_checks
- clean_generated
- clean_html
- clean_man
- clean_pdf
- clean_validusage
- config/extDependency.sh
- diff_html
- extinc
- generated
- hostsyncinc
- **html**
- katexinst
- manhtml
- **manhtmlpages**
- manpdf
- **pdf**
- reflow
- registry
- **styleguide**
- validinc
- validusage

