---
title: "Approximating the best move probabilities"
mathjax: true
layout: post
---

# Introduction
In this post we are going to be considering the following problem. In some game state there are \\(n\\) possible moves that can be played. Each of these \\(n\\) child nodes is evaluated by a rollout policy, resulting in \\(a_n\\) wins and \\(b_n\\) loses. The question then is, what is the probability that child node k has the highest winning probability?
