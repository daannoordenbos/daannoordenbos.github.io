<script type="text/x-mathjax-config">
  MathJax.Hub.Config({
    tex2jax: {
      inlineMath: [ ['$','$'], ["\\(","\\)"] ],
      processEscapes: true
    }
  });
</script>
---
title: "Interesting queueing problem"
mathjax: true
layout: post
---
In this post I am going to present an interesting queueing problem, the main intrigue comes from the neat mathematical tricks I will present that one can use to solve this problem.

So let us consider a G/G/1 queue with the addition that the server can be interrupted (for repairs for example). Let us assume that we know the distributions of the interarrival times \(X\) and the service times \(S_0\). Then to model these interruptions we make two additional assumptions, firstly that the time between two interruptions is memoryless with mean \(1/\lambda\) and secondly that the down times \(\{R_i\}\) are i.i.d.
Our goal will be to determine the expected (approximate) waiting time using Sakasewaga's formula.
