#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>

#include "basic_functions.h"
#include "move_gen.h"

bool PerftTest = false;

U64 trash[16][3] = {0};

U64 direction_masks[54][4] = {0};


void initMoveGen(){
    int d[4] = {5, 6, -5, -6}; // 2 and 3 BSR
    for (int square = 0; square < 54; square++) {
        for (int i = 0; i < 4; i++) {
            for (int m = 1; m < 9; m++) {
                if ((aliveSquares & (1ULL << (square + m * d[i]))) == 0){
                    break;
                }
                direction_masks[square][i] |= 1ULL << (square + m * d[i]);
            }
            //printBitboard(direction_masks[square][i]);
        }
    }
}

U64 moveStack[35][100][3] = {0};

U64 perft(U64* pos, int depth, int plies, int color){
    //if (depth == 0) return 1;
    U64 num = 0;
    captures(pos, moveStack[plies], color);
    if (moveStack[plies][0][0] == 0) slide(pos, moveStack[plies], color);
    if (depth == 1) return moveStack[plies][0][0];
    for (int i = 1; i < (int)moveStack[plies][0][0] + 1; i++) {
        num += perft(moveStack[plies][i], depth - 1, plies + 1, -color);
    }
    return num;
}

void moveGeneratorTest(U64* pos, int color, int depth){
    PerftTest = true;
    board(pos);
    U64 total = 0;
    for (int i = 1; i < depth + 1; i++) {
        double start = getTime();
        total = perft(pos, i, 0, color);
        double time = getTime() - start;
        std::cout << "Perft(" << i << ")";
        if (i < 10) std::cout<<" ";
        std::cout<< " N = " << total;
        for (int s = 0; s < 14 - floor(log10(total)); s++)std::cout<<" ";
        std::cout  <<time/1000<<" sec\t   " << " Kn/s: ";
        if (time > 0) std::cout << (int) (total/time) << "\n";
        else std::cout << "0\n";
    }
    PerftTest = false;
}

void makeMove(U64* pos, U64 option[3]){
    pos[0] ^= option[0];
    pos[1] ^= option[1];
    pos[2] ^= option[2];
}

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

void manRecursive(U64* pos, U64 moves[100][3], int depth, int color, U64 central){
    U64 free = aliveSquares & (~(pos[0] | pos[2]));
    U64 opponent = pos[1 - color];
    bool no_capture = true;


    if ((opponent & (central << 5)) && (free & (central << 10)))  {
        no_capture = false;
        trash[depth][1+color] = (central) | (central << 10);
        trash[depth][1-color] = central << 5;
        trash[depth][1]       = (central << 5) & pos[1];

        makeMove(pos, trash[depth]);
        manRecursive(pos, moves, depth + 1, color, central << 10);
        makeMove(pos, trash[depth]);
    }
    if ((opponent & (central << 6)) && (free & (central << 12)))  {
        no_capture = false;
        trash[depth][1+color] = (central) | (central << 12);
        trash[depth][1-color] = central << 6;
        trash[depth][1]       = (central << 6) & pos[1];

        makeMove(pos, trash[depth]);
        manRecursive(pos, moves, depth + 1, color, central << 12);
        makeMove(pos, trash[depth]);
    }
    if ((opponent & (central >> 5)) && (free & (central >> 10)))  {
        no_capture = false;
        trash[depth][1+color] = (central) | (central >> 10);
        trash[depth][1-color] = central >> 5;
        trash[depth][1]       = (central >> 5) & pos[1];

        makeMove(pos, trash[depth]);
        manRecursive(pos, moves, depth + 1, color, central >> 10);
        makeMove(pos, trash[depth]);
    }
    if ((opponent & (central >> 6)) && (free & (central >> 12)))  {
        no_capture = false;
        trash[depth][1+color] = (central) | (central >> 12);
        trash[depth][1-color] = central >> 6;
        trash[depth][1]       = (central >> 6) & pos[1];

        makeMove(pos, trash[depth]);
        manRecursive(pos, moves, depth + 1, color, central >> 12);
        makeMove(pos, trash[depth]);
    }

    if(no_capture){
        int index;
        if (depth == moves[0][1]) {
            index = moves[0][0] + 1;
            moves[index][0] = trash[0][0];
            moves[index][1] = trash[0][1];
            moves[index][2] = trash[0][2];
            for (int i = 1; i < depth; i++) {
                moves[index][0] ^= trash[i][0];
                moves[index][1] ^= trash[i][1];
                moves[index][2] ^= trash[i][2];
            }
            moves[index][0] ^= trash[15][0];
            moves[index][1] ^= trash[15][1];
            moves[index][2] ^= trash[15][2];

            // promotions
            moves[index][1] |= moves[index][0] & ROB1;
            moves[index][1] |= moves[index][2] & ROW1;

            moves[0][0]++;
        }
        if (depth > moves[0][1]) {
            moves[0][0] = 1;
            moves[1][0] = trash[0][0];
            moves[1][1] = trash[0][1];
            moves[1][2] = trash[0][2];
            for (int i = 1; i < depth; i++) {
                moves[1][0] ^= trash[i][0];
                moves[1][1] ^= trash[i][1];
                moves[1][2] ^= trash[i][2];
            }
            moves[1][0] ^= trash[15][0];
            moves[1][1] ^= trash[15][1];
            moves[1][2] ^= trash[15][2];

            // promotions
            moves[1][1] |= moves[1][0] & ROB1;
            moves[1][1] |= moves[1][2] & ROW1;

            moves[0][1] = depth; // set most captures
        }
    }

}

