---
title: "Interesting queueing problem"
mathjax: true
layout: post
---
In this post I am going to present an interesting queueing problem, the main intrigue comes from the neat mathematical tricks I will present that one can use to solve this problem.

# Introduction

So, let us consider a G/G/1 queue with the addition that the server can be interrupted (for repairs for example). Let us assume that we know the distributions of the interarrival times \\(X\\) and the service times \\(S_0\\). Then to model these interruptions we make two additional assumptions, firstly that the time between two interruptions is memoryless with mean \\(1/\lambda\\) and secondly that the down times \\(\{R_i\}\\) are i.i.d.
Our goal will be to determine the expected (approximate) waiting time using Sakasewaga's formula.

$$E[W]=\frac{C^2_a+C^2_s}{2}\frac{\rho}{1-\rho}E[S]\text{, (Sakasewaga's formula for one server)}$$

Note that to compute \\(E[W]\\) we need to determine \\(E[S]\\) and \\(E\left[S^2\right]\\), where \\(S\\) is our yet to be determined true service time.

# Mathematical details

Because \\(S\\) is our total service time (including interruptions) we get that it is given by

$$S=S_0+\sum_{i=1}^NR_i$$

Where \\(N\\) denotes the amount of interruptions during the service time. Because we assume that the time between interruptions is memoryless we obtain the conditional distribution of N, namely \\(N\|S_0=s\sim\text{Pois}(\lambda s)\\). Therefore we get that

$$E[N]=E[E[N|S_0]]=E[\lambda S_0] = \lambda E[S_0]$$
