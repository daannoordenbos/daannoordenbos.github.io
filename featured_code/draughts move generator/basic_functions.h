#ifndef BASIC_FUNCTIONS_H_INCLUDED
#define BASIC_FUNCTIONS_H_INCLUDED

#define U64 unsigned long long
// parallel bit extract (PEXT) useful for indexing
//#define countBits(x) (__builtin_popcountll(x))

// board constants
const U64 border        = 0b1111111111000000000010000000000100000000001000000000010000000000ULL;
const U64 aliveSquares  = 0b0000000000111111111101111111111011111111110111111111101111111111ULL;
// random functions
int   random_int        ();
void  setRandom         ();
// time functions
int   getTime           ();
void  sleep             (int);
U64   accurateTime      ();
// bit intrinsic functions
#define BSF(x)          (__builtin_ctzll(x))        // best by test
#define BSR(x)          (__builtin_clzll(x) ^ 63)   // best by test
#define countBits(x)    (__builtin_popcountll(x))   // best by test
U64   get_msb           (U64);
// board functions
void  printBitboard     (U64);
void  board             (U64*);
void  invert            (U64*);
bool  noThreat          (U64*);
bool  quiet             (U64*, int);
void  printMove         (U64*, U64*, int);

#define R01 (1ULL <<  0)
#define R02 (1ULL <<  1)
#define R03 (1ULL <<  2)
#define R04 (1ULL <<  3)
#define R05 (1ULL <<  4)
#define R06 (1ULL <<  5)
#define R07 (1ULL <<  6)
#define R08 (1ULL <<  7)
#define R09 (1ULL <<  8)
#define R10 (1ULL <<  9)
#define R11 (1ULL << 11)
#define R12 (1ULL << 12)
#define R13 (1ULL << 13)
#define R14 (1ULL << 14)
#define R15 (1ULL << 15)
#define R16 (1ULL << 16)
#define R17 (1ULL << 17)
#define R18 (1ULL << 18)
#define R19 (1ULL << 19)
#define R20 (1ULL << 20)
#define R21 (1ULL << 22)
#define R22 (1ULL << 23)
#define R23 (1ULL << 24)
#define R24 (1ULL << 25)
#define R25 (1ULL << 26)
#define R26 (1ULL << 27)
#define R27 (1ULL << 28)
#define R28 (1ULL << 29)
#define R29 (1ULL << 30)
#define R30 (1ULL << 31)
#define R31 (1ULL << 33)
#define R32 (1ULL << 34)
#define R33 (1ULL << 35)
#define R34 (1ULL << 36)
#define R35 (1ULL << 37)
#define R36 (1ULL << 38)
#define R37 (1ULL << 39)
#define R38 (1ULL << 40)
#define R39 (1ULL << 41)
#define R40 (1ULL << 42)
#define R41 (1ULL << 44)
#define R42 (1ULL << 45)
#define R43 (1ULL << 46)
#define R44 (1ULL << 47)
#define R45 (1ULL << 48)
#define R46 (1ULL << 49)
#define R47 (1ULL << 50)
#define R48 (1ULL << 51)
#define R49 (1ULL << 52)
#define R50 (1ULL << 53)

/*  reversed square numbers for black's perspective */
#define S50 (1ULL <<  0)
#define S49 (1ULL <<  1)
#define S48 (1ULL <<  2)
#define S47 (1ULL <<  3)
#define S46 (1ULL <<  4)
#define S45 (1ULL <<  5)
#define S44 (1ULL <<  6)
#define S43 (1ULL <<  7)
#define S42 (1ULL <<  8)
#define S41 (1ULL <<  9)
#define S40 (1ULL << 11)
#define S39 (1ULL << 12)
#define S38 (1ULL << 13)
#define S37 (1ULL << 14)
#define S36 (1ULL << 15)
#define S35 (1ULL << 16)
#define S34 (1ULL << 17)
#define S33 (1ULL << 18)
#define S32 (1ULL << 19)
#define S31 (1ULL << 20)
#define S30 (1ULL << 22)
#define S29 (1ULL << 23)
#define S28 (1ULL << 24)
#define S27 (1ULL << 25)
#define S26 (1ULL << 26)
#define S25 (1ULL << 27)
#define S24 (1ULL << 28)
#define S23 (1ULL << 29)
#define S22 (1ULL << 30)
#define S21 (1ULL << 31)
#define S20 (1ULL << 33)
#define S19 (1ULL << 34)
#define S18 (1ULL << 35)
#define S17 (1ULL << 36)
#define S16 (1ULL << 37)
#define S15 (1ULL << 38)
#define S14 (1ULL << 39)
#define S13 (1ULL << 40)
#define S12 (1ULL << 41)
#define S11 (1ULL << 42)
#define S10 (1ULL << 44)
#define S09 (1ULL << 45)
#define S08 (1ULL << 46)
#define S07 (1ULL << 47)
#define S06 (1ULL << 48)
#define S05 (1ULL << 49)
#define S04 (1ULL << 50)
#define S03 (1ULL << 51)
#define S02 (1ULL << 52)
#define S01 (1ULL << 53)

/* rows, as counted from the top of the board */
#define ROW1 (S01 | S02 | S03 | S04 | S05)
#define ROW2 (S06 | S07 | S08 | S09 | S10)
#define ROW3 (S11 | S12 | S13 | S14 | S15)
#define ROW4 (S16 | S17 | S18 | S19 | S20)
#define ROW5 (S21 | S22 | S23 | S24 | S25)
#define ROW6 (S26 | S27 | S28 | S29 | S30)
#define ROW7 (S31 | S32 | S33 | S34 | S35)
#define ROW8 (S36 | S37 | S38 | S39 | S40)
#define ROW9 (S41 | S42 | S43 | S44 | S45)
#define ROW10 (S46 | S47 | S48 | S49 | S50)

/* rows, as counted from the bottom of the board */
#define ROB10 (S01 | S02 | S03 | S04 | S05)
#define ROB9 (S06 | S07 | S08 | S09 | S10)
#define ROB8 (S11 | S12 | S13 | S14 | S15)
#define ROB7 (S16 | S17 | S18 | S19 | S20)
#define ROB6 (S21 | S22 | S23 | S24 | S25)
#define ROB5 (S26 | S27 | S28 | S29 | S30)
#define ROB4 (S31 | S32 | S33 | S34 | S35)
#define ROB3 (S36 | S37 | S38 | S39 | S40)
#define ROB2 (S41 | S42 | S43 | S44 | S45)
#define ROB1 (S46 | S47 | S48 | S49 | S50)

/* columns, as counted from the left of the board */
#define COL1 (S06 | S16 | S26 | S36 | S46)
#define COL2 (S01 | S11 | S21 | S31 | S41)
#define COL3 (S07 | S17 | S27 | S37 | S47)
#define COL4 (S02 | S12 | S22 | S32 | S42)
#define COL5 (S08 | S18 | S28 | S38 | S48)
#define COL6 (S03 | S13 | S23 | S33 | S43)
#define COL7 (S09 | S19 | S29 | S39 | S49)
#define COL8 (S04 | S14 | S24 | S34 | S44)
#define COL9 (S10 | S20 | S30 | S40 | S50)
#define COL10 (S05 | S15 | S25 | S35 | S45)

#endif // BASIC_FUNCTIONS_H_INCLUDED
