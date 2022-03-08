---
title: "Blockudoku Agent"
mathjax: true
layout: post
---

# Blockudoku
Blockudoku is a puzzle game and it is described as “An original combination of Sudoku and block puzzle games”. During game play one gets three pieces (drawn from a selection) which they need to place down on the board, a 9x9 grid. If a line is filled or one of the nine 3x3 Sudoku boxes is filled, it is cleared and points are rewarded. The lines are cleared in between piece placements if they can be cleared, so one does not need to be able to place all pieces at once. Then if one has placed all three pieces, it repeats and one again gets three new random pieces. This continues until one cannot place all three pieces, then the game is terminated. The game is made more interesting by rewarding extra points for clearing multiple sections at once, however this will be ignored by the agent since it does not aim to get as many points as possible but to play for as long as possible.

<p align="middle">
  <img src="/images/blockudoku/quzwEgv.png" width="45%" /> 
  <img src="/images/blockudoku/h0IjWzM.png" width="45%" />
</p>
<div align="center">
  Examples of a terminal position and a move which will clear a Sudoku block and a line.
</div>

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

The rest of the state logic is not noteworthy and can be read in the source code.

This is infact a slight variation on 

# Agent

# Engine interface connection
The user interface will tell the engine what the current board is and which pieces it can use. The engine create the specified game state and compute the sequence it thinks is best. This sequence is then told to the user interface. The integration of the engine into the user interface is not more complicated, most of the heavy lifting is done by the following boilerplate code which runs the executable.

{% highlight python %}
class Executable:
    def __init__(self, path_executable: str = "Executable Path"):
        self.engine = subprocess.Popen(path_executable, universal_newlines=True, 
                                       stdin=subprocess.PIPE, stdout=subprocess.PIPE)

    def _put(self, command):
        if not self.engine.stdin:
            raise BrokenPipeError()
        self.engine.stdin.write(f"{command}\n")
        self.engine.stdin.flush()

    def _read_line(self):
        if not self.engine.stdout:
            raise BrokenPipeError()
        return self.engine.stdout.readline().strip()

    def interact(self):
        # interact with the executable

    def kill(self):
        self.engine.terminate()
{% endhighlight %}
# Improvements
