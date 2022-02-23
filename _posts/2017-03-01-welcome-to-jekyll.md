---
title: "Welcome to my blog"
mathjax: true
layout: post
---

Hi, this is my first post to ensure that everything is working properly.

Now we derive the Euler equation for our new maximization problem. The maximization problem is 
$$
\text{max} \int_0^T\left[ b\log\left(\frac{1}{\alpha}\left(\dot p(t) + \gamma p(t)\right)\right)-cp(t)\right]e^{-rt}\;dt.
$$
The Euler equation for this problem is 
$$\frac{\partial F(t,p(t),\dot p(t))}{\partial p} - \frac{d}{dt}\left(\frac{\partial F(t,p(t),\dot p(t))}{\partial \dot p}\right)=0.$$
$$\frac{\partial F(t,p(t),\dot p(t))}{\partial p}=e^{-rt}\left(\frac{b\gamma}{\dot p(t) + \gamma p(t)} - c\right),\;\frac{\partial F(t,p(t),\dot p(t))}{\partial \dot p}=\frac{be^{-rt}}{\dot p(t) + \gamma p(t)}$$
$$
\frac{\partial F(t,p(t),\dot p(t))}{\partial p} - \frac{d}{dt}\left(\frac{\partial F(t,p(t),\dot p(t))}{\partial \dot p}\right)=0\Rightarrow be^{-rt}\left[\frac{\gamma(\dot p(t) + \gamma p(t))+\gamma\dot p(t) +\ddot p(t) + r(\dot p(t) + \gamma p(t))}{\left(\dot p(t) + \gamma p(t)\right)^2}\right]-ce^{-rt}=0.
$$
Now making use again of $\ddot p(t) = \alpha \dot d(t) - \gamma \dot p(t)$ and $\alpha d(t) = \dot p(t) + \gamma p(t)$ the Euler equation becomes
$$
\frac{(\gamma + r)\alpha d(t) + \alpha\dot d(t)}{\alpha^2d(t)^2}=\frac{c}{b} \Rightarrow (\gamma + r) d(t)+\dot d(t) = \frac{\alpha c}{b}d(t)^2\Rightarrow \dot d(t) + (\gamma + r) d(t)-\frac{\alpha c}{b}d(t)^2 = 0.
$$

Because the term $e^{-rt}$ is a constant with respect to the variables $p$ and $\dot p$ we have that $F$ is still a concave function in $(p,\dot p)$. Therefore any admissible solution of the Euler equation is a solution to our maximization problem.
