#include <iostream>
#include <fstream>

#include "state_logic.h"


int piece_list_data[48][11] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{2, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
	{2, 0, 0, 1, -1, 0, 0, 0, 0, 0, 0},
	{2, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
	{2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
	{3, 0, 0, -1, -1, 1, 1, 0, 0, 0, 0},
	{3, 0, 0, 1, -1, -1, 1, 0, 0, 0, 0},
	{3, 0, -1, 0, 0, 0, 1, 0, 0, 0, 0},
	{3, 0, 0, -1, 0, 1, 0, 0, 0, 0, 0},
	{3, 0, 0, -1, 0, 0, 1, 0, 0, 0, 0},
	{3, 0, 0, -1, 0, 0, -1, 0, 0, 0, 0},
	{3, 0, 0, 1, 0, 0, -1, 0, 0, 0, 0},
	{3, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
	{4, 0, -1, 0, 0, 0, 1, 0, 2, 0, 0},
	{4, 0, 0, -1, 0, 1, 0, 2, 0, 0, 0},
	{4, 0, -1, 0, 0, 0, 1, 1, -1, 0, 0},
	{4, 0, -1, 0, 0, 0, 1, -1, 1, 0, 0},
	{4, 0, -1, 0, 0, 0, 1, -1, -1, 0, 0},
	{4, 0, -1, 0, 0, 0, 1, 1, 1, 0, 0},
	{4, 0, -1, 0, 0, 0, 1, -1, 0, 0, 0},
	{4, 0, -1, 0, 0, 0, 1, 1, 0, 0, 0},
	{4, 0, 0, -1, 0, 1, 0, 1, -1, 0, 0},
	{4, 0, 0, -1, 0, 1, 0, -1, 1, 0, 0},
	{4, 0, 0, -1, 0, 1, 0, -1, -1, 0, 0},
	{4, 0, 0, -1, 0, 1, 0, 1, 1, 0, 0},
	{4, 0, 0, -1, 0, 1, 0, 0, 1, 0, 0},
	{4, 0, 0, -1, 0, 1, 0, 0, -1, 0, 0},
	{4, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0},
	{4, 0, 0, 0, 1, 1, 0, 1, -1, 0, 0},
	{4, 0, 0, 1, 0, 0, -1, 1, 1, 0, 0},
	{4, 0, 0, 0, 1, 1, 0, -1, 1, 0, 0},
	{4, 0, 0, 1, 0, 0, -1, -1, -1, 0, 0},
	{5, -2, 0, -1, 0, 0, 0, 1, 0, 2, 0},
	{5, 0, -2, 0, -1, 0, 0, 0, 1, 0, 2},
	{5, 0, -1, -1, 0, 0, 0, 1, 0, 0, 1},
	{5, 1, -1, 0, -1, 0, 0, 0, 1, 1, 1},
	{5, -1, -1, 0, -1, 0, 0, 0, 1, -1, 1},
	{5, -1, -1, -1, 0, 0, 0, 1, 0, 1, -1},
	{5, -1, 1, -1, 0, 0, 0, 1, 0, 1, 1},
	{5, 1, 1, -1, 0, 0, 0, 1, 0, 1, -1},
	{5, -1, 1, -1, 0, 0, 0, 1, 0, -1, -1},
	{5, -1, -1, 0, -1, 0, 0, 0, 1, 1, -1},
	{5, 1, 1, 0, -1, 0, 0, 0, 1, -1, 1},
	{5, -1, 1, 0, 1, 1, 1, 1, 0, 1, -1},
	{5, 1, -1, 0, -1, -1, -1, -1, 0, -1, 1},
	{5, 1, 1, 0, 1, -1, 1, -1, 0, -1, -1},
	{5, -1, -1, 0, -1, 1, -1, 1, 0, 1, 1}
};

Piece piece_list[48] = {0};


void init_piece_list() {
    for (int i = 0; i < 48; i++) {
        piece_list[i].block_count = piece_list_data[i][0];
        piece_list[i].structure[0][0] = piece_list_data[i][1];
        piece_list[i].structure[0][1] = piece_list_data[i][2];
        piece_list[i].structure[1][0] = piece_list_data[i][3];
        piece_list[i].structure[1][1] = piece_list_data[i][4];
        piece_list[i].structure[2][0] = piece_list_data[i][5];
        piece_list[i].structure[2][1] = piece_list_data[i][6];
        piece_list[i].structure[3][0] = piece_list_data[i][7];
        piece_list[i].structure[3][1] = piece_list_data[i][8];
        piece_list[i].structure[4][0] = piece_list_data[i][9];
        piece_list[i].structure[4][1] = piece_list_data[i][10];
    }
}

void init_state(State* state) {
    for(int a = 0; a < 13; a++) {
       for(int b = 0; b < 13; b++) {
            if (b > 1 && b < 11 && a > 1 && a < 11) {
                state->board[a][b] = false;
            } else {
                state->board[a][b] = true;
            }
        }
    }
    state->hand[0] = piece_list[0];
    state->hand[1] = piece_list[0];
    state->hand[2] = piece_list[0];
    state->score = 0;
}

void set_hand(State* state, int a, int b, int c) {
    state->hand[0] = piece_list[a];
    state->hand[1] = piece_list[b];
    state->hand[2] = piece_list[c];
}

void new_hand(State* state) {
    for(int i = 0; i < 3; i++) {
        state->hand[i] = piece_list[1  + random_int() % 47];
    }
}

void generate_move(State* state, Piece* piece, Move_list* moves) {
    int count = 0;
    bool legal;
    for(int a = 2; a < 11; a++) {
        for(int b = 2; b < 11; b++) {
            legal = true;
            for (int s = 0; s < piece->block_count; s++) {

                if (state->board[a + piece->structure[s][0]][b + piece->structure[s][1]]) {
                    legal = false;
                    break;
                }
            }
            if (legal) {
                // add move
                moves->moves[count][0] = a;
                moves->moves[count][1] = b;
                count++;
            }
        }
    }
    moves->move_count = count;
    moves->piece = *piece;
}

void flip_piece(State* state, Piece* piece, int x, int y) {
    for (int s = 0; s < piece->block_count; s++) {
        state->board[x + piece->structure[s][0]][y + piece->structure[s][1]] =
         !state->board[x + piece->structure[s][0]][y + piece->structure[s][1]];
    }
}

const int groups[27][9] = {
	{28, 29, 30, 31, 32, 33, 34, 35, 36},
	{41, 42, 43, 44, 45, 46, 47, 48, 49},
	{54, 55, 56, 57, 58, 59, 60, 61, 62},
	{67, 68, 69, 70, 71, 72, 73, 74, 75},
	{80, 81, 82, 83, 84, 85, 86, 87, 88},
	{93, 94, 95, 96, 97, 98, 99, 100, 101},
	{106, 107, 108, 109, 110, 111, 112, 113, 114},
	{119, 120, 121, 122, 123, 124, 125, 126, 127},
	{132, 133, 134, 135, 136, 137, 138, 139, 140},
	{28, 41, 54, 67, 80, 93, 106, 119, 132},
	{29, 42, 55, 68, 81, 94, 107, 120, 133},
	{30, 43, 56, 69, 82, 95, 108, 121, 134},
	{31, 44, 57, 70, 83, 96, 109, 122, 135},
	{32, 45, 58, 71, 84, 97, 110, 123, 136},
	{33, 46, 59, 72, 85, 98, 111, 124, 137},
	{34, 47, 60, 73, 86, 99, 112, 125, 138},
	{35, 48, 61, 74, 87, 100, 113, 126, 139},
	{36, 49, 62, 75, 88, 101, 114, 127, 140},
	{28, 29, 30, 41, 42, 43, 54, 55, 56},
	{31, 32, 33, 44, 45, 46, 57, 58, 59},
	{34, 35, 36, 47, 48, 49, 60, 61, 62},
	{67, 68, 69, 80, 81, 82, 93, 94, 95},
	{70, 71, 72, 83, 84, 85, 96, 97, 98},
	{73, 74, 75, 86, 87, 88, 99, 100, 101},
	{106, 107, 108, 119, 120, 121, 132, 133, 134},
	{109, 110, 111, 122, 123, 124, 135, 136, 137},
	{112, 113, 114, 125, 126, 127, 138, 139, 140}
};

void clear_lines(State* state) {
    bool clear[27];
    for(int i = 0; i < 27; i++) {
        clear[i] = true;
        for(int j = 0; j < 9; j++) {
            if(!state->board[groups[i][j] / 13][groups[i][j] % 13]) {
                clear[i] = false;
                break;
            }
        }
    }

    for(int i = 0; i < 27; i++) {
        if (clear[i]) {
            state->score += 16;
            for(int j = 0; j < 9; j++) {
                state->board[groups[i][j] / 13][groups[i][j] % 13] = false;
            }
        }
    }
}

int possible_moves(State* state) {
    int total = 0;
    Move_list moves;
    for(int i = 1; i < 48; i++) {
        generate_move(state, &piece_list[i], &moves);
        total += moves.move_count;
    }
    return total;
}

void show_state(State* state) {
    std::cout << "+---+---+---+---+---+---+---+---+---+\n";
    for(int a = 2; a < 11; a++) {
        std::cout << "|";
        for(int b = 2; b < 11; b++) {
            if (state->board[a][b]) {
                std::cout << " X |";
            } else {
                std::cout << "   |";
            }
        }
        std::cout << "\n+---+---+---+---+---+---+---+---+---+\n";
    }

    std::cout << "\n+----------+-----------+-----------+\n";
    for(int a = 0; a < 5; a++) {
        std::cout << "|";
        for(int b = 0; b < 17; b++) {
            bool occupied = false;
            for (int piece = 0; piece < 3; piece++) {
                for(int square = 0; square < state->hand[piece].block_count; square++) {
                    if (state->hand[piece].structure[square][0] + 2 == a) {
                        if (state->hand[piece].structure[square][1] + 2 + 6 * piece == b) {
                            occupied = true;
                        }
                    }
                }
            }
        if (occupied) {
            std::cout << "X ";
        } else if (b == 5 || b == 11) {
            std::cout << "| ";
        } else {
            std::cout << "- ";
        }

        }
        std::cout << "|\n";
    }
    std::cout << "+----------+-----------+-----------+\n";
}

/** Random function **/
int random_state = 1804289387;

void set_random() {
    random_state = get_time();
}

int random_int() {
    int number = random_state;
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    random_state = number;
    return std::abs(number);
}

void sleep(int ms) {
    int start = get_time();
    int end   = get_time();
    while(end - start <= ms){
        end = get_time();
    }
}

int get_time() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
