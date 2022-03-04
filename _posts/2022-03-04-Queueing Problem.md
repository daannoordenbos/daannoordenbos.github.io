---
title: "Tackling a queueing problem"
mathjax: true
layout: post
---
In this post I am going to present an interesting queueing problem, it is quite a straightforward problem but going through the computations is a good exercise.

# Introduction

So, let us consider a G/G/1 queue with the addition that the server can be interrupted (for repairs for example). Let us assume that we know the distributions of the interarrival times \\(X\\) and the service times \\(S_0\\). Then to model these interruptions we make two additional assumptions, firstly that the time between two interruptions is memoryless with mean \\(1/\lambda\\) and secondly that the down times \\(\{R_i\}\\) are i.i.d.
Our goal will be to determine the expected (approximate) waiting time using Sakasegawa's formula.

$$E[W]=\frac{C^2_a+C^2_s}{2}\frac{\rho}{1-\rho}E[S]\text{, (Sakasegawa's formula for one server)}$$

Note that to compute \\(E[W]\\) we need to determine \\(E[S]\\) and \\(E\left[S^2\right]\\), where \\(S\\) is our yet to be determined true service time.

# Computations

Because \\(S\\) is our total service time (including interruptions) we get that it is given by

$$S=S_0+\sum_{i=1}^NR_i$$

Where \\(N\\) denotes the amount of interruptions during the service time. Because we assume that the time between interruptions is memoryless we obtain the conditional distribution of N, namely \\(N\mid S_0=s\sim\text{Pois}(\lambda s)\\). Therefore we get that

$$E[N]=E[E[N|S_0]]=E[\lambda S_0] = \lambda E[S_0]$$

$$E[N^2]=E[E[N^2|S_0]]=E[\lambda^2 S_0^2 + \lambda S_0] = \lambda^2 E[S_0^2] + \lambda E[S_0]$$

We will use this result to determine the expectation and variance of \\(S\\).

$$E[S] = E\left[S_0+\sum_{i=1}^NR_i\right]=E[S_0]+E\left[E\left[\sum_{i=1}^NR_i\right]|N\right]=$$

$$E[S_0]+E\left[NE[R]\right]=E[S_0]+E[N]E[R]=E[S_0]+\lambda E[S_0]E[R]$$

Now we will use (iterated) eve's law for the variance.

$$V[S|S_0,N]=V\left[S_0\sum_{i=1}^NR_i\right]=NV[R]$$

$$V[S|S_0] = E[V[S|S_0,N] |S_0]+ V[E[S|S_0N] |S_0] = \lambda S_0V[R]+ V[S_0 +N E[R] |S_0]$$

$$= \lambda S_0V[R]+(E[R])2V[N|S_0] = \lambda S_0V[R]+(E[R])2\lambda S_0$$

$$V[S] = E[V[S|S_0]]+ V[E[S|S_0]] = \lambda (V[R]+(E[R])^2)E[S_0]+ V[(1+\lambda E[R])S_0]$$

$$= \lambda E[R^2]E[S_0]+(1+\lambda E[R])^2V[S_0]$$

The computation of this variation looks straightforward, but one can easily make a mistake in this calculation. When I first tried computing this variance I got the correct result but I used some Illegal steps. Being thorough is important however since probability is hard and miscalculation can easily be made.
With some algebra we get that

$$C^2_{s} = C^2_{s_0} + \frac{\lambda E\left[R^2\right]\left(1+\lambda E[R]\right)^2}{E[S_0]}$$

Now we can use Sakasegawa's formula to perform sensitivity analysis on this system.


