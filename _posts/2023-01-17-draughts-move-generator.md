---
title: "Draughts move generator"
mathjax: true
layout: post
---

Today we are going to take a closer look at my first game playing agent, namely my draughts engine OWL (Optimal win/loss). Specifically, we will discuss bitboards and how they are utilized in OWL's move generator.

# Draughts
To get everyone up to speed. What is draughts, and what are the rules? The game of draughts is played on a checkered 10x10 board, with a dark square in the bottom left corner. Only the dark squares are used with 20 white and 20 black pieces filling the bottom 4 and top 4 rows of the board respectively. The first move is played by white and turns alternate. A normal (sliding) move consists of moving a piece diagonally forward to an empty square. A capture move consists of jumping diagonally forward or backwards over an enemy piece on an adjacent square to an empty square immediately behind that, and removing the captured piece. A single piece can capture multiple pieces, however captured pieces are only removed at the end of the turn and it is now allowed to jump over the same piece multiple times. If a men ends its turn at the end of the board it is crowned as a king, a king captures and slides the same as a men, except that it can also move backwards and it can move to any empty square on a diagonal. Captures are compulsory and one has to choose from the move(s) capturing the most pieces.

# Bitboards
Now that we know the specific draughts variant we are looking at. Yes, there are other variants with different rules. We will introduce the concept of bitboards and the basic bit operations we apply to them. So, what are bitboard? A bitboard is a bit array, usually of 64 bits (the size of a long int), where each bit corresponds to a game board space or piece. So, we can use bitboards to store binary information about the squares of a game board. Bitboards are (generally) not suitable for games which have more than 64 squares or strange geometries. Games like Go, Stratego or Abalone. Fortunately, bitboards can be used effectively for chess and with some work in draughts. The main advantage of bitboards is that they allow for parallel operations on all squares of the board. Most commonly these operations are: AND, OR, XOR, NOT and shift. However, we are not limited to this, there are also operations that find the index of the most or least significant bits, operations that get the most or least significant bit, and operations that even count the number of bits that are one.

# Draught bitboards
As mentioned previously, we can make bitboards work in draughts. This is because draughts has 50 squares which can be mapped to 64 bits. However, because draughts has a diagonal geometry, a slightly non-obvious bitmapping is used to represent the board.
<div style="text-align:center"><img src="/images/draughts/board map.png" width="50%" /></div>
<div align="center">
  Draughts board bitmapping.
</div>
This is the main bitmapping that is used in draughts. Notice that not all bits are used. Partly because a 64 bit integer has redundancy when only needing to store 50 bits. More importantly however, we see that the bits on positions 10, 21, 32, 43 are missing. This peculiar exclusion ensures that the board has nicer properties. The diagonals can be traversed by adding or subtracting 5 or 6 successively. Moreover, this bitmapping also handles edge detection. If these so called 'ghost' squared would not be included then the bitboard mapping would not have these nice properties. 

## Game state
Using this bitmapping we can store a draughts game state with three bitboards. We store a bitboard with the white pieces, the black pieces, and the kings. From these three bitboards we can (with elementary operations) retrieve all relevant information. For example, the black men are given as AND(black, NOT(kings)).

# Move Generation
During move generation we first try to generate the capture moves since we must capture if possible. If there are no captures we generate the sliding moves. Moves are stored as an array of successor game states, however in the pseudo-code below this has been slightly abstracted away. 
Moreover, we employ the general function shift(bits, x) but, in practice this is actually two separate functions, left and right shift for positive and negative x respectively. The pseudo-code below is therefore, more succinct than the source code.

## Sliding Moves
The sliding moves are very straightforward to compute, loop over all men and check whether or not they can go forward, either left or right, and add each move to the movelist. However, this would require many if statements, and looping over empty squares. We can avoid these pitfalls with the following more elegant solution which utilizes bitboards. We use the following combination of bit operations, shift(AND(shift(men, x), empty), -x) where men is a bitboard of our men, empty is a bitboard of the empty squares, and x is the direction (5, 6, -5 or -6). The output of this is a bitboard of all men that can move in the specified direction. After which, we can turn the relevant bitboards into move lists using the bit extraction described previously.

