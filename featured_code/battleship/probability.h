#ifndef PROBABILITY_H_INCLUDED
#define PROBABILITY_H_INCLUDED

#define SHIPSIZE(i) (2 + i - (i > 1))

struct Intel {
    char type = 'M';
    char sunkShip = '-';
    Intel()
    {

    }
    Intel(char shotType, char sunkType)
    {
        type = shotType;
        sunkShip = sunkType;
    }
    void print()
    {
        std::cout << "Intel: " << type << sunkShip << "\n";
    }
};

struct Opponent {
    Bitboard shots;
    bool automatic;
    int  layout[5];
    bool sunk[5];
    int  ply;

    // Mode: F -> follower, I -> interactive, A -> automatic
    Opponent(char mode);
    bool allSunk();
    void showShips();
    void reset();
    void randomLayout();
    void randomGoodLayout();
    void customLayout();
    Intel autoMessage(int square);
};


struct Agent {
    Bitboard shots, hits, provenSunk;
    bool sunk[5];
    unsigned int sunkLocation[5];
    bool deduced[5];
    unsigned int shipLocation[5];
    unsigned int previousShot;

    Agent();
    void print();
    void reset();
    // Update shots, hits, sunk and sunk location.
    void updateState(Intel intel);
    // Policies
    int move();
    int huntPolicy();
    int smartHuntPolicy();
    int expectationPolicy();
    int enumerationPolicy(double);
};

void initialize();
void policyPerformance(int iter);
void playGame(Opponent);



#endif // PROBABILITY_H_INCLUDED
