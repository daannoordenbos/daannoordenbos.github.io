---
title: "Draughts move generator"
mathjax: true
layout: post
---

Today we are going to dive into my first game playing agent, namely my draughts engine OWL (Optimal win/loss). Specifically, we will discuss bitboards and how they are utilized in OWL's move generator.

# Draughts
To get everyone up to speed. What is draughts, and what are the rules? The game of draughts is played on a checkered 10x10 board, with a dark square in the bottom left corner. Only the dark squares are used with 20 white and 20 black pieces filling the bottom 4 and top 4 rows of the board respectively. The first move is played by white and turns alternate. A normal (sliding) move consists of moving a piece diagonally forward to an empty square. A capture move consists of jumping diagonally forward or backwards over an enemy piece on an adjacent square to an empty square immediately behind that, and removing the captured piece. A single piece can capture multiple pieces, however captured pieces are only removed at the end of the turn and it is now allowed to jump over the same piece multiple times. If a men ends its turn at the end of the board it is crowned as a king, a king captures and slides the same as a men, except that it can also move backwards and it can move to any empty square on a diagonal. Captures are compulsory and one has to choose from the move(s) capturing the most pieces.

# Bitboards
Now that we know the specific draughts variant we are looking at. Yes, there are other variant with different rules. We will introduce the concept of bitboards and the basic bit operations we apply to them. So, what are bitboard? A bitboard is a bit array, usually of 64 bits (the size of a long int), where each bit corresponds to a game board space or piece. So, we can use bitboards to store binary information about the squares of a game board. Bitboards are (generally) not suitable for games which have more than 64 squares or strange geometries. Games like Go, Stratego or Abalone. However, bitboards are especially useful in chess furthermore, they can also be used in draughts. The main advantage of bitboards is that they allow for parallel operations on all squares of the board. Most commonly these operations are: AND, OR, XOR, NOT and shift. However, we are not limited to this, there are also operations that find the index of the most or least significant bits, operations that get the most or least significant bit, and operations that even count the number of bits that are one.

# Draught bitboards
As mentioned previously, we can make bitboards work in draughts. Because, draughts has a diagonal geometry it is useful to look at how the bits are mapped onto the board.
<div style="text-align:center"><img src="/images/draughts/board map.png" width="50%" /></div>
<div align="center">
  Draughts board bitmapping.
</div>
This is the main bitmapping that is used in draughts. Notice that not all bits are used. Partly because a 64 bit integer has redundancy when only needing to store 50 bits. More importantly however, we see that the bits on positions 10, 21, 32, 43 are missing. This peculiar exclusion ensures that the board has nicer properties. The diagonals can be traversed by adding or subtracting 5 or 6 successively. Moreover, this bitmapping also handles edge detection. If these so called 'ghost' squared would not be included then the bitboard mapping would not have these nice properties. Why this is so useful will become more clear when we move on to the move generation.

## Game state
Using this bitmapping we can store a draughts game state with three bitboards. We store a bitboard with the white pieces, the black pieces, and the kings. From these three bitboards we can (with elementary operations) retrieve all relevant information. For example, the black men are given as and(black, not(kings)). Furthermore, we can change any board position into any other by xor'ing all three bitboards with three other bitboards which indicate change. This allows us to make and unmake moves quickly.

# Move Generation
During move generation we first try to generate the capture moves since we must capture if possible. If there are no captures we generate the sliding moves.

## Sliding Moves
My initial idea for generating sliding moves was to loop over all the men and check with if statements if they can move forward or not. This would work but, there is a far more elegant solution available. We can use the following combination of bit operations, shift(and(shift(men, x), empty), -x) where men is a bitboard of our men, empty is a bitboard of the empty squares, and x is the direction (5, 6, -5 or -6). The output of this is a bitboard of all men that can move in the specified direction. After which, we can turn the relevant bitboards into move lists using the bit extraction described previously.

There are also some clever and sophisticated tricks one could apply to king sliding moves. However, the majority of the game there are no kings on the board, and when there are kings on the board it is usually a few at most. Therefore, optimizing the king sliding move generation is not important. A simple for loop suffices.



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

{% highlight c++ %}
manRecursive(state, moveList, depth, whiteToMove, central){
  empty = AND(bitmapping, NOT(OR(white, black)))
  opponent = whiteToMove ? black : white
  noCapture = true

  for d in (-6, -5, 5, 6):
    if AND(opponent, shift(central, d) and AND(empty, shift(central, 2d):
      noCapture = false
      captureStack[depth].set(OR(central, shift(central, 2d)), AND(kings, shift(central, d)), shift(central, d), whiteToMove)
      state = XOR(state, captureStack[depth])
      manRecursive(state, moveList, depth + 1, whiteToMove, shift(central, 2d))
      state = XOR(state, captureStack[depth])

  if noCapture:
    if depth == moveList.longestCapture:
      addCapture(moveList, depth)

    else if depth > moveList.longestCapture:
      moveList.reset()
      moveList.longestCapture = depth
      addCapture(moveList, depth)

addCapture(moveList, depth):
  move = (captureStack[depth][0], captureStack[depth][1], captureStack[depth][2])
  for i in (1, ..., depth):
    move = XOR(move, (captureStack[i][0], captureStack[i][1], captureStack[i][2]))
  move = XOR(move, (captureStack[15][0], captureStack[15][1], captureStack[15][2]))
  move = OR(move, (AND(move[0], ROB1), 0, AND(move[2], ROW1))
  moveList.append(move)
{% endhighlight %}