int direc[13][3] = {{-5,-6,5},{-5,-6,6},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{5,6,-6},{5,6,-5}};
int direc_2[13][2] = {{-5,5},{-6,6},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{6,-6},{5,-5}};
int all_d[4] = {5, -5,- 6, 6};
U64 info_change[16][3] = {0};

template<int N>
void kingRecursive(int (&v)[N], U64* info, U64 moves[100][3], int depth, int color, int origin){
    bool no_capture = true;
    int first;
    U64 alpha;

    for(int way: v){ // loop over ways
        //if (origin != 0){
        alpha = info[0] & direction_masks[origin][(1 + ((way < 0) << 1) - (way & 1))];


        // nothing to capture
        if (alpha == 0) continue;

        if (way < 0) {
            first = BSR(alpha);
        } else {
            first = BSF(alpha);
        }
        // looking at own piece


        if((info[1] & (1ULL<<(first))) != 0){
            for(int j = 1; j < 9; j++){
                int hoop = first + j * way;
                if((info[0] & (1ULL << hoop)) != 0){
                    break;
                }
                no_capture = false;

                //if (first != origin + i  *way) std::cout<<"houston we got a problem"\n;

                U64 victim = 1ULL << (first);

                trash[depth][1+color]   = (1ULL << origin) + (1ULL << hoop);
                trash[depth][1]         = ((1ULL << origin) + (1ULL << hoop)) | (victim & info[2]);
                trash[depth][1-color]   = victim;

                info_change[depth][0]          = (1ULL << origin) + (1ULL << hoop);
                info_change[depth][1]          = victim;
                info_change[depth][2]          = victim & info[2];

                makeMove(info, info_change[depth]); // change the information
                if(j==1){
                   kingRecursive(direc[way + 6], info, moves, depth + 1, color, hoop);
                } else {
                    kingRecursive(direc_2[way + 6], info, moves, depth + 1, color, hoop);
                }
                //kingRecursive(direc[way + 6], info, moves, depth + 1, color, hoop);
                makeMove(info, info_change[depth]);
            }
        }

    }
    if((no_capture) && depth != 0){
        if(depth == moves[0][1]){
            int index = moves[0][0] + 1;
            moves[index][0] = trash[0][0];
            moves[index][1] = trash[0][1];
            moves[index][2] = trash[0][2];
            for(int i = 1; i < depth; i++){
                moves[index][0] ^= trash[i][0];
                moves[index][1] ^= trash[i][1];
                moves[index][2] ^= trash[i][2];
            }
            moves[index][0] ^= trash[15][0];
            moves[index][1] ^= trash[15][1];
            moves[index][2] ^= trash[15][2];
            moves[0][0]++;

        }
        if(depth > moves[0][1]){
            moves[0][0] = 1;
            moves[1][0] = trash[0][0];
            moves[1][1] = trash[0][1];
            moves[1][2] = trash[0][2];
            for(int i = 1; i < depth; i++){
                moves[1][0] ^= trash[i][0];
                moves[1][1] ^= trash[i][1];
                moves[1][2] ^= trash[i][2];
            }
            moves[1][0] ^= trash[15][0];
            moves[1][1] ^= trash[15][1];
            moves[1][2] ^= trash[15][2];
            moves[0][1] = depth; // set most captures
        }
    }
}

