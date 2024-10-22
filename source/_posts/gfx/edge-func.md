---
title: Edge Function - A Little Math You Want to Know about CG
date: 2024-10-22 11:38:10
tags: [OpenGL]
categories: graphics
---

# Definition

{% katex %}
E_0_1(P)=(P.x - V_0.x) * (V_1.y - V_0.y) - (P.y - V_0.y) * (V_1.x - V_0.x)
{% endkatex %}

更好记的形式:

{% katex %}
\text{\textbardbl} \overrightharpoon{V_1V_0} \times \overrightharpoon{PV_0} \text{\textbardbl}
{% endkatex %}

# Reference
[A Parallel Algorithm for Polygon Rasteriztion](https://www.cs.drexel.edu/~deb39/Classes/Papers/comp175-06-pineda.pdf)
