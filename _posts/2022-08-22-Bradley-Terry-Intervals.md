---
title: "Confidence intervals for the Bradley-Terry model"
mathjax: true
layout: post
---


# Introduction
The Bradley-Terry model is a simple model to rank different individuals. This is done by assuming that
$$P(i\text{ beats } j)=\frac{p_i}{p_i+p_j}.$$
Where \(p_k\) is some fitness value for the players. This simple model can be extended to include ties, home side advantages etc, but for now we will limit ourselves to the simple case.

A feature of this model is that if $i\) is twice as likely to beat \(j\) than \(j\) is to beat \(i\) we will have that \(p_i=2p_j\). However, if absolute differences are desired one can use \(p_k=e^{S\beta_k}\) such that \(\beta_i=\beta_j+\gamma\) for some \(\gamma\) when \(i\) is twice as likely to beat \(j\). In fact the Elo-rating system uses \(S=\frac{\log(10)}{400}\). Since scaling can be done afterwards we will use \(p_k=e^{\beta_k}\).\\
We will now derive the maximum likelihood estimate and the associated confidence intervals.
