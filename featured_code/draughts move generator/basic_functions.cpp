#include <iostream>
#include <chrono>
#include "basic_functions.h"

bool noThreat(U64* pos){
    U64 free = aliveSquares & (~(pos[0] | pos[2]));
    U64 opponent = pos[0];
    U64 our_men = pos[2];

    U64 way_1  = (((our_men << 5) & opponent) >> 5) & (((our_men << 10) & free) >> 10); // +5 didrection NE
    way_1 |= (((our_men << 6) & opponent) >> 6) & (((our_men << 12) & free) >> 12); // +6 didrection NW
    way_1 |= (((our_men >> 5) & opponent) << 5) & (((our_men >> 10) & free) << 10); // -5 didrection SW
    way_1 |= (((our_men >> 6) & opponent) << 6) & (((our_men >> 12) & free) << 12); // -6 didrection SE
    way_1 |= (((opponent << 5) & our_men) >> 5) & (((opponent << 10) & free) >> 10); // +5 didrection NE
    way_1 |= (((opponent << 6) & our_men) >> 6) & (((opponent << 12) & free) >> 12); // +6 didrection NW
    way_1 |= (((opponent >> 5) & our_men) << 5) & (((opponent >> 10) & free) << 10); // -5 didrection SW
    way_1 |= (((opponent >> 6) & our_men) << 6) & (((opponent >> 12) & free) << 12); // -6 didrection SE
    if(way_1 == 0){
        return true;
    }
    return false;
}

int random_state = 1804289387;

void setRandom ()
{
    random_state = getTime();
}

int random_int ()
{
    int number = random_state;
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    random_state = number;
    return std::abs(number);
}

U64 accurateTime ()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void sleep (int ms)
{
    int start = getTime();
    int end   = getTime();
    while(end - start <= ms){
        end = getTime();
    }
}

int getTime ()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

U64 get_msb(U64 bits)
{
    bits |= bits >> 1;
    bits |= bits >> 2;
    bits |= bits >> 4;
    bits |= bits >> 8;
    bits |= bits >> 16;
    bits |= bits >> 32;
    return (bits & ~(bits >> 1));
}


void invert(U64* pos)
{
    U64 white = pos[0];
    U64 black = pos[2];
    U64 kings = pos[1];

    const U64 h1 = (0x5555555555555555);
    const U64 h2 = (0x3333333333333333);
    const U64 h4 = (0x0F0F0F0F0F0F0F0F);
    const U64 v1 = (0x00FF00FF00FF00FF);
    const U64 v2 = (0x0000FFFF0000FFFF);
    black = ((black >>  1) & h1) | ((black & h1) <<  1);
    black = ((black >>  2) & h2) | ((black & h2) <<  2);
    black = ((black >>  4) & h4) | ((black & h4) <<  4);
    black = ((black >>  8) & v1) | ((black & v1) <<  8);
    black = ((black >> 16) & v2) | ((black & v2) << 16);
    black = ( black >> 32)       | ( black       << 32);
    black = black >> 10;

    white = ((white >>  1) & h1) | ((white & h1) <<  1);
    white = ((white >>  2) & h2) | ((white & h2) <<  2);
    white = ((white >>  4) & h4) | ((white & h4) <<  4);
    white = ((white >>  8) & v1) | ((white & v1) <<  8);
    white = ((white >> 16) & v2) | ((white & v2) << 16);
    white = ( white >> 32)       | ( white       << 32);
    white = white >> 10;

    kings = ((kings >>  1) & h1) | ((kings & h1) <<  1);
    kings = ((kings >>  2) & h2) | ((kings & h2) <<  2);
    kings = ((kings >>  4) & h4) | ((kings & h4) <<  4);
    kings = ((kings >>  8) & v1) | ((kings & v1) <<  8);
    kings = ((kings >> 16) & v2) | ((kings & v2) << 16);
    kings = ( kings >> 32)       | ( kings       << 32);
    kings = kings >> 10;

    pos[0] = black;
    pos[1] = kings;
    pos[2] = white;
    //board(pos);
}



