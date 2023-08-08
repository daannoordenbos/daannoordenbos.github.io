#include <iostream>
#include "bitboard.h"
#include "probability.h"
#include "random.h"
#include <fstream>
/** ship mask initialization **/

Bitboard shipMask[4][200];
Bitboard shipBorder[4][180];

void initialize()
{
    // Ship masks
    for (int s = 2; s <= 5; s++)
    {
        for (int i = 0; i < 100; i++)
        {
            if (i % 10 <= 10 - s)
            {
                shipMask[s - 2][i % 10 + i / 10 * (11 - s)].set(0, 0);
                for (int l = 0; l < s; l++)
                {
                    shipMask[s - 2][i % 10 + i / 10 * (11 - s)] |= squareMask[i + l];
                }
            }

            if (i / 10 <= 10 - s)
            {
                shipMask[s - 2][i + 110 - 10 * s].set(0, 0);
                for (int l = 0; l < s; l++)
                {
                    shipMask[s - 2][i + 110 - 10 * s] |= squareMask[i + 10 * l];
                }
            }
        }
    }
    // Border of ships
    for (int s = 0; s < 4; s++)
    {
        for (int i = 0; i < 180 - 20 * s; i++)
        {
            shipBorder[s][i] = shipMask[s][i].adjacent();
        }
    }
}

/** Opponent object code **/

Opponent::Opponent(char mode)
{
    automatic = (mode == 'I' ? false : true);

    for (int i = 0; i < 5; i++)
    {
        layout[i] = 0;
        sunk[i] = 0;
    }
    ply = 0;
    shots.set(0, 0);

    if (mode == 'A')
    {
        char setup = '-';
        while (setup != 'Y' && setup != 'N')
        {
            std::cout << "Do you want to use a random ship arrangement? (Y/N): ";
            std::cin >> setup;
            // Cast to uppercase
            setup = (setup >= 'a' && setup <= 'z') ? (setup - 'a' + 'A') : setup;
        }
        if (setup == 'Y')
        {
            randomLayout();
        }
        else
        {
            customLayout();
        }
    }
    if (mode == 'F')
    {
        randomLayout();
    }
}

bool Opponent::allSunk()
{
    return sunk[0] && sunk[1] && sunk[2] && sunk[3] && sunk[4];
}

void Opponent::reset()
{
    for (int i = 0; i < 5; i++)
    {
        layout[i] = 0;
        sunk[i] = 0;
    }
    ply = 0;
    shots.set(0, 0);
    randomLayout();
}

void Opponent::showShips()
{
    (shipMask[SHIPSIZE(0) - 2][layout[0]] | shipMask[SHIPSIZE(1) - 2][layout[1]] | shipMask[SHIPSIZE(2) - 2][layout[2]] | shipMask[SHIPSIZE(3) - 2][layout[3]] | shipMask[SHIPSIZE(4) - 2][layout[4]]).print();
}

void Opponent::randomLayout()
{
    Bitboard arrangement(false);
    int shipSizes[5] = {2, 3, 3, 4, 5};
    int selected;
    for (int i = 0; i < 5; i++)
    {
        // select random remaining ship
        selected = randomInteger() % 5;
        while (!shipSizes[selected])
        {
            selected = randomInteger() % 5;
        }
        // select random possible ship location with rejection sampling
        int location = randomInteger() % (220 - 20 * shipSizes[selected]);
        while (arrangement & shipMask[shipSizes[selected] - 2][location])
        {
            location = randomInteger() % (220 - 20 * shipSizes[selected]);
        }
        // append position to occupied positions
        arrangement |= shipMask[shipSizes[selected] - 2][location];
        // make ship unavailable
        shipSizes[selected] = 0;
        // set location of ship
        layout[selected] = location;
    }
}

