#include <iostream>
#include "bitboard.h"
#include "probability.h"
#include "random.h"

int main()
{
    /** Optional for testing **/
    // setSeed(5);
    initialize();

    Opponent opp('I');
    playGame(opp);

    return 0;
}
