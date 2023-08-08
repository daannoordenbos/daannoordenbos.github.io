---
title: "Better Elo performance rating"
mathjax: true
layout: post
---

# Introduction 

Recently I played a chess tournament where I played against six opponents with ratings of 1859, 2265, 2263, 2155, 1841 and 2166. I scored 4 out of 6 and the pairing website gave this performance a TPR (tournament performance rating) of 2212, however this is not correct! Moreover, TPR is often calculated incorrectly. To know why this happens we need to first look at the definitions of expected score and performance rating.

**Definition**: If player \\(A\\) has a rating of \\(r_A\\) and player \\(B\\) has a rating \\(r_B\\), then the expected score of \\(A\\) playing against \\(B\\) is given by the function \\(\text{We}(r_A-r_B)\\). Where \\(\text{We}(\cdot)\\) is a valid cumulative distribution function (CDF).

**Definition**: If player \\(p\\) plays k games against players with ratings \\(r_1,\dots,r_k\\) and scores \\(w\\) points. Then the performance rating, \\(\hat{r}\\), is the rating at which the expected score is \\(w\\). That is,
	
$$\sum_{i=1}^{k}\text{We}(\hat{r}-r_i)=w.$$

(**Remark**: If \\(w=0\\) or \\(w=k\\) the performance rating is not defined, so these extremes will be ignored.)

In the Elo rating system, often used in chess, the following \\(\text{We}(t)\\) is used,

$$\text{We}(t)=\frac{1}{1+10^{-t/400}}.$$

Now, by continuity and the intermediate value theorem we know that \\(\hat{r}\\) exists. However, actually computing \\(\hat{r}\\) exactly is not trivial and has to be done with a numerical method, like Newton's method. 

# Simple approximation

To circumvent this, the following is often done: Instead of solving,

$$\frac{1}{k}\sum_{i=1}^{k}\text{We}(\hat{r}-r_i)=\frac{w}{k},$$

the following is usually solved

$$\text{We}\left(\hat{r}-\frac{1}{k}\sum_{i=1}^{k}r_i\right)=\frac{w}{k}.$$

The latter can easily be solved and the solution is given by \\(\hat{r}=\bar{r}-400\log_{10}(k/w-1)\\). While this is a good approximation for when the opponent ratings are all very similar, it is not a good approximation when the opponent ratings are variable. The clear advantage however is that no sophisticated numerical methods are needed, making it something that can be computed in excel with ease. However I do not find this be an excuse, this simple method is in my opinion too simple and can be improved quite easily, still allowing it to be computed in excel.

# Modified approximation

The advantage of the simple approximation is that the logistic CDF has a very nice inverse. I leverage this in the modified approximation, by approximating \\(\frac{1}{k}\sum_{i=1}^{k}\text{We}(\hat{r}-r_i)\\) as a logistic CDF. Let us denote the function we want to fit to as,

$$F_X(t)=\frac{1}{k}\sum_{i=1}^{k}\frac{1}{1+10^{\frac{r_i-t}{400}}}.$$

Furthermore, let \\(L(t;\mu_{\mathrm{MM}},s_{\mathrm{MM}})\\) be the logistic CDF we are going to fit to \\(F_X(t)\\), with location parameter \\(\mu_{\mathrm{MM}}\\) and shape parameter \\(s_{\mathrm{MM}}\\). To fit \\(L(t;\mu_{\mathrm{MM}},s_{\mathrm{MM}})\\) to \\(F_X(t)\\) we will use method of moments. To do so note the following about the logistic distribution.

**Theorem**: If a random variable has the following cumulative distribution function,

$$F(t)=\frac{1}{1+\exp(-(t-\mu)/s)}$$

Then, the first and second moments are \\(\mu\\) and \\(\frac{\pi^2}{3}s^2+\mu^2\\) respectively.

The first and second moments associated with the CDF \\(L(t;\mu_{\mathrm{MM}},s_{\mathrm{MM}})\\) are \\(\mu_{\mathrm{MM}}\\) and \\(\frac{\pi^2}{3}s_{\mathrm{MM}}^2+\mu_{\mathrm{MM}}^2\\) respectively. The first and second moments associated with the CDF \\(F_X(t)\\) are given by

$$\begin{align*}\text{First moment: }\int_{-\infty}^{\infty}t\,\mathrm{d}F_X(t)&=\int_{-\infty}^{\infty}t\,\mathrm{d}\frac{1}{k}\sum_{i=1}^{k}\left[\frac{1}{1+\exp\left(-\log(10)(t-r_i)/400\right)}\right]\\&=\frac{1}{k}\sum_{i=1}^{k}\int_{-\infty}^{\infty}t\,\mathrm{d}\left[\frac{1}{1+\exp\left(-\log(10)(t-r_i)/400\right)}\right]\\&=\frac{1}{k}\sum_{i=1}^{k}r_i\\ \text{Second moment: }\int_{-\infty}^{\infty}t^2\,\mathrm{d}F_X(t)&=\int_{-\infty}^{\infty}t^2\,\mathrm{d}\frac{1}{k}\sum_{i=1}^{k}\left[\frac{1}{1+\exp\left(-\log(10)(t-r_i)/400\right)}\right]\\&=\frac{1}{k}\sum_{i=1}^{k}\int_{-\infty}^{\infty}t^2\,\mathrm{d}\left[\frac{1}{1+\exp\left(-\log(10)(t-r_i)/400\right)}\right]\\&=\frac{1}{k}\sum_{i=1}^{k}\left(\frac{\pi^2}{3}\frac{400^2}{\log^2(10)}+r_i^2\right)\\&=\frac{\pi^2}{3}\frac{400^2}{\log^2(10)}+\frac{1}{k}\sum_{i=1}^{k}r_i^2.\end{align*}$$

Therefore the method of moment estmators for the parameters are given by:

$$\mu_{\mathrm{MM}}=\frac{1}{k}\sum_{i=1}^{k}r_i=\bar{r}\text{, } s_{\mathrm{MM}}=\sqrt{\left(\frac{400}{\log(10)}\right)^2+\frac{3}{\pi^2}\text{var}\left(r\right)}\text{, with var}(r)=\frac{1}{k}\sum_{i=1}^{k}(r_i-\bar{r})^2$$

Using this approximation for \\(F_X(t)\\) we can approximate \\(\hat{r}\\). Recall that for \\(\hat{r}\\) we have \\(F_X(\hat{r})=\frac{w}{k}\\). So \\(\hat{r}\approx L^{-1}\left(\frac{w}{k};\mu_{\mathrm{MM}},s_{\mathrm{MM}}\right)\\), which yields the following:

$$\hat{r}\approx\bar{r}-\log\left(\frac{k}{w}-1\right)\sqrt{\left(\frac{400}{\log(10)}\right)^2+\frac{3}{\pi^2}\text{var}\left(r\right)}.$$

# Illustrative example

The main advantage of this method shines when considering victories against significantly lower-rated opponents. For instance, take a player securing a 2/3 score against competitors rated at 2300, 2400, and 2100. In reality their TPR is 2403, the conventional method yields 2387, and the refined approach yields 2396. Now, when wins against opponents rated at 1300, 1500, and 1700 are taken into account, the true TPR increases to 2410. In contrast, the simple TPR plummets to 2163, whereas the refined TPR only decreases to 2342. Evidently, this method demonstrates remarkable resilience to extreme rating deviations, considering its simplicity.
