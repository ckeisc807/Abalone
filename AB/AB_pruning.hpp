#ifndef _AB_PRUNING_HPP_
#define _AB_PRUNING_HPP_

#include <utility>
#include <vector>

#include "../game_act_type.hpp"
#include "AB_abalone.hpp"

using std::pair;
using std::vector;

static const int MAX_DEPTH = 3;

namespace AB {
	pair<ActType, int> min_max(const AB::AB_abalone &board, int alpha = -INF, int beta = INF, int depth = 1, const char SELF = 'O') {
		const auto [O_point, X_point] = board.points();
		if (O_point >= 5) return {ActType(), INF};
		if (X_point >= 5) return {ActType(), -INF};
		if (depth == MAX_DEPTH) return {ActType(), board.evaluate()};

		const char OPS = (SELF == 'O') ? 'X' : 'O';

		vector<ActType> &&moves = board.legal_moves(SELF);
		ActType best_act;

		for (const ActType &act : moves) {
			AB::AB_abalone next_board = board;
			next_board.move(act, SELF);
			next_board.point_count();

			auto [O_point, X_point] = board.points();
			auto [nxt_O_point, nxt_X_point] = next_board.points();
			if (nxt_O_point - nxt_X_point < O_point - X_point) continue;

			int score = -min_max(next_board, -beta, -alpha, depth + 1, OPS).second;
			if (score > alpha) alpha = score, best_act = act;
			if (score >= beta) return {ActType(), beta};
		}

		return {best_act, alpha};
	};
};	// namespace AB

#endif