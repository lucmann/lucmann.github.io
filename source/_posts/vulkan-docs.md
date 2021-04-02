---
title: Vulkan-Docs Build
date: 2021-04-02 21:21:27
tags: [vulkan]
categories: media
---

# Build
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



