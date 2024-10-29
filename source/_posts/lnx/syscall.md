---
title: Syscall vs C routines
date: 2023-06-13 18:30:15
tags: [C/C++]
categories: linux
---

系统调用是 Linux 用户态程序与内核通信的接口。 每个特定的文件系统都会在自己的 `file_operations` 里提供各种文件操作接口，像 `.open`, `.close`, `.ioctl`。系统调用会通过 VFS 的接口调用这些具体的实现，而对于应用程序来说，一般不会直接使用系统调用，而是调用 C 库函数 (C routines).

ioctl 这个系统调用的声明大概是这样:

```
COMPAT_SYSCALL_DEFINE3(ioctl, unsigned int, fd, unsigned int, cmd, compat_ulong_t, arg)
```

ioctl 的 C 库函数 (Aarch64 实现)：

```
	.text
ENTRY(__ioctl)
    /* move ioctl syscall number to x8 register */
	mov	x8, #__NR_ioctl
    /* sign extend w0 to x0 */
	sxtw	x0, w0
    /* issue software interrupt to invoke syscall */
	svc	#0x0
    /* add 4095 to x0 and set flags according to result */
	cmn	x0, #4095
    /* if carry bit is set (previous addition carrys out), branch to Lsyscall_error */
	b.cs	.Lsyscall_error
	ret
PSEUDO_END (__ioctl)

/* symbol management, making __ioctl and ioctl effectively the same function */
libc_hidden_def (__ioctl)
weak_alias (__ioctl, ioctl)
```

<!--more-->

# 返回值

因为系统调用失败时返回的都是 `-errno`, 所以当失败时 `cmn x0, #4095` 的结果必然导致 carry 标志位被置，后面的 `b.cs` (carry set branch) 则会跳转到 `Lsyscall_error` 标签继续执行，所以 C 库函数 `ioctl()` 失败时的返回值是多少，就要看 `Lsyscall_error` 的处理了。

```
# if !IS_IN (libc)
#  define SYSCALL_ERROR  .Lsyscall_error
#  if RTLD_PRIVATE_ERRNO
#   define SYSCALL_ERROR_HANDLER				\
.Lsyscall_error:						\
	adrp	x1, C_SYMBOL_NAME(rtld_errno);			\
	neg     w0, w0;						\
	str     w0, [x1, :lo12:C_SYMBOL_NAME(rtld_errno)];	\
	mov	x0, -1;						\
	RET;
#  else

#   define SYSCALL_ERROR_HANDLER				\
.Lsyscall_error:						\
	adrp	x1, :gottprel:errno;				\
	neg	w2, w0;						\
	ldr	PTR_REG(1), [x1, :gottprel_lo12:errno];		\
	mrs	x3, tpidr_el0;					\
	mov	x0, -1;						\
	str	w2, [x1, x3];					\
	RET;
#  endif
# else
#  define SYSCALL_ERROR __syscall_error
#  define SYSCALL_ERROR_HANDLER                                 \
.Lsyscall_error:                                                \
	b	__syscall_error;
# endif
```

可以看到无论是 `#if` 或 `#else` 哪个分支， 都有 `mov x0, -1`, 所以结论就是 C 库函数 `ioctl()` 的返回值：成功 `0`, 失败 `-1`. 相应的错误码会保存在 `errno`.

这里再看一下 [libdrm](https://gitlab.freedesktop.org/mesa/drm) 库对 ioctl 的封装函数  `drmIoctl()`, 它的返回值仍然是：成功 `0`, 失败 `-1`. 只不过它对 `EINTR`(4, "Interrupted system call") 和 `EAGAIN`(11, "Try again") 这两种错误码进行了重试。

```
drm_public int
drmIoctl(int fd, unsigned long request, void *arg)
{
    int ret;

    do {
        ret = ioctl(fd, request, arg);
    } while (ret == -1 && (errno == EINTR || errno == EAGAIN));
    return ret;
}
```

再看一个调用 `drmIoctl()` 的 libdrm 的库函数，比如 `drmSyncobjWait()`, 它的返回值在失败时变成了 `-errno`, 当然这样避免让上层调用者去检查 `errno`, 但却打破了接口的一致性，有利有弊吧。

```
drm_public int drmSyncobjWait(int fd, uint32_t *handles, unsigned num_handles,
                              int64_t timeout_nsec, unsigned flags,
                              uint32_t *first_signaled)
{
    struct drm_syncobj_wait args;
    int ret;

    memclear(args);
    args.handles = (uintptr_t)handles;
    args.timeout_nsec = timeout_nsec;
    args.count_handles = num_handles;
    args.flags = flags;

    ret = drmIoctl(fd, DRM_IOCTL_SYNCOBJ_WAIT, &args);
    if (ret < 0)
        return -errno;

    if (first_signaled)
        *first_signaled = args.first_signaled;
    return ret;
}
```