void Opponent::randomGoodLayout()
{
    const Bitboard d1(0xEFFBFEFFBFEFFBFE, 0x0000000FFBFEFFBF);
    const Bitboard d2(0xF7FDFF7FDFF7FDFF, 0x00000007FDFF7FDF);
    const Bitboard d3(0xFFFFFFFFFFFFFC00, 0x0000000FFFFFFFFF);
    const Bitboard d4(0xFFFFFFFFFFFFFFFF, 0x0000000003FFFFFF);

    Bitboard arrangement(false);
    Bitboard avoid(false);
    int shipSizes[5] = {2, 3, 3, 4, 5};
    int selected;
    for (int i = 0; i < 5; i++)
    {
        // select random remaining ship
        selected = randomInteger() % 5;
        while (!shipSizes[selected])
        {
            selected = randomInteger() % 5;
        }
        // select random ship location which does not lay next to another ship.
        avoid = arrangement |
              ((arrangement & d1) >> 1) |
              ((arrangement & d2) << 1) |
              ((arrangement & d3) >> 10) |
              ((arrangement & d4) << 10);
        int location = randomInteger() % (220 - 20 * shipSizes[selected]);
        while (avoid & shipMask[shipSizes[selected] - 2][location])
        {
            location = randomInteger() % (220 - 20 * shipSizes[selected]);
        }
        // append position to occupied positions
        arrangement |= shipMask[shipSizes[selected] - 2][location];
        // make ship unavailable
        shipSizes[selected] = 0;
        // set location of ship
        layout[selected] = location;
    }
}

// To do
void Opponent::customLayout()
{
    randomLayout();
}

Intel Opponent::autoMessage(int square)
{
    const char shipTypes[5] = {'D', 'C', 'S', 'B', 'A'};
    Intel intel;
    for (int i = 0; i < 5; i++)
    {
        Bitboard ship = shipMask[SHIPSIZE(i) - 2][layout[i]];
        if (ship & squareMask[square])
        {
            intel.type = 'H';
        }
        if (((shots & ship) != ship) && (((shots | squareMask[square]) & ship) == ship))
        {
            intel.type = 'S';
            intel.sunkShip = shipTypes[i];
            sunk[i] = true;
        }
        //if ( layout[i]
    }
    // Update internal state
    ply++;
    shots |= squareMask[square];

    return intel;
}

void playGame(Opponent opponent)
{
    Agent agent;
    if (opponent.automatic)
    {
        opponent.showShips();
        while(!opponent.allSunk())
        {
            int i = agent.expectationPolicy();
            Intel intel = opponent.autoMessage(i);
            agent.updateState(intel);

            agent.print();
            intel.print();
        }
    }
    else
    {
        while(!opponent.allSunk())
        {
            int i = agent.expectationPolicy();
            // Ask if the ship whether the
            char type = '-';
            char sunk = '-';
            while(type != 'H' && type != 'M' && type != 'S')
            {
                std::cout << "Agent calls: " << char('A' + i / 10) << char('1' + i % 10) << "\nThis shot hits, misses or sinks. (H/M/S): ";
                std::cin >> type;
                type = (type >= 'a' && type <= 'z') ? (type - 'a' + 'A') : type;
            }
            if (type == 'S')
            {
                while (sunk != 'D' && sunk != 'C' && sunk != 'S' && sunk != 'B' && sunk != 'A')
                {
                    std::cout << "The sunk ship is a destroyer, cruiser, submarine, battleship or an aircraft carrier: (D/C/S/B/A): ";
                    std::cin >> sunk;
                    sunk = (sunk >= 'a' && sunk <= 'z') ? (sunk - 'a' + 'A') : sunk;
                }
            }
            opponent.ply++;
            opponent.shots |= squareMask[i];
            Intel intel(type, sunk);
            agent.updateState(intel);
            agent.print();
        }
    }
}

/** Agent code **/

Agent::Agent()
{
    shots.set(0, 0);
    hits.set(0, 0);
    provenSunk.set(0, 0);
    for (int i = 0; i < 5; i++)
    {
        sunk[i] = false;
        sunkLocation[i] = 0;
        deduced[i] = false;
        shipLocation[i] = 0;
    }
    previousShot = 0;
}

void Agent::reset()
{
    shots.set(0, 0);
    hits.set(0, 0);
    provenSunk.set(0, 0);
    for (int i = 0; i < 5; i++)
    {
        sunk[i] = false;
        sunkLocation[i] = 0;
        deduced[i] = false;
        shipLocation[i] = 0;
    }
    previousShot = 0;
}

