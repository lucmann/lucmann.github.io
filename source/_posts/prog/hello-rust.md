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

# #![no_std]

Rust 的标准库依赖 C 库 libc.so.6。但 Rust 语言允许你禁用标准库，从而不依赖 C 库。要达到这个目的，需要对 Hello world 程序和编译过程做些[修改](https://github.com/lucmann/pmp/blob/master/rust/hello-world-nostd/main.rs)

- `#![no_std]` 明确告诉 rustc 不要用标准库， 那就意味着不能调用 `println!` 宏在标准输出上打印字符, 可以通过内联汇编调用 `write` 系统调用直接将字符送到标准输出
- `#![no_main]` 不要 main 函数，那就意味着要写一个名字为 `_start` 的函数作为程序入口点，也意味着需要告诉**静态链接器**不要链接 `__libc_start_main`, `main`这些函数，这一点通过环境变量 `RUSTFLAGS` 可以告诉 cargo
    ```shell
    RUSTFLAGS="-C link-arg=-nostartfiles" cargo build
    ```
- 要提供一个 `panic_handler`, 且需要将 `panic` 的触发事件改为 `abort` (默认 `panic=unwind`)

```rust
#![no_std]
#![no_main]

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_: &PanicInfo) -> ! {
    loop {} // Loop forever to halt the program
}

#[no_mangle]
pub extern "C" fn _start() {
    let message = b"Hello, world!\n";
    let fd: usize = 1; // File descriptor for stdout
    let syscall_no: usize = 1; // Syscall number for write in Linux
    unsafe {
        // Use inline asm macro to perform the syscall
        core::arch::asm!(
            "syscall",
            in("rax") syscall_no, // syscall number
            in("rdi") fd,         // file descriptor
            in("rsi") message.as_ptr(), // pointer to the message
            in("rdx") message.len(), // length of the message
            out("rcx") _, out("r11") _,
        );
    }

    loop {} // Loop forever to avoid segfault since _start() is expected
            // to never return in a no_std environment
}
```

# Rust for Linux

Rust 是 2022 年 10 月随 Linux 6.1-rc1 进入内核主线的。Linux 内核从此就变成了一个双语言项目，首先要解决的问题之一就是 C 和 Rust 函数互相调用的问题，这就需要一个自动生成 **Rust FFI bindings to C** 的工具 [bindgen](https://github.com/rust-lang/rust-bindgen), 所以 bindgen 也是构建内核中 Rust 代码的依赖之一。

内核有个特殊之处就是它不能链接 C 标准库，所以它自然也不能链接 Rust 标准库，所以编译开启 Rust (`CONFIG_RUST`) 内核需要安装 Rust 标准库的源码，这步可以通过 rustup 完成

```
rustup component add rust-src
```

这些完成后，在内核源码的根目录下执行 `make rustavailable` 检查编译内核的 Rust 环境是否已经准备 OK.
