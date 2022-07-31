---
title: "Endgame play in Amazons"
mathjax: true
layout: post
---

# Introduction: Endgame play in the game of the Amazons  

Recently my development of my Amazons playing agent has been focused on the last phase of the game, the filling phase.
The filling phase is the phase of the game where there is no uncontested territory, so the goal for both players is to make optimal use of their captured territory.
When a territory cannot be filled completely it is called defective. Minimax search can have trouble identifying defective regions, since they only reveal themselves at the end of a search. **example**

Instead of adapting minimax search I will device a different method for playing out these endgames.

**roadmap**
To do this I first build a database of small endgames.
Firstly 

# Solving small Amazons chambers
To start of we will define what a region and a chamber is:

**Definition:** A region is a set of squares in the grid such that all squares have at least one neighbour in on of the 8 surrounding squares. The size of a region is the amount of squares in the region.  

**Definition:** A chamber is a region with only amazons in it of one color.

From the definition of a region the natural question that arises is how many different regions are there of size of size \\(n\\) and subsequently, when are two regions different?
Starting with the latter, we say that regions \\(A\\) and \\(B\\) are different when they cannot be transformed into each other with transformations that do not change the properties of a region.
The transformations that have this property are spatial shifts, reflections and 90 degree rotations. **example**

Using this definition of equivalence we can address our first question.

**Definition:** Let \\(S_n\\) be set the of all unique regions of size n.

This also raised the question 
An important concept is that two regions can by isomorphic to each other.
## Generating all regions up to isomorphism
To start 
### Definitions and idea
- Definition: A region is a set of connected squares in the grid (That is all tiles have a neighbour in one of the 8 surrounding squares). The size of a region is the amount of squares in the region.
- Chambers are equivilant under spatial translation, reflections and 90 degree rotations. So when region A can be transformed into region B using shifts, reflections and rotations we say that they are equivilant up to 'shift symmetry'.
- Definition: Let S_n be the set of all unique chambers of size n (unique up to shift symmetry).
- If we have a region s \in S_n and a square k \in s then we can create a new region t of size n+1 by taking s and adding a square not in s and surrounding k. Note that for any k there might be multiple surrounding squares we could add to s to get a new region of size n+1. If we do this for all k \in s, we get a set of chambers of size n+1. Let us denote this procedure as T=f(s), where T is the new set.
- Since S_n contains all unique chambers of size n we have that S_{n+1}\subset f(S_n).
- If we then filter out all isomorphisms out of f(S_n), we get S_{n+1}.
- There is only one region in S_1, namely a single square on the grid. Because we know S_1 we can compute all S_n k\in\mathbb{N}.

### Results

### Pseudo code
AllRegion = [S_1]
while (size < limit):
    NewRegion = []
    for region in AllRegion:
        NewRegion.add(f(region))
    for region in NewRegion:
        for symmetry in shift_symmetry:
            if region.symmetry not in AllCRegion:
                AllRegion.add(region)
                
## Computing the defect of all chambers
### Definition defect
- Let there be a chamber s of size n with only one amazon in it on the square k \in s. If the amazon shoots m arrows in the region s, then the defect of a region is n - m - 1 and the space in the region is m.
**Example defect and non defect region**
- Our goal is to determine the defect of any region with a size up to 7.

### Method
- To avoid computation during the retrograde analysis we will consider all feasible regions up to spatial translation.
- A region of size n can have an amazon at n different squares. So we have **data here** regions sizes.
- The chamber of size one has a space of 0. now we look at all chambers with size 2, generate all the moves and compute the highest space we can get on the next move. We repeat untill all data is exhausted. here is the pseudo code

### Pseudocode
AllChambers = [...]
AllChambers[0].space = 0

for chamber in AllChambers:
    succesors = chamber.GenerateSuccesors()
    for succesor in succesors:
        chamber.defect = max(chamber.defect, AllChambers[succesor].space + 1)
        
## Storing and retrieving chamber evaluations
- we observe that most regions do not have a defect, we can make use of this.
- we only store the cases where we have a defect.
- We represent regions are 64 bit numbers since this makes applying the shift symmetries computationallye easier, these 64 bit numbers can also be used as key values. no hash required.


### Pseudocode


int EndgameDefect (board, amazon):
    bits = ExtractRegion(board, amazon)
    symmetries = ExtractSymmetries(bits)
    
    for representation in symmetries:
    	if representation in PossibleDefectRegions:
    	    amazon_square = GetAmazonSquare(region)
    	    return Defect(amazon_square, representation)
    return 0 
