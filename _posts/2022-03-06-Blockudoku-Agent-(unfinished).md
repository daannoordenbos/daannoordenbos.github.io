---
title: "Blockudoku Agent"
mathjax: true
layout: post
---

# Blockudoku
Blockudoku is a puzzle game and it is described as “An original combination of Sudoku and block puzzle games”. During game play one gets three pieces (drawn from a selection) which they need to place down on the board, a 9x9 grid. If a line is filled or one of the nine 3x3 Sudoku boxes is filled, it is cleared and points are rewarded. The lines are cleared in between piece placements if they can be cleared, so one does not need to be able to place all pieces at once. Then if one has placed all three pieces, it repeats and one again gets three new random pieces. This continues until one cannot place all three pieces, then the game is terminated. The game is made more interesting by rewarding extra points for clearing multiple sections at once, however this will be ignored by the agent since it does not aim to get as many points as possible but to play for as long as possible.

<img src='images/clearing lines.png' alt='missing' style="width: 25%; float: right; vertical-align:top; padding: 0% 0% 2% 2%">

# Interface
The graphical user interface needs to keep track of the board, which moves can be made and display the board. The board is stored as a 17x17 matrix consisting of 1's and 0's. This is useful for computing which moves can be made, because to determine where a piece can be place we slide a 5x5 piece matrix over it and perform a check whether or not it can be placed. To implement this pieces are stored as a list of coordinates in the 5x5 grid they exist. This gives rise to the move generating function. 

{% highlight python %}

def generate_moves(self, piece):
    result = []
    # loop over all squares
    for i in range(13):
        for j in range(13):
            add_move = True
            # check for validity
            for square in piece:
                if 13 > i + square[0] > 3 and 13 > j + square[1] > 3:
                    if self.state[i + square[0]][j + square[1]] == 1:
                        add_move = False
                        break
                else:
                    add_move = False
                    break
            if add_move:
                result.append((i, j))

    return result
{% endhighlight %}

The internal representation also features a make move function

This is infact a slight variation on 

# Agent

# Engine interface connection

# Improvements
