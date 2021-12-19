---
title: Hello, Rust
date: 2021-07-11 23:47:20
tags: rust
categories: language
---

# Rust Basic

- Rust的编译器 `rustc`
- Rust程序的构建系统或包管理器 `cargo`

# Cargo.toml
## [dependencies]
增加`rand` crate包后再执行构建

{% asset_img cargo-build.png %}

# `cargo update`会做些什么
首先要了解的是在安装`rustup`后，在`$HOME`下会创建一个`.cargo`目录，它的目录结构大概如下

<pre>
/home/luc/.cargo
|-- bin
|   |-- cargo
|   |-- cargo-clippy
|   |-- cargo-fmt
|   |-- cargo-miri
|   |-- clippy-driver
|   |-- rls
|   |-- rust-gdb
|   |-- rust-lldb
|   |-- rustc
|   |-- rustdoc
|   |-- rustfmt
|   `-- rustup
|-- env
`-- registry
    |-- cache
    |   `-- github.com-1ecc6299db9ec823
    |-- index
    |   `-- github.com-1ecc6299db9ec823
    `-- src
        `-- github.com-1ecc6299db9ec823

8 directories, 13 files
</pre>

`cargo update`会根据工程目录下的`Cargo.toml`中`Dependencies`的版本信息下载相应版本的依赖以及依赖的依赖，`cargo update`后的`.cargo`目录结构大概如下

<pre>
/home/luc/.cargo
|-- bin
|   |-- cargo
|   |-- cargo-clippy
|   |-- cargo-fmt
|   |-- cargo-miri
|   |-- clippy-driver
|   |-- rls
|   |-- rust-gdb
|   |-- rust-lldb
|   |-- rustc
|   |-- rustdoc
|   |-- rustfmt
|   `-- rustup
|-- env
`-- registry
    |-- cache
    |   `-- github.com-1ecc6299db9ec823
    |       |-- libc-0.2.98.crate
    |       |-- rand-0.3.23.crate
    |       `-- rand-0.4.6.crate
    |-- index
    |   `-- github.com-1ecc6299db9ec823
    `-- src
        `-- github.com-1ecc6299db9ec823
            |-- libc-0.2.98
            |-- rand-0.3.23
            `-- rand-0.4.6

11 directories, 16 files
</pre>
