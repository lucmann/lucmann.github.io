---
title: GCC __attribute__
date: 2020-02-26 10:49:57
tags: [gcc]
categories: programming
---

`__attribute__` is a keyword introduced by GCC. It is regarded as an extension of a language. It helps the compiler optimize calls, check code more carefully for correctness, control memory placement and code generation options.

<!--more-->

## Syntax
```
__attribute__ ((attribute-list))
```

where an attribute-list is a possibly empty comma-separated sequence of attributes. Say:

```
static void _init( void ) __attribute__((constructor));
```

## Categories
- [Function Attributes](https://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html#Function-Attributes)
- [Variable Attribute](https://gcc.gnu.org/onlinedocs/gcc/Variable-Attributes.html#Variable-Attributes)
- [Type Attributes](https://gcc.gnu.org/onlinedocs/gcc/Type-Attributes.html#Type-Attributes)
- [Label Attributes](https://gcc.gnu.org/onlinedocs/gcc/Label-Attributes.html#Label-Attributes)
- [Enumerator Attributes](https://gcc.gnu.org/onlinedocs/gcc/Enumerator-Attributes.html#Enumerator-Attributes)
- [Statement Attributes](https://gcc.gnu.org/onlinedocs/gcc/Statement-Attributes.html#Statement-Attributes)

Let's say to specify an attribute of variables.

```
extern __thread struct _glapi_table * _glapi_tls_Dispatch
    __attribute__((tls_model("initial-exec")));
```

where the `tls_model` attribute sets thread-local storage model of a particular thread variable, overriding `-ftls-model=` command-line switch on a per-variable basis. The [`tls_model`](https://docs.oracle.com/cd/E53394_01/html/E54813/man-tlsam.html#scrolltoc) argument should be one of `global-dynamic`, `local-dynamic`, `initial-exec`, or `local-exec`.

### [Visibility](http://anadoxin.org/blog/control-over-symbol-exports-in-gcc.html)

```
__attribute__((visibility("default")));
```

this attribute involves with the visibility of exported symbols in a shared object, overriding `-fvisibility=hidden` command-line option during the compilation. The visibility argument should be one of `default`, `hidden`, `internal`, or `protected`.
