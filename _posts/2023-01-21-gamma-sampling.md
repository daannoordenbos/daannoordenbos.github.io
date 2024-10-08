---
title: "Sampling from a Gamma distribution"
mathjax: true
layout: post
---

Computer generated random numbers are an interesting field of study, computers are deterministic so they cannot make random numbers, yet the 'random' sequences that are created look indistinguishable from true random sequences. Most commonly computers generate random 32 or 64 bit integers, these integers are the building blocks for all other random numbers that are generated. Specifically, they can be turned into to random uniform numbers between 0 and 1. With these numbers we can sample from various other continuous and discrete distributions.
In this post we will build up to and discuss [A Simple Method for Generating Gamma Variables](https://dl.acm.org/doi/pdf/10.1145/358407.358414) by George Marsaglia and Wai Wan Tsang, since the Gamma distribution is very important for sampling other distributions (Chi, Beta, Dirichlet... ). Hopefully the discussed technique will make clear how we can sample from any distribution (in theory).

# Inverse method
First of all, let us recall a nice result from probability theory. Let \\(X\\) have CDF \\(F_X(t)\\), then \\(X\sim F_X^{-1}(U)\\) where \\(U\sim\text{unif}(0,1)\\). So, if the CDF of the distribution has an easy to compute inverse we can sample the distribution directly. This allows us to sample from distributions like the exponential, weibull, and even exotic ones like the log-logistic.

# Normal random variables
The inverse method is not applicable to the normal distribution, since the CDF does not have a simple inverse. This is where the interesting part of sampling from distributions starts, as we need to use clever insights to sample from complicated distributions. To sample from the normal distribution we will need to make the following observation. Let \\(X, Y\sim N(0, 1)\\) be independent of each other, then by changing from Cartesian to polar coordinates we get that \\((X, Y) \sim (\cos(A)\sqrt{R}, \sin(A)\sqrt{R})\\) with \\(A\sim\text{unif}(0,2\pi)\\) and \\(R=X^2+Y^2\sim\chi^2(2)\sim\text{expo}(\frac{1}{2})\\). \\(A\\) and \\(R\\) are easy to sample from, so we can obtain samples from a normal distribution.

# Rejection sampling
To sample from an arbitrary random variable \\(X\\) with PDF \\(f_X(t)\\) and support \\(I\\), we need to employ a Monte Carlo like method. Firstly, let \\(L=\text{max}f_X(t)\\) so we can bound \\(f_X(t)\\) by the set \\(A\\) given by,

$$A=\{(x,y)\in\mathbb{R}^2|x\in I, y\in[0, L]\}.$$

Now sample a point, \\(s_1=(p_x,p_y)\\), uniformly from \\(A\\), if \\(\frac{p_y}{L}\le f_X(t)\\) we keep the point, otherwise we repeat until this holds. After (repeated) sampling we have that \\(s_x\sim X\\). The figure below gives an obvious intuition as to why this holds.
<div style="text-align:center"><img src="/images/rejection.png" width="75%" /></div>
Whilst the method described above works in theory, it is bad in practice since, we will have to resample many times. We can improve this method by using a better bound than just a rectangle, specifically another PDF. Let \\(Y\\) have PDF \\(f_Y(t)\\) such that for some \\(M\\) we have that \\(f_X(t)\le M f_Y(t)\\) for all \\(t\\). Then to sample from \\(X\\) we first sample \\(y\\) from \\(Y\\) and keep it if \\(u<\frac{f_X(y)}{Mf_Y(y)}\\) and otherwise repeat, with \\(u\sim\text{unif}(0,1)\\).

# Gamma sampling
With the preliminaries out of the way, we focus on the case at hand: Sampling from a Gamma distribution. Firstly, let us focus on \\(\alpha > 1\\) and \\(\beta=1\\), the relevant PDF is then given by

$$f_X(t)=\frac{1}{\Gamma(\alpha)}t^{\alpha-1}e^{-t}.$$

Moreover, we define the random variable \\(Y\\) by following PDF,

$$f_Y(t)=\frac{1}{\Gamma(\alpha)}h(t)^{\alpha-1}e^{-h(t)}h'(t) \implies X=h(Y).$$

This subsitution trick can often work to find a an envelope PDF from which we can sample. We will pick an \\(h(\cdot)\\) such that \\(Y\\) is close to a normal distribution, so our rejection rate is low. A desirable \\(h(\cdot)\\) exists, namely \\(h(t)=d(1+ct)^3\\) with \\(d=\alpha - \frac{1}{3}\\) and \\(c=(9d)^{-\frac{1}{2}}\\) (see main paper as to why). Now conveniently (but not coincidentally) we have that for \\(M=\frac{f_X(t)}{f_Y(t)}\\) that \\(f_X(t)\le M f_Y(t)\\) for all \\(t\\). So, we sample \\(x\\) from a standard normal and accept it if 

$$\log{U}\le \frac{1}{2}y^2+d-h(y)+d\log{h(y)}-\log{d}\text{, where }U\sim\text{unif}(0,1).$$

otherwise we repeat. If accepted we return \\(h(y)\\) as our Gamma sample.
# Special case: \\(\alpha<1\\)
When \\(\alpha<1\\), the above method does not work. But, we can still generate the samples with the following identity

$$X_{\alpha}\sim X_{\alpha + 1}U^{1/\alpha}\text{, with }X_p\sim\text{Gamma}(p, 1) \text{ and } U\sim\text{unif}(0,1).$$

We can proof this result using the characteristic functions.

$$X_{\alpha}\sim X_{\alpha + 1}U^{1/\alpha} \Leftrightarrow \log{X_{\alpha}}\sim\log{X_{\alpha+1}}+\frac{1}{\alpha}\log{U}$$

$$\log{X_{\alpha}}\sim\log{X_{\alpha+1}}+\frac{1}{\alpha}\log{U}\Leftrightarrow \phi(\log{X_{\alpha}}) = \phi(\log{X_{\alpha+1}})\phi(\frac{1}{\alpha}\log{U})$$

$$\phi(\log{X_p})=\mathbb{E}\left[e^{it\log{X_p}}\right]=\mathbb{E}\left[X_p^{it}\right]=\frac{\Gamma(p+it)}{\Gamma(p)}.$$

$$\phi(\frac{1}{\alpha}\log{U})=\mathbb{E}\left[e^{\frac{it}{\alpha}\log{U}}\right]=\mathbb{E}\left[U^{\frac{it}{\alpha}}\right]=\frac{\alpha}{\alpha+it}$$

With this one can verify that \\(\phi(\log{X_{\alpha}}) = \phi(\log{X_{\alpha+1}})\phi(\frac{1}{\alpha}\log{U})\\).

# Scaling
We have found a way of sampling from \\(\text{Gamma}(\alpha,1)\\), with this we can easily allow for \\(\beta\in\mathbb{R}^+\\) using \\(\text{Gamma}(\alpha,\beta)=\frac{1}{\beta}\text{Gamma}(\alpha,1)\\). This concludes the discussing of sampling from a Gamma distribution. Below is the c code that can do the sampling


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
