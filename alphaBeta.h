#ifndef ALPHA_BETA
#define ALPHA_BETA
#define EvaluationType float

#include <assert.h>

#include<utility>
#include <vector>
#include <functional>

#include "EvaluationBase.h"
#include "abalone.h"

using std::max, std::min;
using std::swap;
using std::vector, std::pair;
using std::function;

const int inf=1e9;
const move empty_move;

pair<EvaluationType, move> AlphaBeta(Abalone &abalone, Evaluation &evaluate, int depth = 0, int depth_limit = 3, EvaluationType alpha = -inf, EvaluationType beta = inf) {
	int current_player = abalone.current_player, opponent_player = (current_player == 1) ? 2 : 1;
	if (abalone.winner || depth == depth_limit) return {evaluate.evaluate(abalone), empty_move};
	float best_score = -inf, score;
	move best_move = empty_move;
	Abalone temp;
	for (const move &mv : abalone.get_moves()) {
		temp = abalone;
		temp.step(mv);
		score = -AlphaBeta(temp, evaluate, depth + 1, depth_limit, -beta, -alpha).first;
		if (score >= beta) return {beta, empty_move};
		if (score > alpha) alpha = score, best_move = mv;
	}
	return {alpha, best_move};
}

#endif
