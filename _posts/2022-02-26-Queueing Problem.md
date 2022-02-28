---
title: "Interesting queueing problem"
mathjax: true
layout: post
---
In this post I am going to present an interesting queueing problem, it is quite a straightforward problem but going through the computations is a good exercise.

# Introduction

So, let us consider a G/G/1 queue with the addition that the server can be interrupted (for repairs for example). Let us assume that we know the distributions of the interarrival times \\(X\\) and the service times \\(S_0\\). Then to model these interruptions we make two additional assumptions, firstly that the time between two interruptions is memoryless with mean \\(1/\lambda\\) and secondly that the down times \\(\{R_i\}\\) are i.i.d.
Our goal will be to determine the expected (approximate) waiting time using Sakasegawa's formula.

$$E[W]=\frac{C^2_a+C^2_s}{2}\frac{\rho}{1-\rho}E[S]\text{, (Sakasegawa's formula for one server)}$$

Note that to compute \\(E[W]\\) we need to determine \\(E[S]\\) and \\(E\left[S^2\right]\\), where \\(S\\) is our yet to be determined true service time.

# Computing moments

Because \\(S\\) is our total service time (including interruptions) we get that it is given by

$$S=S_0+\sum_{i=1}^NR_i$$

Where \\(N\\) denotes the amount of interruptions during the service time. Because we assume that the time between interruptions is memoryless we obtain the conditional distribution of N, namely \\(N\mid S_0=s\sim\text{Pois}(\lambda s)\\). Therefore we get that

$$E[N]=E[E[N|S_0]]=E[\lambda S_0] = \lambda E[S_0]$$

$$E[N^2]=E[E[N^2|S_0]]=E[\lambda^2 S_0^2 + \lambda S_0] = \lambda^2 E[S_0^2] + \lambda E[S_0]$$

We will use this result to determine the first and second moments of \\(S\\).

$$E[S] = E\left[S_0+\sum_{i=1}^NR_i\right]=E[S_0]+E\left[E\left[\sum_{i=1}^NR_i\right]|N\right]=$$

$$E[S_0]+E\left[NE[R]\right]=E[S_0]+E[N]E[R]=E[S_0]+\lambda E[S_0]E[R]$$

$$E\left[S^2\right]=E\left[\left(S_0+\sum_{i=1}^NR_i\right)^2\right]=E\left[S_0^2\right]+2E\left[S_0\sum_{i=1}^NR_i\right]+E\left[\left(\sum_{i=1}^NR_i\right)^2\right]$$

Now to determine the results of the second and third terms we will use conditioning.

$$E\left[S_0\sum_{i=1}^NR_i\right] = E\left[E\left[S_0\sum_{i=1}^NR_i\mid S_0\right]\right]=E\left[S_0 E[R]E\left[N\mid S_0\right]\right] = E[\lambda S_0^2 E[R]] = \lambda E[R]E[S_0^2]$$

$$E\left[\left(\sum_{i=1}^NR_i\right)^2\right]=E\left[\left(\sum_{i=1}^NR_i\right)\left(\sum_{i=1}^NR_j\right)\right]=E\left[E\left[\left(\sum_{i=1}^NR_i\right)\left(\sum_{i=1}^NR_j\right)\mid N\right]\right]=$$

$$E\left[E\left[\sum_{i=1}^NR_i^2+\sum_{i\ne j}^N R_i R_j\mid N\right]\right]=E\left[NE\left[R^2\right]+N(N-1)E\left[R\right]^2\right]=$$

$$E[N]E\left[R^2\right]+(E\left[N^2\right]-E[N])E[R]^2=\lambda E[S_0]E[R^2] + \lambda^2 E\left[S_0^2\right]E[R]^2$$

# Clean up

We can clean up these 
