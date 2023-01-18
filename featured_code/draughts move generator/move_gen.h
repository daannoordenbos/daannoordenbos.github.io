#ifndef MOVE_GEN_H_INCLUDED
#define MOVE_GEN_H_INCLUDED

void moveGeneratorTest  (U64*, int, int);
U64  perft              (U64*, int, int, int);
void initMoveGen        ();
void makeMove           (U64*, U64[3]);

void slide              (U64*, U64(*)[3], int);

void kingRecursive      (U64*, U64(*)[3], int, int, int);
void manRecursive       (U64*, U64(*)[3], int, int, U64);
void captures           (U64*, U64(*)[3], int);

#endif // MOVE_GEN_H_INCLUDED
