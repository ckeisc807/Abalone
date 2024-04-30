#ifndef POS_NORAND
#define POS_NORAND
#include <random>

#include "../EvaluationBase.h"

struct pos_norand : Evaluation {
	const EvaluationType weight[2] = {1e6, 1e2};
	const EvaluationType pos_value[5] = {0.5, 0.4, 0.3, 0.2, 0.1};
	const EvaluationType current_weight = 1;
	const EvaluationType opponent_weight = -1;

	EvaluationType evaluate(Abalone abalone) override {
		int current_player = abalone.current_player, opponent_player = (current_player == 1) ? 2 : 1;
		EvaluationType delta_score = abalone.score[current_player] - abalone.score[opponent_player];
		if (delta_score == abalone.score[0]) return weight[0];
		else if (delta_score == -abalone.score[0]) return -weight[0];
		EvaluationType value, tmp_value;
		int tmp;
		value = weight[1] * (abalone.score[current_player] - abalone.score[opponent_player]);
		for (int y = 0; y < 9; y++) {
			for (int x = 0; x < 9; x++) {
				if (abalone.board[x][y] == -1) continue;
				tmp = max(abs(x - 4), max(abs(y - 4), abs(abs(x - y) - 4)));
				tmp_value = pos_value[tmp];
				if (abalone.board[x][y] == current_player) value += tmp_value * current_weight;
				else if (abalone.board[x][y] == opponent_player) value -= tmp_value * opponent_weight;
			}
		}
		return value;
	}
};

#endif