#include <iostream>

#include "state_logic.h"

void best_move(State*, int, bool, bool, bool);


int best_path [3][3];
int current_path [3][3];
int best_value;
int options;

void copy_data(State* from, State* to);
void python_protocol ();

int main()
{
    set_random();
    init_piece_list();

    while (true) {
        python_protocol();
    }
    return 0;
}

void python_protocol () {
    State board;
    init_state(&board);
    int piece_1;
    int piece_2;
    int piece_3;

    std::cin >> piece_1;
    std::cin >> piece_2;
    std::cin >> piece_3;
    set_hand(&board, piece_1, piece_2, piece_3);

    for(int a = 2; a < 11; a++) {
        for(int b = 2; b < 11; b++) {
            std::cin >> board.board[a][b];
        }
    }

    show_state(&board);

    best_move(&board, 0, true, true, true);

    if (best_value == 0) {
        std::cout << "Cannot avoid loss.\n";
        return;
    }

    for(int i = 0; i < 3; i++) {
            flip_piece(&board, &board.hand[best_path[i][0]], best_path[i][1], best_path[i][2]);
            board.score += board.hand[best_path[i][0]].block_count;
            clear_lines(&board);
            show_state(&board);
        }

    std::cout << best_path[0][0] << " " << best_path[0][1] << " " << best_path[0][2] << " ";
    std::cout << best_path[1][0] << " " << best_path[1][1] << " " << best_path[1][2] << " ";
    std::cout << best_path[2][0] << " " << best_path[2][1] << " " << best_path[2][2] << "\n";



    std::cout << "Engine done.\n";
}

void copy_data(State* from, State* to) {
    for(int a = 0; a < 13; a++) {
        for(int b = 0; b < 13; b++) {
            to->board[a][b] = from->board[a][b];
        }
    }
    to->score = from->score;
    to->hand[0] = from->hand[0];
    to->hand[1] = from->hand[1];
    to->hand[2] = from->hand[2];
}


void best_move(State* state, int depth, bool p_1, bool p_2, bool p_3) {
    if (depth == 0) {
        best_value = 0;
        options = 0;
    }
    // We do not care about positions with nearly no pieces
    if (best_value > 1500) {
        return;
    }

    if (depth == 3) {
        options++;
        int value = possible_moves(state);
        if (value > best_value) {
            best_value = value;
            best_path[0][0] = current_path[0][0];
            best_path[0][1] = current_path[0][1];
            best_path[0][2] = current_path[0][2];
            best_path[1][0] = current_path[1][0];
            best_path[1][1] = current_path[1][1];
            best_path[1][2] = current_path[1][2];
            best_path[2][0] = current_path[2][0];
            best_path[2][1] = current_path[2][1];
            best_path[2][2] = current_path[2][2];
        }
        return;
    }

    Move_list moves;
    State next_state;

    if (p_1) {
        generate_move(state, &state->hand[0], &moves);
        for(int i = 0; i < moves.move_count; i++) {
            current_path[depth][0] = 0;
            current_path[depth][1] = moves.moves[i][0];
            current_path[depth][2] = moves.moves[i][1];
            flip_piece(state, &state->hand[0], moves.moves[i][0], moves.moves[i][1]);

            copy_data(state, &next_state);
            clear_lines(&next_state);

            best_move(&next_state, depth + 1, false, p_2, p_3);

            flip_piece(state, &state->hand[0], moves.moves[i][0], moves.moves[i][1]);
        }
    }
    if (p_2) {
        generate_move(state, &state->hand[1], &moves);
        for(int i = 0; i < moves.move_count; i++) {
            current_path[depth][0] = 1;
            current_path[depth][1] = moves.moves[i][0];
            current_path[depth][2] = moves.moves[i][1];
            flip_piece(state, &state->hand[1], moves.moves[i][0], moves.moves[i][1]);

            copy_data(state, &next_state);
            clear_lines(&next_state);

            best_move(&next_state, depth + 1, p_1, false, p_3);

            flip_piece(state, &state->hand[1], moves.moves[i][0], moves.moves[i][1]);
        }
    }
    if (p_3) {
        generate_move(state, &state->hand[2], &moves);
        for(int i = 0; i < moves.move_count; i++) {
            current_path[depth][0] = 2;
            current_path[depth][1] = moves.moves[i][0];
            current_path[depth][2] = moves.moves[i][1];
            flip_piece(state, &state->hand[2], moves.moves[i][0], moves.moves[i][1]);

            copy_data(state, &next_state);
            clear_lines(&next_state);

            best_move(&next_state, depth + 1, p_1, p_2, false);

            flip_piece(state, &state->hand[2], moves.moves[i][0], moves.moves[i][1]);
        }
    }
}
