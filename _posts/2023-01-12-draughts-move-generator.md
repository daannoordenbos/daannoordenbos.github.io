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

void slide(U64* pos, U64 moves[100][3], int color){
    U64 empty = aliveSquares & (~(pos[0] | pos[2]));
    int k = 1;
    U64 way_1;
    U64 way_2;
    U64 white = pos[2] & (~pos[1]);
    U64 black = pos[0] & (~pos[1]);
    U64 bit;
    U64 b = pos[0];
    U64 kings = pos[1];
    U64 w = pos[2];


    if (color == 1) {
        way_1 = ((white << 5) & empty) >> 5;
        way_2 = ((white << 6) & empty) >> 6;
        while (way_1) {
            bit = way_1 & -way_1;
            way_1 = (way_1 - 1) & way_1;
            moves[k][2] = w ^ bit ^ (bit << 5);
            moves[k][1] = kings ^ ((bit << 5) & ROW1);
            moves[k][0] = b;

            k++;
        }
        while (way_2) {
            bit = way_2 & -way_2;
            way_2 = (way_2 - 1) & way_2;
            moves[k][2] = w ^ bit ^ (bit << 6);
            moves[k][1] = kings ^ ((bit << 6) & ROW1);
            moves[k][0] = b;

            k++;
        }

    } else {
        way_1 = ((black >> 5) & empty) << 5;
        way_2 = ((black >> 6) & empty) << 6;
        while (way_1) {
            bit = way_1 & -way_1;
            way_1 = (way_1 - 1) & way_1;
            moves[k][0] = b ^ bit ^ (bit >> 5);
            moves[k][1] = kings ^ ((bit >> 5) & ROB1);
            moves[k][2] = w;

            k++;
        }
        while (way_2) {
            bit = way_2 & -way_2;
            way_2 = (way_2 - 1) & way_2;
            moves[k][0] = b ^ bit ^ (bit >> 6);
            moves[k][1] = kings ^ ((bit >> 6) & ROB1);
            moves[k][2] = w;

            k++;
        }
    }
    if (color == 1) {
        U64 king = pos[1] & pos[2];

        U64 dir;
        while (king) {
            bit = king & -king;
            king &= (king -1);

            dir = bit;
            while ((empty & (dir << 5)) != 0) {
                dir = dir << 5;
                moves[k][2] = w ^ bit ^ dir;
                moves[k][1] = kings ^ bit ^ dir;
                moves[k][0] = b;
                k++;
            }

            dir = bit;
            while ((empty & (dir << 6)) != 0) {
                dir = dir << 6;
                moves[k][2] = w ^ bit ^ dir;
                moves[k][1] = kings ^ bit ^ dir;
                moves[k][0] = b;
                k++;
            }

            dir = bit;
            while ((empty & (dir >> 5)) != 0) {
                dir = dir >> 5;
                moves[k][2] = w ^ bit ^ dir;
                moves[k][1] = kings ^ bit ^ dir;
                moves[k][0] = b;
                k++;
            }

            dir = bit;
            while ((empty & (dir >> 6)) != 0) {
                dir = dir >> 6;
                moves[k][2] = w ^ bit ^ dir;
                moves[k][1] = kings ^ bit ^ dir;
                moves[k][0] = b;
                k++;
            }
        }
    } else {
        U64 king = pos[1] & pos[0];

        U64 dir;
        while (king) {
            bit = king & -king;
            king &= (king -1);

            dir = bit;
            while ((empty & (dir << 5)) != 0) {
                dir = dir << 5;
                moves[k][2] = w;
                moves[k][1] = kings ^ bit ^ dir;
                moves[k][0] = b ^ bit ^ dir;
                k++;
            }

            dir = bit;
            while ((empty & (dir << 6)) != 0) {
                dir = dir << 6;
                moves[k][2] = w;
                moves[k][1] = kings ^ bit ^ dir;
                moves[k][0] = b ^ bit ^ dir;
                k++;
            }

            dir = bit;
            while ((empty & (dir >> 5)) != 0) {
                dir = dir >> 5;
                moves[k][2] = w;
                moves[k][1] = kings ^ bit ^ dir;
                moves[k][0] = b ^ bit ^ dir;
                k++;
            }

            dir = bit;
            while ((empty & (dir >> 6)) != 0) {
                dir = dir >> 6;
                moves[k][2] = w;
                moves[k][1] = kings ^ bit ^ dir;
                moves[k][0] = b ^ bit ^ dir;
                k++;
            }
        }
    }

    moves[0][0] = k - 1;
}

{% endhighlight %}
