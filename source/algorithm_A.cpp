#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cassert>
#include "../include/algorithm.h"

using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/

struct Point {
    int x, y;
    int score;
    bool is_null;

    Point(int _x = 0, int _y = 0): x(_x), y(_y), score(0), is_null(true) {}
};

int evaluate(Board &board, char pl_color);
bool win_game(Board &board, char pl_color);
Point negamax(Board board, int ply, Player &player, Player &opponent);
Point alpha_beta(Board board, int ply, Player &player, Player &opponent,
                 int alpha, int beta);


void algorithm_A(Board board, Player player, int index[]){
    const int inf = 9999999;

    char pl_color = player.get_color();
    char op_color = (pl_color == RED ? BLUE : RED);
    
    Player opponent(op_color);

    Point place_idx = alpha_beta(board, 5, player, opponent, -inf, inf);
    // assert(!place_idx.is_null);
    // Point place_idx = negamax(board, 3, player, opponent);

    index[0] = place_idx.x;
    index[1] = place_idx.y;
}

Point negamax(Board board, int ply, Player &player, Player &opponent){
    Point best;
    char pl_color = player.get_color();
    char op_color = opponent.get_color();

    if (ply == 0 || win_game(board, op_color)){
        best.score = evaluate(board, pl_color);
        return best;
    }

    bool no_more_move = true;
    for (int i = 0; i < ROW; i++){
        for (int j = 0; j < COL; j++){
            if (board.get_cell_color(i, j) == op_color){
                continue;
            }
            no_more_move = false;
            
            Board next_board = board;
            assert(next_board.place_orb(i, j, &player));
            Point result = negamax(next_board, ply - 1, opponent, player);

            if (best.is_null || -result.score > best.score) {
                best.is_null = false;
                best.x = i; best.y = j;
                best.score = -result.score;
            }
        }
    }

    if (no_more_move){
        best.score = evaluate(board, pl_color);
        return best;
    }

    return best;
}

Point alpha_beta(Board board, int ply, Player &player, Player &opponent,
                 int alpha, int beta){
    Point best;
    char pl_color = player.get_color();
    char op_color = opponent.get_color();

    if (ply == 0 || win_game(board, op_color)){
        best.score = evaluate(board, pl_color);
        return best;
    }

    bool no_more_move = true;
    for (int k = 3; k >= 0; k--){
        for (int i = 0; i < ROW; i++){
            for (int j = 0; j < COL; j++){
                if (board.get_cell_color(i, j) == op_color || 
                    board.get_orbs_num(i, j) != k){
                    continue;
                }
                no_more_move = false;
                
                Board next_board = board;
                next_board.place_orb(i, j, &player);
                Point result = alpha_beta(next_board, ply - 1, opponent, player, -beta, -alpha);
                
                if (best.is_null || -result.score > alpha) {
                    best.is_null = false;
                    alpha = -result.score;
                    best.x = i; best.y = j;
                    best.score = alpha;
                }
                if (alpha >= beta){
                    return best;
                }
            }
        }
    }

    if (no_more_move){
        best.score = evaluate(board, pl_color);
        return best;
    }

    return best;
}

bool win_game(Board &board, char pl_color){
    int cnt = 0;
    for (int i = 0; i < ROW; i++){
        for (int j = 0; j < COL; j++){
            char c = board.get_cell_color(i, j);
            if (c != 'w' && c != pl_color){
                return false;
            }
            if (c == pl_color){
                cnt += board.get_orbs_num(i, j);
            }
        }
    }
    
    return cnt > 1;
}

int evaluate(Board &board, char pl_color){
    const int inf = 9999999;

    int score = 0;
    bool all_player = true, all_opponent = true;

    for (int i = 0; i < ROW; i++){
        for (int j = 0; j < COL; j++){
            char c = board.get_cell_color(i, j);
            if (c == pl_color){
                all_opponent = false;
                score++;
            }
            else if (c != 'w') {
                all_player = false;
                score--;
            }
        }
    }

    if (all_opponent){
        return -inf;
    }
    else if (all_player) {
        return inf;
    }

    return score;
}