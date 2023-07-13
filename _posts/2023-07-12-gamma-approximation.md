---
title: "Curious linear asymptote"
mathjax: true
layout: post
---

In this blog post we are going to be considering the function \\(f:\mathbb{R}_+\to\mathbb{R}\\) given by \\(f(x)= \Gamma\left(1+\frac{1}{x}\right)^x\\).  As \\(f(x)\\) looks remarkably smooth and \\(xf(x)\\) is almost linear, we will determine the linear asymptote of \\(xf(x)\\). To determine this linear asymptote its slope and intercept have to be calculated. Evidently the slope is given by

$$\lim\limits_{x\to\infty}\frac{xf(x)}{x}=\lim\limits_{x\to\infty}f(x).$$

To compute this limit we will use the Taylor expansions of the Gamma function. Famously, \\(\Gamma(1+x)=1-\gamma x + O(x^2)\\), where \\(\gamma\\) is the Euler-Mascheroni constant. Now,

$$\begin{align*}\lim\limits_{x\to\infty}f(x)&=\lim\limits_{x\to\infty}\Gamma\left(1+\frac{1}{x}\right)^x\\&=\lim\limits_{x\to\infty}\left(1-\frac{\gamma}{x}+O(x^{-2})\right)^x\\&=\lim\limits_{x\to\infty}\left(1-\frac{\gamma}{x}\right)^x=e^{-\gamma}.\end{align*}$$

Where we have use of the fact that the term \\(O(x^{-2})\\) is dominated by \\(-\frac{\gamma}{x}\\). The intercept of the linear asymptote is given by the following limit

$$\lim\limits_{x\to\infty}\left(x \Gamma\left(1+\frac{1}{x}\right)^x-xe^{-\gamma}\right).$$

Again a Taylor expansion is used to determine this limit. The expansion in question is the following

$$\log\Gamma(1+x)=-\gamma x+\sum\limits_{k=2}^{\infty}\frac{(-1)^k\zeta(k)}{k}x^k,$$

where \\(\zeta(s)=\sum\limits_{k=1}^{\infty}k^{-s}\\). This Taylor expansion yields that, \\(\log\Gamma(1+x)=-\gamma x + \frac{\pi^2}{12}x^2+O(x^3)\\). Now,

$$\begin{align*}\lim\limits_{x\to\infty}\left(x \Gamma\left(1+\frac{1}{x}\right)^x-xe^{-\gamma}\right)&=\lim\limits_{x\to\infty}x \left(\Gamma\left(1+\frac{1}{x}\right)^x-e^{-\gamma}\right)\\&=\lim\limits_{x\to0}\frac{\Gamma\left(1+x\right)^\frac{1}{x}-e^{-\gamma}}{x}\\&=\lim\limits_{x\to0}\frac{\mathrm{d}}{\mathrm{d}x}\left(\Gamma\left(1+x\right)^\frac{1}{x}-e^{-\gamma}\right)\\&=\lim\limits_{x\to0}\left(\frac{\Gamma'(1+x)}{\Gamma(1+x)}\frac{1}{x}-\frac{1}{x^2}\log\Gamma(1+x)\right)\Gamma(1+x)^{\frac{1}{x}}\\&=\lim\limits_{x\to0}\frac{1}{x^2}\left(x\frac{\mathrm{d}}{\mathrm{d}x}\log\Gamma(1+x)-\log\Gamma(1+x)\right)\Gamma(1+x)^{\frac{1}{x}}\\&=\lim\limits_{x\to0}\frac{1}{x^2}\left(-\gamma x+\frac{\pi^2}{6}x^2+xO(x^2)+\gamma x - \frac{\pi^2}{12}x^2+O(x^3)\right)\Gamma(1+x)^{\frac{1}{x}}\\&=\lim\limits_{x\to0}\frac{1}{x^2}\left(\frac{\pi^2}{12}x^2+O(x^3)\right)\Gamma(1+x)^{\frac{1}{x}}\\&=\frac{\pi^2}{12}e^{-\gamma}\end{align*}$$

The linear asymptote is therefore \\(frac{\pi^2}{12}e^{-\gamma}+e^{-\gamma}x\\), and \\(\Gamma\left(1+\frac{1}{x}\right)^x\approx \frac{\pi^{2}+12x}{12x}e^{-\gamma}\\) for large \\(x\\). This approximation is within one procent of the true value for \\(x\ge1.7844\\).