void Agent::print()
{
    char shipName[5] = {'D', 'C', 'S', 'B', 'A'};
    Bitboard uncovered(false);
    for (int s = 0; s < 5; s++)
    {
        if (deduced[s])
        {
            uncovered |= shipMask[SHIPSIZE(s) - 2][shipLocation[s]];
        }
    }

    std::cout << "\n\n     1   2   3   4   5   6   7   8   9  10\n   +---+---+---+---+---+---+---+---+---+---+\n";
    for (int i = 0; i < 100; ++i)
    {
        bool shotFlag = (shots & squareMask[i]);
        bool hitFlag  = (hits  & squareMask[i]);
        bool sunkFlag = (provenSunk & squareMask[i]);
        bool deduceFlag = (uncovered & ~shots & squareMask[i]);
        if (i % 10 == 0) std::cout << " " << char('A' + i / 10) << " |";
        std::cout << " " << (shotFlag ? (hitFlag ? (sunkFlag ? "S" : "H") : "=") : (deduceFlag ? "D" : " ")) << " |";
        std::cout << (i % 10 == 9 ? "\n   +---+---+---+---+---+---+---+---+---+---+\n" : "");
    }
    std::cout << "\n";/*
    for (int s = 0; s < 5; s++)
    {
        std::cout << " " << shipName[s] << " Sunk:    " << (sunk[s] ? "Yes" : "No") << "\n   Deduced: " << (deduced[s] ? "Yes" : "No") << "\n";
    }
    std::cout << "\n";*/
}

void policyPerformance(int iter)
{
    int frequency[100] = {0};
    Opponent opponent('F');
    Agent agent;
    int move;
    for (int i = 0; i < iter; i++)
    {
        //randomStatus();
        //setSeed(122379448, 2002401382);
        opponent.reset();
        agent.reset();
        while(!opponent.allSunk())
        {
            move = agent.smartHuntPolicy();
            Intel intel = opponent.autoMessage(move);
            agent.updateState(intel);
            if (false)
            {
                opponent.showShips();
                agent.print();
                intel.print();
            }
        }
        frequency[opponent.ply]++;
        if (!(i % 1000))
           std::cout << i << "\n";
    }
    for (int i = 0; i < 100; i++)
    {
        std::cout << (double) frequency[i] / iter << "\n";
    }
}

int Agent::move()
{
    char a, b;
    std::cout << "Enter square: ";
    std::cin >> a;
    std::cin >> b;
    previousShot = 10 * (a - 'A') + (b - '1');
    return previousShot;
}

