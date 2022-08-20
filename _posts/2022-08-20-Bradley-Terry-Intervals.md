---
title: "Confidence intervals for the Bradley-Terry model"
mathjax: true
layout: post
---


# Introduction
The Bradley-Terry model is a simple model to rank different individuals. This is done by assuming that
$$P(i\text{ beats } j)=\frac{p_i}{p_i+p_j}.$$
Where \\(p_k\\) is some fitness value for the players. This simple model can be extended to include ties, home side advantages etc, but for now we will limit ourselves to the simple case.

A feature of this model is that if \\(i\\) is twice as likely to beat \\(j\\) than \\(j\\) is to beat \\(i\\) we will have that \\(p_i=2p_j\\). However, if absolute differences are desired one can use \\(p_k=e^{S\beta_k}\\) such that \\(\beta_i=\beta_j+\gamma\\) for some \\(\gamma\\) when \\(i\\) is twice as likely to beat \\(j\\). In fact the Elo-rating system uses \\(S=\frac{\log(10)}{400}\\). Since scaling can be done afterwards we will use \\(p_k=e^{\beta_k}\\).\\
We will now derive the maximum likelihood estimate and the associated confidence intervals.

# Likelihood
Let us take \\(w_{ij}\\) to be the amount of times that \\(i\\) has beaten \\(j\\). Then the likelihood of \\(i\\) beating \\(j\\) \\(w_{ij}\\) times and \\(j\\) beating \\(i\\) \\(w_{ji}\\) times is given by
$$
    \binom{w_{ij}+w_{ji}}{w_{ij}}\left(\frac{e^{\beta_i}}{e^{\beta_i}+e^{\beta_j}}\right)^{w_{ij}}\left(\frac{e^{\beta_j}}{e^{\beta_i}+e^{\beta_j}}\right)^{w_{ji}}
$$
We can use this to determine the squared likelihood function for all results (it is squared since all results are considered exactly twice).
$$
    L(\boldsymbol{\beta})^2=\prod^n_{i}\prod^n_{j}\binom{w_{ij}+w_{ji}}{w_{ij}}\left(\frac{e^{\beta_i}}{e^{\beta_i}+e^{\beta_j}}\right)^{w_{ij}}\left(\frac{e^{\beta_j}}{e^{\beta_i}+e^{\beta_j}}\right)^{w_{ji}}
$$
This will result in the following log-likelihood
$$
\begin{aligned}
    2l(\boldsymbol{\beta}) &= \sum^n_{i}\sum^n_{j}\left[\log\left(\binom{w_{ij}+w_{ji}}{w_{ij}}\right)+w_{ij}\log\left(\frac{e^{\beta_i}}{e^{\beta_i}+e^{\beta_j}}\right)+w_{ji}\log\left(\frac{e^{\beta_j}}{e^{\beta_i}+e^{\beta_j}}\right)\right]\\
     &\sim \sum^n_{i}\sum^n_{j}\left[w_{ij}\log\left(\frac{e^{\beta_i}}{e^{\beta_i}+e^{\beta_j}}\right)+w_{ji}\log\left(\frac{e^{\beta_j}}{e^{\beta_i}+e^{\beta_j}}\right)\right]\\
     &=2\sum^n_{i}\sum^n_{j}\left[w_{ij}\log\left(\frac{e^{\beta_i}}{e^{\beta_i}+e^{\beta_j}}\right)\right]\text{, because we are double counting matches and therefore we have} \\
     l(\boldsymbol{\beta})&= \sum^n_{i}\sum^n_{j}w_{ij}\log\left(\frac{e^{\beta_i}}{e^{\beta_i}+e^{\beta_j}}\right)=\sum^n_{i}\sum^n_{j}w_{ij}\left(\beta_i-\log\left(e^{\beta_i}+e^{\beta_j}\right)\right).
\end{aligned}
$$