void printBitboard(U64 pos){
    int k = 0;
    U64 occupied = pos | 0b1111111111000000000010000000000100000000001000000000010000000000L;
    std::cout<<"+---+---+---+---+---+---+---+---+---+---+\n"<<"|   ";
    for(int i = 55; i>-1;i--){
        if((occupied & (1LL<<(i))) == 0)std::cout<<"|   |   ";
        if((pos & (1LL<<(i))) != 0)std::cout<<"| X |   ";
        if(i % 11 == 5){ std::cout<<"\n+---+---+---+---+---+---+---+---+---+---+\n"; k++; }
        if((i % 11 == 0) && (i != 55)){ std::cout<<"|"<<"\n+---+---+---+---+---+---+---+---+---+---+\n"; k++; if(i!=0) std::cout<<"|   ";}
    }
    std::cout<<"\n";
}

void board(U64* pos){
    std::cout<<"{"<<pos[0]<<"ULL, "<<pos[1]<<"ULL, "<<pos[2]<<"ULL};\n";
    const char *squares[10] =  {"      |   | 1 |   | 2 |   | 3 |   | 4 |   | 5 |",
                             "         | 6 |   | 7 |   | 8 |   | 9 |   |10 |   |",
                                "      |   |11 |   |12 |   |13 |   |14 |   |15 |",
                             "         |16 |   |17 |   |18 |   |19 |   |20 |   |",
                                "      |   |21 |   |22 |   |23 |   |24 |   |25 |",
                             "         |26 |   |27 |   |28 |   |29 |   |30 |   |",
                                "      |   |31 |   |32 |   |33 |   |34 |   |35 |",
                             "         |36 |   |37 |   |38 |   |39 |   |40 |   |",
                                "      |   |41 |   |42 |   |43 |   |44 |   |45 |",
                            "         |46 |   |47 |   |48 |   |49 |   |50 |   |"};
    int k = 0;
    U64 occupied = pos[0] | pos[2] | 0b1111111111000000000010000000000100000000001000000000010000000000L;
    U64 bm = pos[0] & ~(pos[1]);
    U64 bk = pos[0] & pos[1];
    U64 wm = pos[2] & ~(pos[1]);
    U64 wk = pos[2] & pos[1];
    std::cout<<"+---+---+---+---+---+---+---+---+---+---+         +---+---+---+---+---+---+---+---+---+---+\n"<<"|   ";
    for(int i = 55; i>-1;i--){
        if((occupied & (1LL<<(i))) == 0)std::cout<<"|   |   ";
        if((bm & (1LL<<(i))) != 0)std::cout<<"| b |   ";
        if((bk & (1LL<<(i))) != 0)std::cout<<"| B |   ";
        if((wm & (1LL<<(i))) != 0)std::cout<<"| w |   ";
        if((wk & (1LL<<(i))) != 0)std::cout<<"| W |   ";
        if(i % 11 == 5){
            std::cout<<squares[k]<<"\n+---+---+---+---+---+---+---+---+---+---+         +---+---+---+---+---+---+---+---+---+---+\n";
            k++;
        }
        if((i % 11 == 0) && (i != 55)){
            std::cout<<"|"<<squares[k]<<"\n+---+---+---+---+---+---+---+---+---+---+         +---+---+---+---+---+---+---+---+---+---+\n";
            k++;
            if(i!=0) std::cout<<"|   ";
        }
    }
    std::cout<<"\n";
}

bool quiet(U64* pos, int color){
    U64 free = aliveSquares & (~(pos[0] | pos[2]));
    U64 opponent = pos[1-color];
    U64 our_men = pos[1+color];

    U64 ways = (((our_men << 5) & opponent) >> 5) & (((our_men << 10) & free) >> 10); // +5 didrection NE
    ways |= (((our_men << 6) & opponent) >> 6) & (((our_men << 12) & free) >> 12); // +6 didrection NW
    ways |= (((our_men >> 5) & opponent) << 5) & (((our_men >> 10) & free) << 10); // -5 didrection SW
    ways |= (((our_men >> 6) & opponent) << 6) & (((our_men >> 12) & free) << 12); // -6 didrection SE
    if (ways == 0) {
        return true;
    }
    return false;

}


void printMove(U64* old_pos, U64* new_pos, int color){
    int from = BSF(old_pos[1 + color] & (old_pos[1 + color] ^ new_pos[1 + color]));
    int to = BSF((~old_pos[1 + color]) & (old_pos[1 + color] ^ new_pos[1 + color]));
    std::cout<<(int) (550-10 * from) / 11;
    if ((old_pos[1 - color] ^ new_pos[1-color]) == 0) std::cout << "-";
    else std::cout << "x";
    std::cout << (int) (550 - 10 * to) / 11 << " ";
}
