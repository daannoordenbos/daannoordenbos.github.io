---
title: "Sampling from a Gamma distribution"
mathjax: true
layout: post
---

Computer generated random numbers are an interesting field of study, computers are deterministic so they cannot make random numbers, yet the 'random' sequences that are created look indistinguishable from true random sequences. Most of the time computers generate random 32 or 64 bit integers, these integers are the building blocks for all other random numbers that are generated. Specifically, they can be turned into to random uniform numbers between 0 and 1. With these numbers we can sample from various other continuous and discrete distributions.
In this post we will build up to sampling from a Gamma distribution, which is will hopefully make clear how we can sample from any distribution.

# Inverse method
First of all, let us recall a nice result from probability theory. Let \\(X\\) have CDF \\(F_X(t)\\), then \\(X\sim F_X^{-1}(U)\\) where \\(U\sim\text{unif}(0,1)\\). So, if the CDF of the distribution has an easy to compute inverse we can sample the distribution directly. This allows us to sample from distributions like the exponential, weibull, and even exotic ones like the log-logistic.

# Normal random variables
The inverse method is not applicable to the normal distribution, since the CDF does not have a simple inverse. This is where the interesting part of sampling from distributions starts, as we need to use clever insights to sample from complicated distributions. To sample from the normal distribution we will need to make the following observation. Let \\(X, Y\sim N(0, 1)\\) be independent of each other, then by changing from Cartesian to polar coordinates we get that \\((X, Y) \sim (\cos(A)\sqrt{R}, \sin(A)\sqrt{R})\\) with \\(A\sim\text{unif}(0,2\pi)\\) and \\(R=X^2+Y^2\sim\chi^2\sim\text{expo}(1)\\). \\(A\\) and \\(R\\) are easy to sample from, so we can obtain samples from a normal distribution.

# Rejection sampling
To sample from an arbitrary random variable \\(X\\) with PDF \\(f_X(t)\\) and support \\(I\\), we need to employ a Monte Carlo like method. Firstly, let \\(L=\text{max}f_X(t)\\) so we can bound \\(f_X(t)\\) by the set \\(A\\) given by,

$$A=\{(x,y)\in\mathbb{R}^2|x\in I, y\in[0, L]\}.$$
Now sample a point, \\(s_1=(p_x,p_y)\\), uniformly from \\(A\\), if \\(\frac{p_y}{L}\le f_X(t)\\) we keep the point, otherwise we repeat until this holds. After (repeated) sampling we have that \\(s_x\sim X\\). The figure below gives an obvious intuition as to why this holds.

Whilst the method described above works in theory, it is bad in practice since, we will have to resample many times.
