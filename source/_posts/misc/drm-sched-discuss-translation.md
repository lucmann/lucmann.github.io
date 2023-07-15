---
title: 我在邮件列表里学英语
date: 2023-07-15 20:20:40
tags: English
categories: misc
---

> Communication is hard, especially with all the different personalities, languages, and
> cultures involved in an international community like this.
>
> -- <cite>Faith Ekstrand</cite>

> It is rules and not our individual smarts that keep us from making mistakes.
>
> -- <cite>Faith Ekstrand</cite>

<!--more-->

已经订阅 dri-devel@lists.freedesktop.org 有一年多了，看到过几次激烈的关于 drm 子系统里的像 drm_sched, dma_fence 的讨论。慢慢地我发现在翻译工具的帮助下，邮件列表里的 e-mail thread 可以成为很好的学习英语的地方。这里的表达感觉介于正式的书面语和随意的口语之间，更重要的，在讨论当中有很多技术性的专业词汇及俚语。这些不仅有助于提高英语水平（读写，不包括说听），而且有助于熟悉技术讨论中常用的表述习惯和语句组织。 

# 翻译1

And, to be clear, for the purposes of this discussion, we're ALL
idiots, myself included.  If there's one thing the DRM community has
learned over the years, it's that drivers are so complex that we all
turn into idiots at some point, relative to the complexity of the code
and hardware behavior.  That's why things like dma_fence are written so
incredibly defensively and why we're so harsh about the rules.  It's
the rules and not our individual smarts that keep us from making
mistakes.  (Kinda like Rust, in a way.)  So while I appreciate the
frustration of "I'm just trying to do something that's clearly correct
here", that doesn't mean that then next person to come by and fix a bug
by tweaking that callback isn't going to screw it up irreparably.  That
person may even be you in 6 to 12 months after this e-mail thread is a
distant memory.

并且，就讨论的目的而言，我们都是笨蛋，包括我自己在内。多年来，DRM社区所学到的一件事就是，驱动程序是如此复杂，以至于在某些情况下，相对于代码和硬件行为的复杂性，我们都会变得愚笨。这就是为什么像dma_fence这样的东西被编写得如此谨慎，以及为什么我们对规则要求如此严格的原因。正是规则而不是我们个人的聪明才智防止了我们犯错。（某种程度上类似于Rust）所以虽然我理解“我只是想在这里做一些显然正确的事情”的沮丧，但这并不意味着接下来的人在调整回调函数以修复错误时不会犯下不可挽回的错误。这个人甚至可能就是你，在这个邮件线程成为遥远记忆的6到12个月后。💁🤔

[英文原文](https://patchwork.freedesktop.org/patch/525461/)