void Agent::updateState(Intel intel)
{
    // Process intel
    shots |= squareMask[previousShot];
    if (intel.type != 'M')
    {
        hits |= squareMask[previousShot];
    }
    if (intel.type == 'S')
    {
        int foundShipIndex  = (intel.sunkShip == 'D') ? 0 :
                              (intel.sunkShip == 'C') ? 1 :
                              (intel.sunkShip == 'S') ? 2 :
                              (intel.sunkShip == 'B') ? 3 :
                              (intel.sunkShip == 'A') ? 4 : -1;
        sunk[foundShipIndex] = true;
        sunkLocation[foundShipIndex] = previousShot;
        // location had already been deduced
        if (deduced[foundShipIndex])
        {
            provenSunk |= shipMask[SHIPSIZE(foundShipIndex) - 2][shipLocation[foundShipIndex]];
        }
    }
    // Determine whether ship locations can be deduced
    // We repeat at most 5 times because deductions can cascade.
    for (int rep = 0; rep < 5; rep++)
    {
        int extraDeductions = 0;
        for (int s = 0; s < 5; s++)
        {
            Bitboard foundShips = provenSunk;
            int unknownShips = 5;
            for (int a = 0; a < 5; a++)
            {

                if (sunk[a])
                {
                    foundShips |= squareMask[sunkLocation[a]];
                }
                if (deduced[a])
                {
                    unknownShips--;
                    foundShips |= shipMask[SHIPSIZE(a) - 2][shipLocation[a]];
                }
            }
            // if already deduced, continue
            if (deduced[s])
            {
                continue;
            }
            int options = 0;
            int deduction = 0;
            for (int i = 0; i < 220 - 20 * SHIPSIZE(s); i++)
            {
                Bitboard ship = shipMask[SHIPSIZE(s) - 2][i];
                // If sunk, a ship must cover hit squares, intersect its sinking square
                // and not intersect other found ships
                if (sunk[s])
                {
                    if ((ship & squareMask[sunkLocation[s]]) &&
                       ((ship & hits) == ship) &&
                       !(ship & (foundShips ^ squareMask[sunkLocation[s]])))
                    {
                        options++;
                        deduction = i;
                    }
                }
                else
                {
                    // A ship cannot be completely hit and not sunk, a shot cannot cross a miss or a found ship
                    if (((ship & hits) != ship) &&
                        !(ship & ((shots & ~hits) | foundShips)) &&
                        (((ship & (hits & ~foundShips)) == (hits & ~foundShips)) || (unknownShips > 1)))
                    {
                        options++;
                        deduction = i;
                    }
                }
                if (options > 1)
                {
                    break;
                }
            }
            if (options == 1)
            {
                shipLocation[s] = deduction;
                deduced[s] = true;
                if (sunk[s])
                {
                    provenSunk |= shipMask[SHIPSIZE(s) - 2][deduction];
                    /*std::cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
                    std::cout << deduction << "\n";
                    provenSunk.print();*/
                }
                extraDeductions++;
            }
        }
        if (!extraDeductions)
        {
            break;
        }
    }
}

int Agent::huntPolicy()
{
    Bitboard deducedSquares(false);
    for (int s = 0; s < 5; s++)
    {
        if (deduced[s])
        {
            deducedSquares |= shipMask[SHIPSIZE(s) - 2][shipLocation[s]];
        }
    }
    deducedSquares &= ~shots;
    if (deducedSquares)
    {
        previousShot = deducedSquares.BSF();
        return previousShot;
    }

    int result;
    if (hits == provenSunk)
    {
        while (true)
        {
            result = randomInteger() % 100;
            if (((result + (result / 10)) % 2 == 0) && ((~shots) & squareMask[result]))
            {
                previousShot = result;
                return previousShot;
            }
        }
    }
    Bitboard targets = hits & (~provenSunk);
    const Bitboard d1(0xEFFBFEFFBFEFFBFE, 0x0000000FFBFEFFBF);
    const Bitboard d2(0xF7FDFF7FDFF7FDFF, 0x00000007FDFF7FDF);
    const Bitboard d3(0xFFFFFFFFFFFFFC00, 0x0000000FFFFFFFFF);
    const Bitboard d4(0xFFFFFFFFFFFFFFFF, 0x0000000003FFFFFF);

    targets = ((targets & d1) >> 1) | ((targets & d2) << 1) | ((targets & d3) >> 10) | ((targets & d4) << 10);
    targets = targets & (~shots);

    int index = randomInteger() % (targets.count());
    for (int i = 0; i < index - 1; i++)
    {
        targets.removeLSB();
    }
    result = targets.BSF();
    previousShot = result;
    return previousShot;
}

