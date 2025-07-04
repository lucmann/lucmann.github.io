---
title: Hello, Rust
date: 2021-07-11 23:47:20
tags: Rust
categories: programming
---

{% asset_img cargo-build.png %}

<!--more-->
# Rust Startup

安装 rust 的方式有好几种，但最方便的应该是通过 **rustup** 脚本

```
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
```

它会安装最新的 rust 到 `$HOME/.cargo` 目录下，安装的组件包括

- Rust 构建系统和包管理器 `cargo`
- Rust 静态代码分析工具 `clippy`
- Rust 离线文档 **rust-docs**
- Rust 标准库 **rust-std**
- Rust 编译器 `rustc`
- Rust 代码格式工具 `rustfmt` 

这些组件安装后， Rust 环境基本上就 OK 了，可以写一个 hello world 测试一下

```rust
fn main() {
    println!("Hello, world!");
}
```
可以直接使用 `rustc` 来编译它，就像使用 gcc 那样

```shell
rustc main.rs -o main
```

当然推荐的方法是使用 `cargo`, 就像编译 C 时大多用 `make` 一样。 `cargo` 是 rust 的包管理工具，帮助管理项目中包的依赖及应用的构建，创建一个 Rust 项目，通常第一步是执行 `cargo init`, 它自动创建一个 **Rust binary (application) package**, 这样的包里会包含一个 **Cargo.toml** 文件(自动生成)， 后面 `cargo build` 就是根据这个文件内容来编译 Rust 项目。(如果要清理构建的结果，使用 `cargo clean`)

# Rust for Linux

Rust 是 2022 年 10 月随 Linux 6.1-rc1 进入内核主线的。Linux 内核从此就变成了一个双语言项目，首先要解决的问题之一就是 C 和 Rust 函数互相调用的问题，这就需要一个自动生成 **Rust FFI bindings to C** 的工具 [bindgen](https://github.com/rust-lang/rust-bindgen), 所以 bindgen 也是构建内核中 Rust 代码的依赖之一。

内核有个特殊之处就是它不能链接 C 标准库，所以它自然也不能链接 Rust 标准库，所以编译开启 Rust (`CONFIG_RUST`) 内核需要安装 Rust 标准库的源码，这步可以通过 rustup 完成

```
rustup component add rust-src
```

这些完成后，在内核源码的根目录下执行 `make rustavailable` 检查编译内核的 Rust 环境是否已经准备 OK.
