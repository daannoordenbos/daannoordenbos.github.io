---
title: "Better Elo performance rating"
mathjax: true
layout: post
---

# Introduction 

Recently I played a chess tournament where I played against six opponents with ratings of 1859, 2265, 2263, 2155, 1841 and 2166. I scored 4 out of 6 and the pairing website gave this performance a TPR (tournament performance rating) of 2212, however as few might know, these performance ratings are not calculated correctly! The TPR is in fact 2245.
To know why they are calculated incorrectly we need to first revise the definitions of expected score and performance rating.
**Definition**: If player \\(A\\) has a rating of \\(r_A\\) and player \\(B\\) has a rating \\(r_B\\), then the expected score of \\(A\\) playing against \\(B\\) is given by the function \\(\text{We}(r_A-r_B)\\). Where \\(\text{We}(\cdot)\\) is a valid cumulative distribution function (CDF).
**Definition**: If player \\(p\\) plays k games against players with ratings \\(\left\{r_1,\dots,r_k\right\}\\) and scores \\(w\\) points. Then the performance rating, \\(\hat{r}\\), is the rating at which the expected score is \\(w\\). That is,
	
$$\sum_{i=1}^{k}\text{We}(\hat{r}-r_i)=w.$$

(**Remark**: If \\(w=0\\) or \\(w=k\\) the performance rating is not defined, so these extremes will be ignored.)
In the Elo rating system, often used in chess, the following \\(\text{We}(t)\\) is used,

$$\text{We}(t)=\frac{1}{1+10^{-t/400}}.$$

Note that because \\(\text{We}(\cdot)\\) is a strictly monotonic function so will the total expected score, \\(\sum_{i=1}^{k}\text{We}(\hat{r}-r_i)\\), be. Therefore the total expected score is an invertible function and \\(\hat{r}\\) exists.
Actually computing the value of \\(\hat{r}\\) is not trivial and has to be done with a numerical method, like Newton's method. To circumvent this, the following is done, instead of solving,

$$\frac{1}{k}\sum_{i=1}^{k}\text{We}(\hat{r}-r_i)=\frac{w}{k},$$

the following is usually solved

$$\text{We}\left(\hat{r}-\frac{1}{k}\sum_{i=1}^{k}r_i\right)=\frac{w}{k}.$$

The latter can easily be solved and the solution is given by \\(\hat{r}=\bar{r}-400\log_{10}(k/w-1)\\). While this is a good approximation for when the opponent ratings are not very variable, it is a poor approximation when the opponent ratings are variable.
As I myself often play against players who are much lower rated than me, using this method is not satisfactory. Consequently I use a different method.
While it is definitely possible to use Newtons method, this is not excel friendly which and therefore a deal breaker. 

# Modified approximation

The advantage of the simple approximation is that the logistic CDF has a very nice inverse. We leverage this in the modified approximation.
We are going to approximate \\(\frac{1}{k}\sum_{i=1}^{k}\text{We}(\hat{r}-r_i)\\) as a logistic CDF. Let us denote the function we want to fit to as,

$$F_X(t)=\frac{1}{k}\sum_{i=1}^{k}\frac{1}{1+10^{\frac{r_i-t}{400}}}.$$

Furthermore, let \\(L(t;\mu^{*},s^{*})\\) be the logistic CDF we are going to fit to \\(F_X(t)\\), with location parameter \\(\mu^{*}\\) and shape parameter \\(s^{*}\\). To fit \\(L(t;\mu^{*},s^{*})\\) to \\(F_X(t)\\) for some \\(\mu^{*},s^{*}\\) we will use the method of moments. Take note of the following theorem.
**Theorem**: If a random variable has the following cumulative distribution function,

$$F(t)=\frac{1}{1+\exp(-(t-\mu)/s)}$$

Then, the first and second moments are \\(\mu\\) and \\(\frac{\pi^2}{3}s^2+\mu^2\\) respectively.
The first and second moments associated with the CDF \\(L(t;\mu^{*},s^{*})\\) are \\(\mu^{*}\\) and \\(\frac{\pi^2}{3}\left(s^*\right)^2+\left(\mu^{*}\right)^2\\) respectively. The first and second moments associated with the CDF \\(F_X(t)\\) are given by

$$\begin{align}&\text{First moment: }\int_{-\infty}^{\infty}t\,\mathrm{d}\frac{1}{k}\sum_{i=1}^{k}F_X(t)=\frac{1}{k}\sum_{i=1}^{k}\int_{-\infty}^{\infty}t\,\mathrm{d}\left[\frac{1}{1+\exp\left(-\log(10)(t-r_i)/400\right)}\right]=\frac{1}{k}\sum_{i=1}^{k}r_i\\&\text{Second moment: }\int_{-\infty}^{\infty}t^2\,\mathrm{d}\frac{1}{k}\sum_{i=1}^{k}F_X(t)=\frac{1}{k}\sum_{i=1}^{k}\int_{-\infty}^{\infty}t^2\,\mathrm{d}\left[\frac{1}{1+\exp\left(-\log(10)(t-r_i)/400\right)}\right]=\\&\frac{1}{k}\sum_{i=1}^{k}\left(\frac{\pi^2}{3}\frac{400^2}{\log^2(10)}+r_i^2\right)=\frac{\pi^2}{3}\frac{400^2}{\log^2(10)}+\frac{1}{k}\sum_{i=1}^{k}r_i^2.\end{align}$$

Solving for the conditions given by the moments we get that:

$$\mu^{*}=\frac{1}{k}\sum_{i=1}^{k}r_i=\bar{r}\text{, } s^{*}=\sqrt{\left(\frac{400}{\log(10)}\right)^2+\frac{3}{\pi^2}\text{var}\left(r\right)}\text{, with var}(r)=\frac{1}{k}\sum_{i=1}^{k}(r_i-\bar{r})^2$$

Using this approximation for \\(F_X(t)\\) we can approximate \\(\hat{r}\\). Recall that for \\(\hat{r}\\) we have \\(F_X(\hat{r})=\frac{w}{k}\\). So \\(\hat{r}\approx L^{-1}\left(\frac{w}{k};\mu^{*},s^{*}\right)\\), which yields the following (after some algebra)

$$\hat{r}\approx\bar{r}-\log\left(\frac{k}{w}-1\right)\sqrt{\left(\frac{400}{\log(10)}\right)^2+\frac{3}{\pi^2}\text{var}\left(r\right)}.$$

The main advantage of this method is that it is more robust to when wins against much lower rated players are included. If a player scores 2/3 against a 2300, 2400 and 2100 player then their true TPR is 2403, the simple method gives 2387 and the modified method gives 2396. However if we include wins against 1300, 1500 and 1700 the true tpr rises to 2410, the simple tpr drops to 2163, whilst the modified tpr only drops to 2342. The method is as we can see much more robust to such extreme rating variations. Conversely it does not overestimate the tpr when losses against much higher rated players are included
