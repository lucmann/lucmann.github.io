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

# [cargo](https://doc.rust-lang.org/cargo/index.html)

Rust 的包叫 **crate**, **cargo** 既是包管理器，也是 Rust 项目的构建系统。

## 构建

常用的 cargo 构建命令, 每个命令可能支持若干选项，如 `cargo build --bin hello_world`, 当一个 crate 下存在多个目标时， `--bin` 指定构建某个目标

- `cargo init`
- `cargo build`
- `cargo clean`
- `cargo modules structure --package XXX`
    - 显示某个 crate 的框架 (如有哪些函数, 类等)

# Rust Hello World

Rust 相对于 C 是复杂的，那我们也来一个 Non-trivial 版本的 Hello world。这个 Hello World 不使用标准库，而是通过**汇编指令(x86)** 直接调用操作系统的系统调用 (write), 来将"Hello world",送到标准输出。

Rust 的标准库依赖 C 库 libc.so.6。但 Rust 语言允许你禁用标准库，从而不依赖 C 库。要达到这个目的，需要对 Hello world 程序和编译过程做些[修改](https://github.com/lucmann/pmp/tree/master/rust)

- `#![no_std]` 明确告诉 rustc 不要用标准库， 那就意味着不能调用 `println!` 宏在标准输出上打印字符, 可以通过内联汇编调用 `write` 系统调用直接将字符送到标准输出。不要 rust 标准库，也就不要 C 标准库，那也就调用不了 crt1.o 里的 `_start` 函数， 所以这也意味着要自己实现 `_start`
- `#![no_main]` 不要 main 函数，因为程序真正的入口点是 `_start` 函数， 既然我们直接实现 `_start()`, 那也就没必要提供 `main` 这个入口了。
- 要提供一个 `panic_handler`, 且需要将 `panic` 的触发事件改为 `abort` (默认 `panic=unwind`)

`cargo build` 时可以通过环境变量 `RUSTFLAGS` 告诉编译器和静态链接器以上这些信息

```shell
RUSTFLAGS="-Clink-arg=-nostartfiles -Cpanic=abort" cargo build --bin hello_nostd
```

```rust
#![no_std]
#![no_main]

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_: &PanicInfo) -> ! {
    loop {} // Loop forever to halt the program
}

#[no_mangle]
#[cfg(target_arch = "x86_64")]
pub unsafe extern "C" fn _start() {
    let message = b"Hello, world!\n";
    let fd: usize = 1; // File descriptor for stdout
    let syscall_no: usize = 1; // Syscall number for write in Linux x86_64
    // Use inline asm macro to perform the syscall
    core::arch::asm!(
        "syscall",
        in("rax") syscall_no, // syscall number
        in("rdi") fd,         // file descriptor
        in("rsi") message.as_ptr(), // pointer to the message
        in("rdx") message.len(), // length of the message
        out("rcx") _, out("r11") _, // telling compiler these registers will
                                    // be clobbered by syscalls
    );

    // Exit the program with status code 0
    core::arch::asm!(
        "syscall",
        in("rax") 60, // syscall number for exit
        in("rdi") 0,  // exit status code
        out("rcx") _, out("r11") _, // telling compiler these registers will
                                    // be clobbered by syscalls
    );
}
```

# Rust for Linux

Rust 是 2022 年 10 月随 Linux 6.1-rc1 进入内核主线的。Linux 内核从此就变成了一个双语言项目，首先要解决的问题之一就是 C 和 Rust 函数互相调用的问题，这就需要一个自动生成 **Rust FFI bindings to C** 的工具 [bindgen](https://github.com/rust-lang/rust-bindgen), 所以 bindgen 也是构建内核中 Rust 代码的依赖之一。

```
sudo pacman -S rust-bindgen
```

内核有个特殊之处就是它不能链接 C 标准库，所以它自然也不能链接 Rust 标准库，所以编译开启 Rust (`CONFIG_RUST`) 内核需要安装 Rust 标准库的源码，这步可以通过 rustup 完成

```
rustup component add rust-src
```

这些完成后，在内核源码树根目录下运行

```
make rustavailable
```

检查编译内核的 Rust 环境是否已经准备 OK.

内核 Rust 构建系统提供对 VSCode **rust-analyzer** 插件的支持(因为 Rust for Linux 不用 Cargo, 所以默认情况下 rust-analyzer server 是无法正常工作的), 运行

```
make rust-analyzer
```
会生成 **rust-project.json**, 有了这个文件，rust-ananlyzer 插件可以运行。但是注意 `rust-analyzer` 目标必须在内核配置过之后才能 make

编译内核第一步，是得到 `.config` 配置文件, 配置项 `Rust support` 的位置在 `General setup` -> `Rust support`

```
make menuconfig
```

# Learn Rust from Rust for Linux

就像之前的 [Learn Makefile from Linux Build System](https://lucmann.github.io/utils/make/) 一样，这次我想一边阅读 Linux 内核中的 Rust 驱动代码，一边学习 Rust 语言，好处是可以了解一个实际的项目中，像 Rust 这样的语言的最佳实践是怎样的，这里就当是学习笔记吧

# Nightly vs. Stable rustc

Nightly rustc 每日自动构建，包含了所有最新的语言功能和标准库功能，所以稳定性也低，有可能有Bug。Rust for Linux 的官方文档虽然没有明确说构建内核 Rust 使用 Stable 编译器还是 Nightly 编译器，但最好使用 Stable, 原因是 Nightly 每天可能都有更新，Rust for Linux 也每天都在更新，如果使用 Nightly 版本的编译器，就需要二者同时都是最新，否则可能出现像下面的情况，编译器的版本号已经是 `1.91.0` 了，但相应的功能还没更新上(对于 Nightly 版本来说，所有的新功能不是在一天内更新进去的)

```rust $srctree/scripts/generate_rust_target.rs
        if cfg.rustc_version_atleast(1, 91, 0) {
            ts.push("target-pointer-width", 64);
        } else {
            ts.push("target-pointer-width", "64");
        }
```

- `rustup install nightly`
    - `nightly-x86_64-unknown-linux-gnu installed - rustc 1.91.0-nightly (1ebbd87a6 2025-08-11)`
- `rustup update nightly`
    - 更新 nightly 版本的 rustc
        ```
        info: syncing channel updates for 'nightly-x86_64-unknown-linux-gnu'

            nightly-x86_64-unknown-linux-gnu unchanged - rustc 1.92.0-nightly (c8905eaa6 2025-09-28)

        info: checking for self-update
        ```
- `rustup default nightly`
    - `nightly-x86_64-unknown-linux-gnu unchanged - rustc 1.91.0-nightly (1ebbd87a6 2025-08-11)`
    - `rustc --version`
- `rustup component add rust-src`
    - `make rustavailable`
    - 安装 rustc nightly 后同样必须安装 nightly rust-src,否则 `make rustavailable` 会失败
- `rustup default nightly` or `rustup default stable`
    - 在 nightly 和 stable 之间切换
- `rustup override set stable`
    - 只改变 current working directory 的编译器版本，不影响系统的编译器版本配置 

# Macros

Rust 中的宏虽说是强大，但也非常复杂，单看它文档里的**声明宏**的语法定义就头大了，更不用说过程宏了。但 Rust 的宏强大就强大在**过程宏**，声明宏只是减少代码量，而过程宏能让编译器**自动生成代码**。

- [https://doc.rust-lang.org/reference/macros-by-example.html](https://doc.rust-lang.org/reference/macros-by-example.html)

## Declarative Macros 声明式宏 `macro_rules!`

Rust 声明式宏给人的感觉就好像是把编译器前端的**词法分析(Lexical)**, **语法分析(Syntactic)** 开放给了用户，用户根据这套语法分析逻辑自己定义代码。定义一个声明式宏的一般语法是

```rust
macro_rules! MACRO_NAME {
    ( PATTERN ) => {
        .....
    }
}
```

声明式宏有点像**模式匹配**(至少它们都用到 `=>`🐶), 调用宏时传入的**那段字串**会与 **PATTERN** 的结构匹配，一旦匹配就会按**展开规则**进行展开，这里的**展开规则**本身的写法很复杂，但也很强大。

内核中的 `dev_dbg` 就是一个声明式宏，实际上 `crate::dev_printk` 还是一个声明式宏

- dev_dbg 宏定义
```rust
#[macro_export]
macro_rules! dev_dbg {
    ($($f:tt)*) => { $crate::dev_printk!(pr_dbg, $($f)*); }
}
```

- dev_printk 宏定义
```rust
#[doc(hidden)]
#[macro_export]
macro_rules! dev_printk {
    ($method:ident, $dev:expr, $($f:tt)*) => {
        {
            ($dev).$method(::core::format_args!($($f)*));
        }
    }
}
```

- dev_dbg 宏使用
```rust
dev_dbg!(dev, "GPU instance built\n");
```

从上面3段代码看，Rust 声明宏与 C 宏本质上差不多，形式上差得多。从 `dev_dbg` 的定义看，这个宏的参数是一个 **TokenTree (tt)**, 这个参数的名字就是 `$f`, 它的展开就是以 `pr_dbg` 这个**Identifier (ident)** 作为第一个参数调用 `dev_printk` 宏，其它参数透传过去。

而 `dev_printk` 宏有3个参数：

- 第1个是一个 **Identifier**, 参数名是 `$method`
- 第2个是一个 **Expression**, 参数名是 `$dev`
- 第3个是一个 **TokenTree**,  参数名是 `$f`

`dev_dbg!(dev, "GPU instance built\n")` 最终展开后就是 `dev.pr_dbg(::core::format_args!("GPU instance built\n"))`

## Procedural Macros 过程宏 Rust code that generates rust code

- Function-like macros
- derive macros
- attribute macros

### Function-like macros

函数式过程宏的一般语法是

```rust
mod foo;

#[proc_macro_derive(Name)]
pub fn foo_bar(input: TokenStream) -> TokenStream {
    foo::foo_bar(input.into())).into()
}
```
具体的宏的实现(如何把input TokenStream 变成结果 TokenStream) 一般在一个单独的 rust mod 里。

### derive macros `#[derive(Debug)]`

### attribute macros `#[bitfield]`

- Outer attribute
    * `#[...]`
        - 仅修饰紧跟着它的项 (fn, struct, trait)
- Inner attribute
    * `#![...]`
        - 修饰它所在的整个项 (crate, mod)
            
# Traits

# Closures 闭包(匿名函数)

Rust closure 和 C++ 的 lambda 类似，都是实现**匿名函数**，都是一种语法糖，也都是**零开销抽象**。Rust closure 的一般语法是

```rust
let a_closure = || { .... };
// now you can call it
a_closure();
```
- 当函数体只有一个表达式时，花括号都可以省略
- Rust closure 的变量捕获相对简单，因为它是由编译器隐式地自动完成的(根据你使用变量的方式)
- 下面是一个使用 closure 的例子

    ```rust
    let _: Result = util::wait_on(Delta::from_micros(10), || None);
    ```
    - `util::wait_on()` 的第2个参数的类型 `F`, 是一个实现了 `Fn()` trait 且返回值类型是 `Option<R>` 的类型
    - 第2个实参： `|| None` (这个闭包够简单吧) 符合这个类型的要求
    - `Option<T>` 是个泛型枚举类型，`None` 就是它的一个枚举值
    - [`util::wait_on()`](https://elixir.bootlin.com/linux/v6.17-rc6/source/drivers/gpu/nova-core/util.rs#L35)的实现如下

        ```rust
        pub(crate) fn wait_on<R, F: Fn() -> Option<R>>(timeout: Delta, cond: F) -> Result<R> {
            let start_time = Instant::<Monotonic>::now();

            loop {
                if let Some(ret) = cond() {
                    return Ok(ret);
                }

                if start_time.elapsed().as_nanos() > timeout.as_nanos() {
                    return Err(ETIMEDOUT);
                }
            }
        }
        ```

# 参考
- [The Cargo Book](https://doc.rust-lang.org/cargo/index.html)
- [Rust 语言圣经](https://course.rs/basic/variable.html)👍
- [Rustling](https://github.com/rust-lang/rustlings)🦀
- [Rust for Linux](https://rust-for-linux.com/)👀
- [rust.docs.kernel.org](https://rust.docs.kernel.org/kernel/)
- [The best way to learn procedural macros is by writing them](https://github.com/dtolnay/proc-macro-workshop)
- [The Rust Programming Language - Macros](https://doc.rust-lang.org/book/ch20-05-macros.html)
- [The Rust Programming Language - Closures: Anonymous Functions That Capture Their Environment](https://doc.rust-lang.org/book/ch13-01-closures.html)
