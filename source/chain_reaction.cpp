#include <iostream>
#include <chrono>
#include <thread>
#include "../include/board.h"
#include "../include/player.h"
#include "../include/rules.h"
#include "../include/algorithm.h"

using namespace std;
using namespace std::chrono_literals;

int main(){

    srand(time(NULL));
    int r_win = 0, b_win = 0;
    
    int cnt;
    cout << "input battle times: ";
    cin >> cnt;

    for (int t = 0; t < cnt; t++){
        Board board;
        Player red_player(RED);
        Player blue_player(BLUE);

        bool first_two_step = true;
        int round = 1;
        int index[2];

        while(1){

            //////////// Red Player operations ////////////
            algorithm_A(board, red_player, index);
            // cout << "RED player: " << endl;
            // cin >> index[0] >> index[1];
            board.place_orb(index[0], index[1], &red_player);

            if(rules_violation(red_player)) return 0;

            // board.print_current_board(index[0], index[1], round);
            // cout << "score: " << evaluate(board, red_player) << endl;
            round++;
            
            // this_thread::sleep_for(100ms);
            // system("Pause");

            if(board.win_the_game(red_player) && !first_two_step){
                cout << "R ";
                r_win++;
                break;
            }

            //////////// Blue Player operations ////////////
            algorithm_B(board, blue_player, index);
            board.place_orb(index[0], index[1], &blue_player);
            // cout << index[0] << "  " << index[1] << endl;
            if(rules_violation(blue_player)) return 0;
            
            // board.print_current_board(index[0], index[1], round);
            // cout << "score: " << evaluate(board, blue_player) << endl;
            round++;
            
            // this_thread::sleep_for(100ms);
            // system("Pause");

            if(board.win_the_game(blue_player) && !first_two_step){
                cout << "B ";
                b_win++;
                break;
            }

            first_two_step = false;
        }
        // this_thread::sleep_for(200ms);
    }

    cout << "Red  win: " << r_win << endl;
    cout << "Blue win: " << b_win << endl;

    return 0;
} 