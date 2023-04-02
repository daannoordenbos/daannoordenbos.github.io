---
title: "The Wishart distribution"
mathjax: true
layout: post
---

In Bayesian statistics conjugate priors are often employed to get analytically tractable posteriors. One such example of a conjugate prior is the inverse Wishart prior for the covariance matrix of a normal distribution.
Consider the following distribution, \\(\boldsymbol{x}\sim\mathcal{N}(\boldsymbol{\mu},\boldsymbol{\Sigma})\\) from which we observe samples \\(\boldsymbol{x}_i\\). If we take \\(\boldsymbol{\mu}\\) as known, \\(\boldsymbol{\Sigma}\\) as unknown, and assume that \\(\boldsymbol{\Sigma}\sim\mathrm{IW}(\boldsymbol{\Psi}, \nu)\\), then the posterior is,
$$\boldsymbol{\Sigma}|\boldsymbol{x}_1\dots,\boldsymbol{x}_n\sim\mathrm{IW}\left(\boldsymbol{\Psi}+\sum\limits^n_{i=1}(\boldsymbol{x}_i-\boldsymbol{\mu})(\boldsymbol{x}_i-\boldsymbol{\mu})',\nu + n \right).&&
Where \\(\mathrm{IW}\\) denotes the inverse Wishart distribution which is the multidimensional generalization of the inverse Gamma distribution. The inverse Wishart distribution is defined as follows, if \\(\boldsymbol{G}_1,\dots,\boldsymbol{G}_{\nu}\sim\mathcal{N}\left(\boldsymbol{0},\Psi^{-1}\right)\\) and \\(\boldsymbol{S}=[\boldsymbol{G}_1\dots\boldsymbol{G}_{\nu}]\\), then \\(\boldsymbol{S}\boldsymbol{S}'\sim\mathrm{IW}(\boldsymbol{\Psi},\nu)\\) (Fink, 1997).
Using the definition we can easily sample from an inverse Wishart distribution. Let \\(\boldsymbol{X}\sim \text{W}(I_k, n)\\) then \\(L \boldsymbol{X}^{-1}L'\sim \text{IW}(\boldsymbol{\Psi}, n)\\), where \\(L\\) is the Cholesky decomposition of \\(\boldsymbol{\Psi}\\). The following C++ code can do this superfast.

{% highlight c++ %}
double randomGamma (double alpha, double beta)
{
    double d, c, x, xSquared, v, u;
    if (alpha >= 1.0)
    {
        d = alpha - 1.0 / 3.0;
        c = 1.0 / sqrt(9.0 * d);
        while (true)
        {
            v = -1;
            while (v <= 0)
            {
                x = randomNormal(0, 1);
                v = 1.0 + c * x;
            }
            v = v * v * v;
            u = randomUniform();
            xSquared = x * x;
            if (u < 1.0 - 0.0331 * xSquared * xSquared || log(u) < 0.5 * xSquared + d * (1.0 - v + log(v)))
                return d * v / beta;
        }
    }
    else
    {
        double g = randomGamma(alpha + 1.0, 1.0);
        double w = randomUniform(0, 1);
        return g * pow(w, 1.0 / alpha) / beta;
    }
}
{% endhighlight %}
