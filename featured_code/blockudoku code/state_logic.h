#ifndef STATE_LOGIC_H_INCLUDED
#define STATE_LOGIC_H_INCLUDED
#include <chrono>



void set_random             ();
int  random_int             ();
void sleep                  (int);
int  get_time               ();

struct Piece {
    short block_count;
    short structure[5][2];
};

struct State {
    bool board[13][13];
    Piece hand[3];
    int score;
};

struct Move_list {
    int move_count;
    Piece piece;
    int moves[81][2];
};

void init_state             (State*);
void init_piece_list        ();
void show_state             (State*);
void generate_move          (State*, Piece*, Move_list*);
void flip_piece             (State*, Piece*, int, int);

void set_hand               (State*, int, int, int);
void new_hand               (State*);
void clear_lines            (State*);
int  possible_moves         (State*);
#define transfer(from, to) (memcpy(&to, &from, sizeof(from)))



#endif // STATE_LOGIC_H_INCLUDED
