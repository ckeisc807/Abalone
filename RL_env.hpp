#ifndef _RL_ENV_HPP_
#define _RL_ENV_HPP_

#include "QLearning.hpp"
#include "game_env.hpp"

namespace RL {
	class RL_env : public GameEnv {
	private:
		RL::QLearning env;

	public:
		RL_env(double _learning_rate_Qtable, double _learning_rate_NN, double _discount_factor, function<double(double)> _act_func, function<double(double)> _dact_func)
			: env(_learning_rate_Qtable, _learning_rate_NN, _discount_factor, _act_func, _dact_func) {}

	public:
		void reset() override {
			env.reset();
		}
		void enemy_step(const ActType &step) override {
			env.enemy_move(step);
		}
		pair<ActType, bool> self_step() override {
			pair<ActType, bool> result;
			result.first = env.select_move();
			auto [O_point, X_points] = env.points();
			result.second = O_point >= 5 || X_points >= 5;
			return result;
		}
		bool self_win() override {
			auto [O_point, X_points] = env.points();
			return O_point >= 5;
		}
	};
}  // namespace RL

#endif