#ifndef _RL_ENV_HPP_
#define _RL_ENV_HPP_

#include "../game_env.hpp"
#include "QLearning.hpp"

namespace RL {
	class RLEnv : public GameEnv {
	private:
		RL::QLearning env;

	public:
		RLEnv(double _learning_rate_Qtable, double _learning_rate_NN, double _discount_factor, function<double(double)> _act_func, function<double(double)> _dact_func)
			: env(_learning_rate_Qtable, _learning_rate_NN, _discount_factor, _act_func, _dact_func) {}

	public:
		virtual void reset() override {
			env.reset();
		}

		virtual void enemy_step(const ActType &step) override {
			env.enemy_move(step);
		}

		virtual pair<ActType, bool> self_step() override {
			pair<ActType, bool> result;
			ActType act = env.select_move('O');
			env.self_move(act);
			result.first = act;
			auto [O_point, X_points] = env.points();
			result.second = O_point >= 5 || X_points >= 5;
			return result;
		}

		virtual bool self_win() override {
			auto [O_point, X_point] = env.points();
			return O_point >= 5 || O_point > X_point;
		}
	};
}  // namespace RL

#endif