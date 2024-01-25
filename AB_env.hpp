#include "abalone.h"
#include "game_env.hpp"

class AB_env : public GameEnv {
private:
	Abalone env;

public:
	AB_env(){};

public:
	void reset() override {
		env.reset();
	}
	void enemy_step(const ActType &step) override {
		int x1 = step.x, y1 = step.y, direction = step.direction;
		int x2 = step.x + dx[step.line_direction] * (step.amount - 1), y2 = step.y + dy[step.line_direction] * (step.amount - 1);
		env.step(x1, y1, x2, y2, direction);
	}
	pair<ActType, bool> self_step() override {
		pair<ActType, bool> result;
		// move mv = AlphaBeta(env).second;  // current move
		move mv = MinMax(env).second;  // current move
		if (mv.x1 == mv.x2 && mv.y1 == mv.y2) result.first = {mv.x1, mv.y1, mv.direction};
		else {
			int amount = max(abs(mv.x2 - mv.x1), abs(mv.y2 - mv.y1));
			for (int line_direction = 0; line_direction < 6; line_direction++) {
				if (mv.x1 + dx[line_direction] * (amount - 1) == mv.x2 && mv.y1 + dy[line_direction] * (amount - 1) == mv.y2) {
					result.first = {mv.x1, mv.y1, line_direction, mv.direction, amount};
				}
			}
		}
		env.step(mv);
		result.second = (env.winner != 0);
		return result;
	}
};