int Agent::smartHuntPolicy()
{
    // Deduced squares are always a hit.
    Bitboard deducedSquares(false);
    for (int s = 0; s < 5; s++)
    {
        if (deduced[s])
        {
            deducedSquares |= shipMask[SHIPSIZE(s) - 2][shipLocation[s]];
        }
    }
    deducedSquares &= ~shots;
    if (deducedSquares)
    {
        previousShot = deducedSquares.BSF();
        return previousShot;
    }


    int expectedShips[100] = {0};
    bool valid;
    for (int s = 0; s < 5; s++)
    {
        Bitboard foundShips = provenSunk;
        for (int a = 0; a < 5; a++)
        {
            if (sunk[a])
            {
                foundShips |= squareMask[sunkLocation[a]];
            }
            if (deduced[a])
            {
                foundShips |= shipMask[SHIPSIZE(a) - 2][shipLocation[a]];
            }
        }
        // if already deduced, continue
        if (deduced[s])
        {
            continue;
        }
        int options = 0;
        int deduction = 0;
        for (int i = 0; i < 220 - 20 * SHIPSIZE(s); i++)
        {
            valid = false;
            Bitboard ship = shipMask[SHIPSIZE(s) - 2][i];
            // If sunk, a ship must cover hit squares, intersect its sinking square
            // and not intersect other found ships
            if (sunk[s])
            {
                if ((ship & squareMask[sunkLocation[s]]) &&
                    ((ship & hits) == ship) &&
                    !(ship & (foundShips ^ squareMask[sunkLocation[s]])))
                {
                    valid = true;
                }
            }
            else
            {
                // A ship cannot be completely hit and not sunk, a shot cannot cross a miss or a found ship
                if (((ship & hits) != ship) &&
                    !(ship & ((shots & ~hits) | foundShips)))
                {
                    valid = true;
                }
            }
            if (valid)
            {
                int start = ship.BSF();
                for (int j = 0; j < SHIPSIZE(s); j++)
                {
                    expectedShips[start + j * (i < 110 - 10 * SHIPSIZE(s) ? 1 : 10)]++;
                }
            }
        }
    }

    Bitboard subset(0xFFFFFFFFFFFFFFFF, 0x0000000FFFFFFFFF);

    const Bitboard blockMasks[8] = {
        Bitboard(0xA556AA556AA556AA, 0x0000000556AA556A),
        Bitboard(0x5AA955AA955AA955, 0x0000000AA955AA95),
        Bitboard(0x4924924924924924, 0x0000000492492492),
        Bitboard(0x2492492492492492, 0x0000000249249249),
        Bitboard(0x9249249249249249, 0x0000000924924924),
        Bitboard(0x9490929252424A49, 0x00000009252424A4),
        Bitboard(0x29252424A4949092, 0x000000024A494909),
        Bitboard(0x424A494909292524, 0x0000000490929252)
    };

    int squaresLeft = 101;

    if (!sunk[0])
    {
        for (int i = 0; i < 2; i++)
        {
            if ((blockMasks[i] & (~shots)).count() < squaresLeft)
            {
                squaresLeft = (blockMasks[i] & (~shots)).count();
                subset = blockMasks[i];
            }
        }
    }/*
    else if (!(sunk[1] && sunk[2]))
    {
        for (int i = 2; i < 8; i++)
        {
            if ((blockMasks[i] & (~shots)).count() <  squaresLeft)
            {
                squaresLeft = (blockMasks[i] & (~shots)).count();
                subset = blockMasks[i];
            }
        }
    }*/
    else
    {
        subset.set(0xFFFFFFFFFFFFFFFF, 0x0000000FFFFFFFFF);
    }
    if (hits != provenSunk)
    {
        subset = hits & (~provenSunk);
        const Bitboard d1(0xEFFBFEFFBFEFFBFE, 0x0000000FFBFEFFBF);
        const Bitboard d2(0xF7FDFF7FDFF7FDFF, 0x00000007FDFF7FDF);
        const Bitboard d3(0xFFFFFFFFFFFFFC00, 0x0000000FFFFFFFFF);
        const Bitboard d4(0xFFFFFFFFFFFFFFFF, 0x0000000003FFFFFF);
        subset = ((subset & d1) >> 1) | ((subset & d2) << 1) | ((subset & d3) >> 10) | ((subset & d4) << 10);
    }

    subset = subset & (~shots);
    int result = subset.BSF();

    for (int i = 0; i < 100; i++)
    {
        if ((~subset) & squareMask[i])
            continue;
        result = (expectedShips[result] > expectedShips[i] ? result : i);
    }
    previousShot = result;
    return previousShot;
}


