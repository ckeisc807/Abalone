#ifndef TEST1
#define TEST1
#include"../EvaluationBase.h"
struct test1: Evaluation {
    float weight[2] = {10,1};
    float pos_value[5] = {1000,0.4,0.3,0.2,0.1};
    float evaluate (Abalone abalone) override {
        int current_player = abalone.current_player, opponent_player = (current_player == 1) ? 2 : 1;
        int delta_score = abalone.score[current_player] - abalone.score[opponent_player];
        if (delta_score == abalone.score[0]) return weight[0];
        else if (delta_score == -abalone.score[0]) return -weight[0];
        int value, tmp_value, tmp;
        value = weight[1] * (abalone.score[current_player] - abalone.score[opponent_player]);
        for (int y = 0; y < 9; y++) {
            for (int x = 0; x < 9; x++) {
                if (abalone.board[x][y] == -1) continue;
                tmp = max(abs(x - 4), max(abs(y - 4), abs(abs(x - y) - 4)));
                tmp_value = pos_value[tmp];
                if (abalone.board[x][y] == current_player) value += tmp_value;
                else if (abalone.board[x][y] == opponent_player) value -= tmp_value;
            }
        }
        return value;
    }
};

#endif