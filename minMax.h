#ifndef MIN_MAX
#define MIN_MAX

#include <assert.h>

#include <functional>
#include <utility>
#include <vector>

#include "EvaluationBase.h"
#include "abalone.h"

using std::function;
using std::max, std::min;
using std::swap;
using std::vector, std::pair;

#ifndef EMPTY_MOVE
#define EMPTY_MOVE
const move empty_move;
#endif

pair<EvaluationType, move> MinMax(Abalone &abalone, Evaluation &evaluate, int depth_limit = 3, int depth = 0) {
	int current_player = abalone.current_player, opponent_player = (current_player == 1) ? 2 : 1;
	if (abalone.winner || depth >= depth_limit) return {evaluate.evaluate(abalone), empty_move};
	EvaluationType best_score = -inf, score;
	move best_move = empty_move;
	Abalone temp;
	for (const move &mv : abalone.get_moves()) {
		temp = abalone;
		temp.step(mv);
		score = -MinMax(temp, evaluate, depth_limit, depth + 1).first;
		if (score > best_score) best_score = score, best_move = mv;
	}
	return {best_score, best_move};
}

#endif