U64 temp[100][3] = {0};

void captures(U64* pos, U64 moves[100][3], int color){
    trash[15][0] = pos[0];
    trash[15][1] = pos[1];
    trash[15][2] = pos[2];
    moves[0][0] = 0;
    moves[0][1] = 0;
    U64 free = aliveSquares & (~(pos[0] | pos[2]));
    U64 opponent = pos[1-color];
    U64 our_men = pos[1+color] & (~pos[1]);

    int from;

    U64 way;
    U64 bit;

    way = (((our_men << 5) & opponent) >> 5) & (((our_men << 10) & free) >> 10); // +5 direction NE
    while (way) {
        bit = way & -way;
        way = (way - 1) & way;
        trash[0][1+color] = (bit) | (bit << 10);
        trash[0][1-color] = (bit << 5);
        trash[0][1]       = (bit << 5) & pos[1];

        makeMove(pos, trash[0]);
        manRecursive(pos, moves, 1, color, bit << 10);
        makeMove(pos, trash[0]);
    }

    way = (((our_men << 6) & opponent) >> 6) & (((our_men << 12) & free) >> 12); // +6 direction NW
    while (way) {
        bit = way & -way;
        way = (way - 1) & way;
        trash[0][1+color] = (bit) | (bit << 12);
        trash[0][1-color] = (bit << 6);
        trash[0][1]       = (bit << 6) & pos[1];

        makeMove(pos, trash[0]);
        manRecursive(pos, moves, 1, color, bit << 12);
        makeMove(pos, trash[0]);
    }

    way = (((our_men >> 5) & opponent) << 5) & (((our_men >> 10) & free) << 10); // -5 direction SW
    while (way) {
        bit = way & -way;
        way = (way - 1) & way;
        trash[0][1+color] = (bit) | (bit >> 10);
        trash[0][1-color] = (bit >> 5);
        trash[0][1]       = (bit >> 5) & pos[1];

        makeMove(pos, trash[0]);
        manRecursive(pos, moves, 1, color, bit >> 10);
        makeMove(pos, trash[0]);
    }

    way = (((our_men >> 6) & opponent) << 6) & (((our_men >> 12) & free) << 12); // -6 direction SE
    while (way) {
        bit = way & -way;
        way = (way - 1) & way;
        trash[0][1+color] = (bit) | (bit >> 12);
        trash[0][1-color] = (bit >> 6);
        trash[0][1]       = (bit >> 6) & pos[1];

        makeMove(pos, trash[0]);
        manRecursive(pos, moves, 1, color, bit >> 12);
        makeMove(pos, trash[0]);
    }


    U64 king = pos[1] & pos[1+color];
    U64 info[3] = {~free, pos[1 - color], pos[1]};

    while (king) {
        from = BSF(king);
        king = (king - 1) & king;
        kingRecursive(all_d, info, moves, 0, color, from);
    }

    /// extra accurate test, filter out duplicate captures (slow down)

    if (PerftTest) {
        if (((moves[0][1] >= 4) || ((pos[1+color] & pos[1]) != 0)) && (moves[0][0] > 1)) {
            int l = (int) moves[0][0] + 1;
            for (int i = 1; i < l - 1; i++) {
                for (int a = i + 1; a < l; a++) {
                    if ((moves[i][0] == moves[a][0]) && (moves[i][1] == moves[a][1]) && (moves[i][2] == moves[a][2])) {
                        moves[a][0] = 0;
                        moves[a][1] = 0;
                        moves[a][2] = 0;
                    }
                }
            }
            int j = 0;
            for (int i = 1; i < l; i++) {
                if (!((moves[i][0] == 0) && (moves[i][1] == 0) && (moves[i][2] == 0))){
                    j++;
                    temp[j][0] = moves[i][0];
                    temp[j][1] = moves[i][1];
                    temp[j][2] = moves[i][2];
                }
            }
            temp[0][0] = j;
            temp[0][1] = moves[0][1];
            for (int i = 0; i < j + 1; i++) {
                moves[i][0] = temp[i][0];
                moves[i][1] = temp[i][1];
                moves[i][2] = temp[i][2];
            }
        }
    }
}