There are also some clever and sophisticated tricks one could apply to computing king sliding moves (for example: [magic bitboards](https://www.chessprogramming.org/Magic_Bitboards) or [PEXT](https://www.felixcloutier.com/x86/pext)) however, this is a topic for another time. Since, the speedups are far too small for the complexity they bring. That is due to the fact that, for the majority of the game there are no kings on the board, and when there are kings on the board it is usually a few at most. Therefore, optimizing the king sliding move generation is not important. So, we use the naive approach. Loop over all kings and use a filling approach to compute all moves that the kings can make. The (detailed) pseudo-code for finding sliding moves is given below.

{% highlight c++ %}
slidingMoves(state, whiteToMove, moveList):
  empty = AND(bitmapping, NOT(OR(white, black)))
  whiteMen = AND(white, NOT(kings)
  blackMen = AND(white, NOT(kings)

  if whiteToMove:
    right = shift(AND(shift(whiteMen, 5), empty), -5)
    left = shift(AND(shift(whiteMen, 6), empty), -6)

    while right:
      bit = AND(right, -right) 
      right = AND(right - 1, right)
      moveList.append(black, XOR(kings, AND(shift(bit, 5), ROW1)), XOR(white, bit, shift(bit, 5)))

    while left:
      bit = AND(left, -left)
      left = AND(left - 1, left)
      moveList.append(black, XOR(kings, AND(shift(bit, 6), ROW1)), XOR(white, bit, shift(bit, 6)))

    king = AND(white, kings)
    while king:
      bit = AND(king, -king)
      king = AND(king - 1, king)
      for d in (-6, -5, 5, 6):
        option = bit
        while AND(empty, shift(option, d)):
          option = shift(option, d)
          moveList.append(black, XOR(kings, bit, option), XOR(white, bit, option))
  else:
    right = shift(AND(shift(blackMen, -6), empty), 6)
    left = shift(AND(shift(blackMen, -5), empty), 5)

    while right:
      bit = AND(right, -right)
      right = AND(right - 1, right)
      moveList.append(XOR(black, bit, shift(bit, -6)), XOR(kings, AND(shift(bit, -6), ROB1)), white)

    while left:
      bit = AND(left, -left)
      left = AND(left - 1, left)
      moveList.append(XOR(black, bit, shift(bit, -5)), XOR(kings, AND(shift(bit, -5), ROB1)), white)

    king = AND(black, kings)
    while king:
      bit = AND(king, -king)
      king = AND(king - 1, king)
      for d in (-6, -5, 5, 6):
        option = bit
        while AND(empty, shift(option, d)):
          option = shift(option, d)
          moveList.append(XOR(black, bit, option), XOR(kings, bit, option), white)
        
{% endhighlight %}

## Captures moves
The difficulty of generating capture moves is that they have a recursive structure, after each partial capture we repeat the same procedure. Therefore, to generate capture moves we must make use of recursion. This is done in the following way, first a valid partial capture is found, then it calls the recursive function to see if any more captures are possible. After there are no more partial captures, the resulting position is added as a successor. Of course, when a longer capture is found, the list of current moves is cleared, since they are not relevant anymore. One might think that king captures are even more complicated, but they are nearly analogous to men captures, the only difference is that kings are more mobile than men. This can be found in the source code, as it is not interesting enough to discuss here. The pseudo-code we get when taking everything together is displayed below



# Performance

The described move generator generates ~130 million successor positions per second from the starting position. This is very comparable to the [best move generators](https://damforum.nl/bb3/viewtopic.php?f=53&t=2308&start=240), which perform marginally better after taking hardware in account. The source code of the move generator can be found [here](https://github.com/daannoordenbos/daannoordenbos.github.io/tree/master/featured_code/draughts%20move%20generator).
