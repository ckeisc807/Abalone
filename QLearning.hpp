#ifndef _QLEARNING_HPP_
#define _QLEARNING_HPP_

#include <functional>
#include <vector>
#define RANDOM_MODE
#include "NN.hpp"
#include "game_act_type.hpp"
#include "game_class.hpp"

using std::function;
using std::vector;
namespace RL {
	class QLearning {
		const double discount_factor;
		const double learning_rate_Qtable;
		const double time_reward = -0.01;
		inline static const double INF = 1e9;
		inline static const vector<int> NN_topo = {61, 80, 80, 80, 80, 1};
		NN::NeuroNetwork Qtable;
		RL::Abalone game;

	public:
		QLearning() = delete;
		QLearning(double _learning_rate_Qtable, double _learning_rate_NN, double _discount_factor, function<double(double)> _act_func, function<double(double)> _dact_func)
			: discount_factor(_discount_factor), Qtable(NN_topo, _act_func, _dact_func, _learning_rate_NN), learning_rate_Qtable(_learning_rate_Qtable) {}

		~QLearning() {
			NN::write(Qtable, "Qtables", "20240105", "Sigmoid", "dSigmoid");
		}

	public:
		void learn() {
			auto [O_point, X_point] = game.points();

			vector<double> cur_board = game.transform_to_vector();
			double pre_val = Qtable.predict(cur_board)[0];

			if (O_point >= 5) Qtable.train(cur_board, {INF});
			if (X_point >= 5) Qtable.train(cur_board, {-INF});

			vector<ActType> &&acts = game.legal_moves('O');
			double max_val = -INF;
			for (const ActType &act : acts) {
				RL::Abalone nxt_game = game;
				nxt_game.move(act, 'O');
				vector<double> &&nxt_board = nxt_game.transform_to_vector();

				double nxt_val = Qtable.predict(nxt_board)[0];
				if (nxt_val > max_val) max_val = nxt_val;
			}

			Qtable.train(cur_board, {(1 - learning_rate_Qtable) * pre_val + learning_rate_Qtable * (time_reward + discount_factor * max_val)});
		}

	public:
		ActType select_move() {
			double max_val = -INF;
			ActType max_act;
			vector<ActType> acts = game.legal_moves('O');

			for (const ActType &act : acts) {
				RL::Abalone nxt_game = game;
				nxt_game.move(act, 'O');
				vector<double> &&nxt_board = nxt_game.transform_to_vector();

				double nxt_val = Qtable.predict(nxt_board)[0];
				if (nxt_val > max_val) max_val = nxt_val, max_act = act;
			}

			return max_act;
		}

	public:
		void enemy_move(const ActType &act) {
			game.move(act, 'X');
			game.point_count();
		}

	public:
		void self_move(const ActType &act) {
			game.move(act, 'O');
			game.point_count();
		}

	public:
		void reset() {
			game.reset();
		}

	public:
		pair<int, int> points() {
			return game.points();
		}
	};

}  // namespace RL

#endif