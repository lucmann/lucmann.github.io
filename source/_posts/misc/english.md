---
title: English Corner
date: 2021-12-18 20:20:40
tags: English
categories: misc
---

> A language that doesn't affect the way you think about programming, is not worth knowing.
>
> -- <cite>Alan J. Perlis, Recipient of 1966 Turing Award</cite>

<!--more-->

# IRC

## [IRC(Internet Relay Chat)](https://zh.wikipedia.org/wiki/IRC)

- 一种应用层协议，用于聊天沟通 
- 一种开源社区常用的通讯方式
- 服务端口号6667[明文], 6697[加密]
- [webchat.oftc.net](https://webchat.oftc.net/)是其中一种基于web的客户端

<!--more-->

## 频道 Channels

- [#dri-devel](https://webchat.oftc.net/)
- [#llvm](https://webchat.oftc.net/)
- [#panfrost](https://webchat.oftc.net/)

如果你想知道谁在跟你交流或你的问题应该@谁，请查看[Who's Who on IRC](https://dri.freedesktop.org/wiki/WhosWho/)

## 短句 Acronym/Phrases

| Acronym | Phrase | 中文 |
|:--------|:-------|:-----|
| AFAICT | As far as I can tell | 个人认为 |
| AFAIK | As far as I know | 据我了解 |
| AFAIU | As far as I understand | 按我理解 |
| BTW | By the way | 顺便 |
| FWIW | For what it's worth | 陈述见解或建议的谦逊说法，比如我觉得这是有用的信息，但是从听者的角度未必也是 |
| FYI | For your information | 供您参考 |
| HTH | Hope this helps | 希望奏效 |
| IDK | I don't know | 我不知道 |
| IIRC | If I recall correctly / If I remember correctly | 如果我没有记错 |
| IMHO | In my humble opinion | 在下愚见 |
| IMO | In my opinion | 依我之见 |
| LGTM | Looks good to me | 我看不错 |
| TBH | To be honest | 老实讲 |
| WFM | Work for me | 对我来说可以 |

# 粘贴服务 Paste Service

在 IRC 上交流时，有时可能需要粘贴 Log，Config 之类的大段文本，但这些又不便直接粘到 IRC 上，这时以下基于 Web 的 Paste 服务可能帮得上你

- [http://ix.io](http://ix.io)
- [https://paste.rs](https://paste.rs)
- [https://bpaste.net](https://bpaste.net)
- [https://gist.github.com](https://gist.github.com)

# 短语汇

## Wiggle Room

（进行解释或表达意见所留的）余地、回旋空间

> there's pretty much zero wiggle room in semantics

在语义方面几乎没有任何余地

## Hand Roll/Hand-rolled

"Hand roll"或"hand-rolled"是一个英语词汇，通常用于描述手工制作的东西。这个词汇可以用来形容手工制作的食品，如手卷寿司（hand-rolled sushi），也可以用来形容手工制作的烟卷（hand-rolled cigarettes）或手工制作的纸卷（hand-rolled paper）。在其他领域，"hand roll"或"hand-rolled"也可以指手工制作的家具、纺织品、珠宝、音乐乐器等。

>  I think we're way past the point where drivers hand-rolling entire gem from nothing is ready for upstream


# 软件开发中的常用词汇

## rubberstamp

- 例句

    > Willing to rubberstamp regardless of this is helping v10 r/e, but I would like to understand what we're up against here.

    无论这个是否有助于 v10 版本硬件的逆向，我都愿意给你通过(不检查), 但我还是想明白我们所面临的问题。

## scaffolding

scaffolding 是指脚手架，通常用于建筑施工中支撑人员和材料的临时结构。在计算机科学中，scaffolding 可以指代软件开发过程中的一些临时结构和代码，用于辅助开发人员快速完成某个功能或实现某个目标。这些临时结构和代码通常会被移除或重构，只留下最终的代码和结构。

- 例句

    > have the minimal scaffolding to support the preempt-ctx fence in drm_sched_entity