int Agent::expectationPolicy()
{
    // Deduced squares are always a hit.
    Bitboard deducedSquares(false);
    for (int s = 0; s < 5; s++)
    {
        if (deduced[s])
        {
            deducedSquares |= shipMask[SHIPSIZE(s) - 2][shipLocation[s]];
        }
    }
    deducedSquares &= ~shots;
    if (deducedSquares)
    {
        previousShot = deducedSquares.BSF();
        return previousShot;
    }

    // Remaining ships
    int squaresLeft = 17;
    int shipsLeft = 0;
    int sizes[5] = {0};
    bool sunkInd[5] = {0};
    int sunkLoc[5] = {0};
    for (int s = 0; s < 5; s++)
    {
        squaresLeft -= deduced[s] * SHIPSIZE(s);
        sizes[shipsLeft] = SHIPSIZE(s);
        sunkInd[shipsLeft] = sunk[s];
        sunkLoc[shipsLeft] = sunkLocation[s];
        shipsLeft += !deduced[s];
    }

    if (shipsLeft > 3)
    {
        return smartHuntPolicy();
    }


    // remaining ship permutations
    int shipOptions[shipsLeft][180];
    int counts[shipsLeft];

    bool valid;
    Bitboard foundShips = provenSunk;
    for (int a = 0; a < 5; a++)
    {
        if (sunk[a])
        {
            foundShips |= squareMask[sunkLocation[a]];
        }
        if (deduced[a])
        {
            foundShips |= shipMask[SHIPSIZE(a) - 2][shipLocation[a]];
        }
    }

    Bitboard fire = hits & (~provenSunk);
    for (int s = 0; s < shipsLeft; s++)
    {
        int index = 0;
        for (int i = 0; i < 220 - 20 * sizes[s]; i++)
        {
            valid = false;
            Bitboard ship = shipMask[sizes[s] - 2][i];
            // If sunk, a ship must cover hit squares, intersect its sinking square
            // and not intersect other found ships
            if (sunkInd[s])
            {
                if ((ship & squareMask[sunkLoc[s]]) &&
                    ((ship & hits) == ship) &&
                    !(ship & (foundShips ^ squareMask[sunkLoc[s]])))
                {
                    valid = true;
                }
            }
            else
            {
                // A ship cannot be completely hit and not sunk, a shot cannot cross a miss or a found ship
                if (((ship & hits) != ship) &&
                    !(ship & ((shots & ~hits) | foundShips)) &&
                    (shipsLeft != 1 || ((ship & fire) == fire)))
                {
                    valid = true;
                }
            }
            if (valid)
            {
                shipOptions[s][index] = i;
                index++;
            }
        }
        counts[s] = index;
    }

    // brute force combinations
    int expectedShips[100] = {0};
    int shipCounts[shipsLeft][180] = {0};
    int good = 0;

    if (shipsLeft == 1)
    {
        for (int a = 0; a < counts[0]; a++)
        {
            good++;
            shipCounts[0][a]++;
        }
    }
    else if (shipsLeft == 2)
    {
        for (int a = 0; a < counts[0]; a++)
        {
            for (int b = 0; b < counts[1]; b++)
            {
                Bitboard placement = shipMask[sizes[0] - 2][shipOptions[0][a]] |
                                     shipMask[sizes[1] - 2][shipOptions[1][b]];
                if ((placement.count() == squaresLeft) && ((placement & fire) == fire))
                {
                    good++;
                    shipCounts[0][a]++;
                    shipCounts[1][b]++;
                }
            }
        }
    }
    else if (shipsLeft == 3)
    {
        Bitboard shipA;
        Bitboard shipB;
        Bitboard shipC;
        for (int a = 0; a < counts[0]; a++)
        {
            shipA = shipMask[sizes[0] - 2][shipOptions[0][a]];
            for (int b = 0; b < counts[1]; b++)
            {
                shipB = shipMask[sizes[1] - 2][shipOptions[1][b]];
                if (shipA & shipB)
                    continue;
                for (int c = 0; c < counts[2]; c++)
                {
                    shipC = shipMask[sizes[2] - 2][shipOptions[2][c]];
                    if (!((shipA | shipB) & shipC) && (((shipA | shipB | shipC) & fire) == fire))
                    {
                        good++;
                        shipCounts[0][a]++;
                        shipCounts[1][b]++;
                        shipCounts[2][c]++;
                    }

                }
            }
        }
    }
    // count square frequencies
    for (int s = 0; s < shipsLeft; s++)
    {
        for (int i = 0; i < counts[s]; i++)
        {
            int start = shipMask[sizes[s] - 2][shipOptions[s][i]].BSF();
            for (int j = 0; j < sizes[s]; j++)
            {
                expectedShips[start + j * (shipOptions[s][i] < 110 - 10 * sizes[s] ? 1 : 10)] += shipCounts[s][i];
            }

        }
    }

    int result = (~shots).BSF();

    for (int i = 0; i < 100; i++)
    {
        if (shots & squareMask[i])
            continue;
        result = (expectedShips[result] > expectedShips[i] ? result : i);
    }
    previousShot = result;
    return previousShot;
}


