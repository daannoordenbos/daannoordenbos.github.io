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
The graphical user interface needs to keep track of the board, which moves can be made and display the board. The board is stored as a 17x17 matrix consisting of 1's and 0's. This is useful for computing which moves can be made, because to determine where a piece can be place we slide a 5x5 piece matrix over it and perform a check whether it can be placed. To implement this pieces are stored as a list of coordinates in the 5x5 grid they exist. This gives rise to the move generating function. 

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

The rest of the state logic is not noteworthy and can be read in the source code. The user interface of the program was built using the Tkinter library. This too can be found in the source code. The user interface is quite minimal but it does feature a score count, the original visuals and a shaded region where the piece will drop if released. Now we will discuss how the agent plays the game.

# Agent
The agent that plays the game has been written in C++ for performance reasons. The game logic has been implemented in a nearly identical way, except for the fact that it uses static arrays and not lists. To find the best sequence of moves the agent will firstly consider all possible sequences of moves and then pick the sequence with the highest heuristic value. The former is done by considering all orders the pieces can be placed on the board, i.e. 1-2-3, 2-1-3, 3-2-1... . Then for each order the possible ways to place the pieces down is consider, resulting in all possible states using the three available (sub)moves. Then to associate each position with a heuristic value I use the amount of moves that can be made by all 47 pieces. This ensures that it will prefer positions with more options in the future with as intended effect that the game length will be longer. Surprisingly enough this simple algorithm is a very good player of the game of blockudoku. The main downside is that it can be very slow if the board is almost empty. To remedy this I added the condition that if the heuristic value is above a certain threshold that that sequence of moves is considered good enough so it will be played.

# Engine interface connection
The user interface will tell the engine what the current board is and which pieces it can use. The engine creates the specified game state and compute the sequence it thinks is best. This sequence is then told to the user interface. The integration of the engine into the user interface is quite straightforward, it is mostly boilerplate code to run the executable. However, I will show this boilerplate code since I believe that it can be applied in many circumstances.

{% highlight python %}
class Executable:
    def __init__(self, path_executable):
        self.executable = subprocess.Popen(path_executable, universal_newlines=True, 
                                       stdin=subprocess.PIPE, stdout=subprocess.PIPE)

    def _put(self, command):
        if not self.executable.stdin:
            raise BrokenPipeError()
        self.executable.stdin.write(f"{command}\n")
        self.executable.stdin.flush()

    def _read_line(self):
        if not self.executable.stdout:
            raise BrokenPipeError()
        return self.executable.stdout.readline().strip()

    def interact(self):
        # interact with the executable

    def kill(self):
        self.executable.terminate()
{% endhighlight %}

# Closing remarks
This project was a good experience for me because it was not too difficult, the end product performed above expectation, and I learned how classes work. For anyone interested the source code can be found [here](https://github.com/daannoordenbos/daannoordenbos.github.io/tree/master/featured_code/blockudoku%20code).
