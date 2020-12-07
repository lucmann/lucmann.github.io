---
title: Altering Your GCC/Python Version Easily
date: 2020-12-04 17:21:05
tags: [Linux]
---

# What's the problem
有时在你的系统上可能已经安装了多个gcc版本(gcc-4.8, gcc-8, gcc-9)或Python版本(Python, Python 2.7, Python 3.8), 那么当你执行gcc或python命令时，到底用的是哪个gcc或python版本呢？而且如何方便地指定我们期望使用的版本呢？在大多数Linux发行版都有一个**update-alternatives**命令专门处理这种情况。

<!--more-->

# How to solve it
`update-alternatives`通过在`/etc/alternatives/`目录下创建一个符号链接文件，通过修改这个文件的指向达到轻松更换像gcc或Python这类系统软件的版本的目的。

## Synopsis

update-alternatives [option...] command

## Commands

--install <u>link</u> <u>name</u> <u>path</u> <u>priority</u>

--set <u>name</u> <u>path</u>

--remove <u>name</u> <u>path</u>

--remove-all <u>name</u>

--query <u>name</u>

