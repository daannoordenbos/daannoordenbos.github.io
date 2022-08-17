---
title: "Endgame play in Amazons"
mathjax: true
layout: post
---

# Introduction: Endgame play in the game of the Amazons  

Recently my development of my Amazons playing agent has been focused on the last phase of the game, the filling phase.
The filling phase is the phase of the game where there is no uncontested territory, so the goal for both players is to make optimal use of their captured territory.
When a territory cannot be filled completely it is called defective. Minimax search can have trouble identifying defective regions, since they only reveal themselves at the end of a search. To remedy this problem I have set out to implement a database of small endgames to aid the search. To start of we will need to determine which endgames are even possible.

# Generating an Amazons endgame database 
## Generating regions
Firstly we will define what a region is:\
**Definition:** A region is a set of squares in the grid such that all squares have at least one neighbour on one of the 8 surrounding squares. The size of a region is the amount of squares in the region.\
From the definition of a region the natural question that arises is how many different regions are there of size \\(n\\) and subsequently, when are two regions different?
Starting with the latter, we say that two regions are different when they cannot be transformed into each other with transformations that do not change the properties of a region. The property that is important is how squares relate to each other, so the transformations which do not change these relations are spatial shifts, reflections and 90 degree rotations. The last two transformations are the more interesting ones, when combining them there are at most eight possible equivalent representations of a region.
<p align="middle">
  <img src="/images/amazons/EXCEL_P9pUXSuvD5.png" width="90%" /> 
</p>
<div align="center">
  Example of a region with eight equivelant representations.
</div>
With this definition of equivalence we can determine the amount of unique regions of size \\(n\\). Firstly, let use denote the set of all unique regions of size \\(n\\) by \\(S_n\\). Then if we have a region \\(s\in S_n\\) and a square \\(k\in s\\) we can create a new region \\(t\\) of size \\(n+1\\) by adding a square to \\(s\\) which is not in \\(s\\) and surrounding \\(k\\). Note that for any \\(k\\) there might be multiple surrounding squares we could add to \\(s\\) to get a new region of size \\(n+1\\). If we do this for all \\(k \in s\\), we get a set of regions of size \\(n+1\\). Let us denote this procedure as \\(T=f(s)\\), where \\(T\\) is the new set. Here is the key point, since \\(S_n\\) contains all regions of size n, we must have that \\(S_{n+1}\subseteq f(S_n)\\). So to go from \\(S_n\\) to \\(S_{n+1}\\) all that is required is to filter out all isomorphisms from \\(f(S_n)\\). Lastly, since we know that the only element in \\(S_1\\) is a region consisting of a single square we can compute all \\(S_n\\). The pseudocode for the general procedure is given below.
### Pseudocode

{% highlight python %}

limit = n
AllRegions = [S_1]
while size < limit:
    NewRegions = []
    for region in AllRegions:
        NewRegions.add(f(region))
    for region in NewRegions:
        new_region = true
        for transformation in transformations:
            new_region &= (region.transformation not in AllRegions):
        if new_region:
            AllRegions.add(region)
    size += 1  

f(s):
    t = []
    for k in s:
        for neighbor around k:
            if neighbor not in s:
                t.add(s + neighbor)
    return t
{% endhighlight %}
### Results
In the table below the amount of regions of a particular size up to 7 are reported. The total column has been filtered up to only spatial translations and the reduced column has been filtered up to spatial translations, reflections and rotations.

|     Size    | Total | Reduced |
|:-----------:|:-----:|:-------:|
| 1           | 1     | 1       |
| 2           | 4     | 2       |
| 3           | 20    | 5       |
| 4           | 110   | 22      |
| 5           | 638   | 94      |
| 6           | 3832  | 524     |
| 7           | 23592 | 3031    |
| \\(\le\\)7  | 28197 | 3679    |

These results are in accordance with Muller & Tegos (2002).

## Evaluating chambers
A chamber is what we call a region with amazons in it of one color. As stated previously we desire chambers with minimal defect, we can define defects in the following way.\
**Definition:** Let there be a region \\(s\\) of size \\(n\\) with an amazon on the square \\(k \in s\\). If the amazon can shoot \\(m\\) arrows in the region \\(s\\), then the defect of the region is \\(n-m-1\\) and the territory of this region is \\(m\\). \
**examples of defective regions** \
With this definition we can actually compute the defects quite easily using retrograde analysis.
### Pseudocode
{% highlight python %}

AllChambers = []

for region in AllRegions:
    for transformation in transformations:
        for k in region.transformation:
          AllChambers.add(CreateChamber(region.transformation, k)) 

AllChambers[0].territory = 0

for chamber in AllChambers:
    succesors = chamber.GenerateSuccesors()
    for succesor in succesors:
        chamber.territory = max(chamber.territory, AllChambers[succesor].territory + 1)
{% endhighlight %}

## Storing chambers efficiently
To store the defects of the chambers efficiently we make use of the following observation: The largest defect for chambers up to a size of seven is three. This means that it takes only two bits to store the defect of a chamber. We can store the defects for all amazon arrangements in the following way.

<p align="middle">
  <img src="/images/amazons/mspaint_9581khUwuH.png" width="90%" /> 
</p>
<div align="center">
  Example of a region and how the defects are stored.
</div>

Furthermore, we do not actually need to store the regions where the defect is zero for all amazon placements. This further reduces the cases we need to store to 1138. Then all that is left is to obtain the right placement number for the amazon.

### Pseudocode
{% highlight python %}

// Hash and key values
keys = [...]
hashes = [...]
 
int RegionDefect (bits, ax, ay) {
    // Get the 8 equivilant representations
    symmetries = GetSymmetries(bits)
    //read table
    for a in range(8):
        for b in range(1138):
            if symmetries[a] == hashes[b]:
                return ((keys[b]) >> (2 * PlacementNumber(a, bits, ax, ay))) & 3
    return 0
{% endhighlight %}


## References
Muller, M., & Tegos, T. (2002). Experiments in Computer Amazons.
