#ifndef _AB_ENV_HPP_
#define _AB_ENV_HPP_

#include <iostream>
#include <utility>

#include "../game_act_type.hpp"
#include "../game_env.hpp"
#include "AB_abalone.hpp"
#include "AB_pruning.hpp"

using std::pair;

namespace AB {
	class AB_env : public GameEnv {
	private:
		AB::AB_abalone game;

	public:
		AB_env() {}

		virtual void reset() override {
			game.reset();
		}

		virtual void enemy_step(const ActType& act) override {
			game.move(act, 'X');
			game.point_count();
		}

		virtual pair<ActType, bool> self_step() override {
			vector<ActType>&& acts = game.legal_moves('O');
			ActType best_act = min_max(game).first;

			game.move(best_act, 'O');
			game.point_count();

			auto [O_point, X_point] = game.points();

			return {best_act, O_point >= 5 || X_point >= 5};
		}

		virtual bool self_win() override {
			auto [O_point, X_point] = game.points();
			std::cout << O_point << " " << X_point << "\n";
			std::cout << game << "\n";
			return O_point >= 5 || O_point > X_point;
		}
	};
}  // namespace AB

#endif