int Agent::enumerationPolicy(double global_p)
{
    // Deduced squares are always a hit.
    Bitboard deducedSquares(false);
    for (int s = 0; s < 5; s++)
    {
        if (deduced[s])
        {
            deducedSquares |= shipMask[SHIPSIZE(s) - 2][shipLocation[s]];
        }
    }
    deducedSquares &= ~shots;
    if (deducedSquares)
    {
        previousShot = deducedSquares.BSF();
        return previousShot;
    }

    // Remaining ships
    int squaresLeft = 17;
    int shipsLeft = 0;
    int sizes[5] = {0};
    bool sunkInd[5] = {0};
    int sunkLoc[5] = {0};
    for (int s = 0; s < 5; s++)
    {
        squaresLeft -= deduced[s] * SHIPSIZE(s);
        sizes[shipsLeft] = SHIPSIZE(s);
        sunkInd[shipsLeft] = sunk[s];
        sunkLoc[shipsLeft] = sunkLocation[s];
        shipsLeft += !deduced[s];
    }

    if (shipsLeft > 3)
    {
        return smartHuntPolicy();
    }


    // remaining ship permutations
    int shipOptions[shipsLeft][180];
    int counts[shipsLeft];

    bool valid;
    Bitboard foundShips = provenSunk;
    for (int a = 0; a < 5; a++)
    {
        if (sunk[a])
        {
            foundShips |= squareMask[sunkLocation[a]];
        }
        if (deduced[a])
        {
            foundShips |= shipMask[SHIPSIZE(a) - 2][shipLocation[a]];
        }
    }

    for (int s = 0; s < shipsLeft; s++)
    {
        int index = 0;
        for (int i = 0; i < 220 - 20 * sizes[s]; i++)
        {
            valid = false;
            Bitboard ship = shipMask[sizes[s] - 2][i];
            // If sunk, a ship must cover hit squares, intersect its sinking square
            // and not intersect other found ships
            if (sunkInd[s])
            {
                if ((ship & squareMask[sunkLoc[s]]) &&
                    ((ship & hits) == ship) &&
                    !(ship & (foundShips ^ squareMask[sunkLoc[s]])))
                {
                    valid = true;
                }
            }
            else
            {
                // A ship cannot be completely hit and not sunk, a shot cannot cross a miss or a found ship
                if (((ship & hits) != ship) &&
                    !(ship & ((shots & ~hits) | foundShips)))
                {
                    valid = true;
                }
            }
            if (valid)
            {
                shipOptions[s][index] = i;
                index++;
            }
        }
        counts[s] = index;
    }

    // brute force combinations
    double expectedShips[100] = {0};
    double shipCounts[shipsLeft][180] = {0};
    int good = 0;
    Bitboard fire = hits & (~provenSunk);

    Bitboard sunkBorder = provenSunk.adjacent();

    if (shipsLeft == 1)
    {
        for (int a = 0; a < counts[0]; a++)
        {
            Bitboard placement = shipMask[sizes[0] - 2][shipOptions[0][a]];
            Bitboard border = shipBorder[sizes[0] - 2][shipOptions[0][a]];
            bool nonadj = (!(placement & border) && !(placement & sunkBorder));
            if ((placement.count() == squaresLeft) && ((placement & fire) == fire))
            {
                good++;
                shipCounts[0][shipOptions[0][a]] += (nonadj ? global_p : 1 - global_p);
            }
        }
    }
    else if (shipsLeft == 2)
    {
        for (int a = 0; a < counts[0]; a++)
        {
            for (int b = 0; b < counts[1]; b++)
            {
                Bitboard placement = shipMask[sizes[0] - 2][shipOptions[0][a]] |
                                     shipMask[sizes[1] - 2][shipOptions[1][b]];
                Bitboard border = shipBorder[sizes[0] - 2][shipOptions[0][a]] |
                                shipBorder[sizes[1] - 2][shipOptions[1][b]];
                bool nonadj = (!(placement & border) && !(placement & sunkBorder));
                if ((placement.count() == squaresLeft) && ((placement & fire) == fire))
                {
                    good++;
                    shipCounts[0][shipOptions[0][a]] += (nonadj ? global_p : 1 - global_p);
                    shipCounts[1][shipOptions[1][b]] += (nonadj ? global_p : 1 - global_p);
                }
            }
        }
    }
    else if (shipsLeft == 3)
    {
        for (int a = 0; a < counts[0]; a++)
        {
            for (int b = 0; b < counts[1]; b++)
            {
                for (int c = 0; c < counts[2]; c++)
                {
                    Bitboard placement = shipMask[sizes[0] - 2][shipOptions[0][a]] |
                                         shipMask[sizes[1] - 2][shipOptions[1][b]] |
                                         shipMask[sizes[2] - 2][shipOptions[2][c]];
                    Bitboard border = shipBorder[sizes[0] - 2][shipOptions[0][a]] |
                                      shipBorder[sizes[1] - 2][shipOptions[1][b]] |
                                      shipBorder[sizes[2] - 2][shipOptions[2][c]];
                    bool nonadj = (!(placement & border) && !(placement & sunkBorder));

                    if ((placement.count() == squaresLeft) && ((placement & fire) == fire))
                    {
                        good++;
                        shipCounts[0][shipOptions[0][a]] += (nonadj ? global_p : 1 - global_p);
                        shipCounts[1][shipOptions[1][b]] += (nonadj ? global_p : 1 - global_p);
                        shipCounts[2][shipOptions[2][c]] += (nonadj ? global_p : 1 - global_p);
                    }
                }
            }
        }
    }
    else if (shipsLeft == 4)
    {
        for (int a = 0; a < counts[0]; a++)
        {
            for (int b = 0; b < counts[1]; b++)
            {
                for (int c = 0; c < counts[2]; c++)
                {
                    for (int d = 0; d < counts[3]; d++)
                    {
                        Bitboard placement = shipMask[sizes[0] - 2][shipOptions[0][a]] |
                                             shipMask[sizes[1] - 2][shipOptions[1][b]] |
                                             shipMask[sizes[2] - 2][shipOptions[2][c]] |
                                             shipMask[sizes[3] - 2][shipOptions[3][d]];
                        if ((placement.count() == squaresLeft) && ((placement & fire) == fire))
                        {
                            good++;
                            shipCounts[0][shipOptions[0][a]]++;
                            shipCounts[1][shipOptions[1][b]]++;
                            shipCounts[2][shipOptions[2][c]]++;
                            shipCounts[3][shipOptions[3][d]]++;
                        }
                    }
                }
            }
        }
    }
    // count square frequencies
    for (int s = 0; s < shipsLeft; s++)
    {
        for (int i = 0; i < 220 - 20 * sizes[s]; i++)
        {
            if (shipCounts[s][i] > 0)
            {
                int start = shipMask[sizes[s] - 2][i].BSF();
                for (int j = 0; j < sizes[s]; j++)
                {
                    expectedShips[start + j * (i < 110 - 10 * sizes[s] ? 1 : 10)] += shipCounts[s][i];
                }
            }
        }
    }

    int result = (~shots).BSF();

    for (int i = 0; i < 100; i++)
    {
        if (shots & squareMask[i])
            continue;
        result = (expectedShips[result] > expectedShips[i] ? result : i);
    }
    previousShot = result;
    return previousShot;
}
