---
title: "Approximating the best move probabilities"
mathjax: true
layout: post
---

# Introduction
In this post we are going to be considering the following problem. In some game state there are \\(n\\) possible moves that can be played. Each of these \\(n\\) child nodes is evaluated by a rollout policy, resulting in \\(a_k\\) wins and \\(b_k\\) loses for child \\(k\\). The question then is, what is the probability that child node \\(k\\) has the highest winning probability?

# Exact method
Let us start at the back, what is the distribution of the winning probability for child \\(k\\)? Luckily this problem has already been solve, the winning probability, \\(w_k\\), has a beta distribution. More specifically, \\(w_k\sim\text{Beta}(a_k + 1, b_k + 1)\\). With this we can solve the problem analytically, 

$$P(x_k>\text{max}(x_1,\dots,x_{k-1},x_{k+1},\dots,x_n)=\int_0^1f_{w_k}(x)\prod^n_{i\ne k}F_{w_i}(x)dx.$$

However, this integral cannot be reduced any further. Computing it numerically is fine for relatively small \\(n\\), but for quite large \\(n\\) we find that \\(\prod^n_{i\ne k}F_{w_i}(x)\\) becomes too steep. So instead we use a different method,

$$P(x_k>\text{max}(x_1,\dots,x_{k-1},x_{k+1},\dots,x_n)\approx\int_t^1f_{w_k}(x)dx\text{, such that } \sum^n_{i=1}\int_t^1f_{w_i}(x)dx=1.$$

Finding the appropriate \\(t\\) is not difficult, we can use a simple bisection method. The reason that this method is a good approximation is due to the fact that for larger \\(n\\),

$$\prod^n_{i\ne k}F_{w_i}(x)\approx\begin{cases}0\text{, for } x < t\\1\text{, for } x\ge t\end{cases}$$

This method also makes intuitive sense since it looks at the tail ends of the distributions. Which is sensible because we are looking at the maximum. 


