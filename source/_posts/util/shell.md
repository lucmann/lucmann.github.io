---
title: Advanced Shell
date: 2022-06-19 22:18:16
tags: shell
categories: utilities
---

# Bash Shell

## Parameter Expansion

### Use an alternate value

```bash
${var:+WORD}
${var+WORD}
```

如果 `var` 没有设置或为空，则这个变量展开为 Nothing (注意：不是空 empty, 是 nothing), 如果被设置了（不包括被设置成空），它展开为 `+` 后面的 WORD.

如果冒号被省略，则 `var` 即使被设置为空，它也展开为 `+` 后面的 WORD


### Indirect

```bash
${!var}
```

如果 `var` 的值是 `MESA_DEBUG`, 那么这个形式展开后是变量 `MESA_DEBUG` 的值，例如 `export MESA_DEBUG=1`, `var=MESA_DEBUG`, 则最后的展开结果是 `1`

### Quoted

```bash
${var@Q}
```

带 `@Q` 指变量展开后的值被单引号引起来，例如 `export ABC=abc`, `echo "ABC=${ABC@Q}"` 的结果是 `ABC='abc'`
