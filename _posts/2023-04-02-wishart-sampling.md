---
title: "The Wishart distribution"
mathjax: true
layout: post
---

In Bayesian statistics conjugate priors are often employed to get analytically tractable posteriors. One such example of a conjugate prior is the inverse Wishart prior for the covariance matrix of a normal distribution.
Consider the following distribution, \\(\boldsymbol{x}\sim\mathcal{N}(\boldsymbol{\mu},\boldsymbol{\Sigma})\\) from which we observe samples \\(\boldsymbol{x}_i\\). If we take \\(\boldsymbol{\mu}\\) as known, \\(\boldsymbol{\Sigma}\\) as unknown, and assume that \\(\boldsymbol{\Sigma}\sim\mathrm{IW}(\boldsymbol{\Psi}, \nu)\\), then the posterior is,
$$\boldsymbol{\Sigma}|\boldsymbol{x}_1\dots,\boldsymbol{x}_n\sim\mathrm{IW}\left(\boldsymbol{\Psi}+\sum\limits^n_{i=1}(\boldsymbol{x}_i-\boldsymbol{\mu})(\boldsymbol{x}_i-\boldsymbol{\mu})',\nu + n \right).&&
Where \\(\mathrm{IW}\\) denotes the inverse Wishart distribution which is the multidimensional generalization of the inverse Gamma distribution. The inverse Wishart distribution is defined as follows, if \\(\boldsymbol{G}_1,\dots,\boldsymbol{G}_{\nu}\sim\mathcal{N}\left(\boldsymbol{0},\Psi^{-1}\right)\\) and \\(\boldsymbol{S}=[\boldsymbol{G}_1\dots\boldsymbol{G}_{\nu}]\\), then \\(\boldsymbol{S}\boldsymbol{S}'\sim\mathrm{IW}(\boldsymbol{\Psi},\nu)\\) (Fink, 1997).
Using the definition we can easily sample from an inverse Wishart distribution. Let \\(\boldsymbol{X}\sim \text{W}(I_k, n)\\) then \\(L \boldsymbol{X}^{-1}L'\sim \text{IW}(\boldsymbol{\Psi}, n)\\), where \\(L\\) is the Cholesky decomposition of \\(\boldsymbol{\Psi}\\). The following C++ code, using the Eigen library, can do this superfast.

{% highlight c++ %}
Eigen::MatrixXd randomInverseWishart(Eigen::MatrixXd& sigma, int n)
{
    /* Generates an inverse Wishart variate by its definition */
    int dim = sigma.rows();
    Eigen::MatrixXd scatter(dim, n);
    double u1, u2;
    // Making use of the fact that we can access the memory as if
    // it is a one dimensional array.
    for (int i = 0; i <= dim * n - 2; i += 2)
    {
        u1 = randomUniform();
        u2 = randomUniform();
        scatter(i    ) = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
        scatter(i + 1) = sqrt(-2.0 * log(u1)) * sin(2.0 * M_PI * u2);
    }
    scatter(dim * n - 1) = randomNormal();
    scatter = scatter * scatter.transpose();
    scatter = scatter.inverse();

    Eigen::MatrixXd L = sigma.llt().matrixL();
    Eigen::MatrixXd variate(dim, dim);
    variate = L * scatter * L.transpose();
    return variate;
}
{% endhighlight %}
