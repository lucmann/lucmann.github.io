---
title: Edge Function - A Little Math You Want to Know about CG
date: 2024-10-22 11:38:10
tags: [OpenGL]
categories: graphics
---

<div class="tenor-gif-embed" data-postid="18866305" data-share-method="host" data-aspect-ratio="2.40601" data-width="100%"><a href="https://tenor.com/view/4points-gif-18866305">4points GIF</a>from <a href="https://tenor.com/search/4points-gifs">4points GIFs</a></div> <script type="text/javascript" async src="https://tenor.com/embed.js"></script>

<!--more-->

# Edge 函数定义

{% katex %}
E_{01}(P)=(P.x - V_0.x) * (V_1.y - V_0.y) - (P.y - V_0.y) * (V_1.x - V_0.x)
{% endkatex %}

- $E_{01}(P) > 0$ 当 $P$ 在 $\overrightharpoon{V_0V_1}$ 的**右边**
- $E_{01}(P) = 0$ 当 $P$ 在 $\overrightharpoon{V_0V_1}$ 的**边上**
- $E_{01}(P) < 0$ 当 $P$ 在 $\overrightharpoon{V_0V_1}$ 的**左边**

更好记的形式: $\lVert \overrightharpoon{V_1V_0} \times \overrightharpoon{PV_0} \rVert$

# 参考
[A Parallel Algorithm for Polygon Rasteriztion](https://www.cs.drexel.edu/~deb39/Classes/Papers/comp175-06-pineda.pdf)
