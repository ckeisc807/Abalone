#ifndef _QLEARNING_HPP_
#define _QLEARNING_HPP_

#include <functional>
#include <future>
#include <iostream>
#include <stack>
#include <utility>
#include <vector>

#include "../ThreadPool/thread_pool.hpp"
#include "../game_act_type.hpp"
#include "NN.hpp"
#include "RL_abalone.hpp"

using std::function;
using std::future;
using std::max;
using std::min;
using std::pair;
using std::stack;
using std::vector;

namespace RL {
	class QLearning {
		const double discount_factor;
		const double learning_rate_Qtable;
		const double time_reward = -0.1;
		inline static const double INF = 1;
		inline static const vector<int> NN_topo = {61, 80, 80, 80, 80, 1};
		NN::NeuroNetwork Qtable;
		RL::RL_Abalone game;
		stack<pair<RL_Abalone, char>> move_record;
		ThreadPool thread_pool;

	public:
		QLearning() = delete;
		QLearning(double _learning_rate_Qtable, double _learning_rate_NN, double _discount_factor, function<double(double)> _act_func, function<double(double)> _dact_func)
			: discount_factor(_discount_factor), Qtable(NN_topo, _act_func, _dact_func, _learning_rate_NN), learning_rate_Qtable(_learning_rate_Qtable), thread_pool(4) {}

		~QLearning() {
			thread_pool.join();
		}

	public:
		void train_board(const pair<RL::RL_Abalone, char> &move) {
			auto [cur_board, OX] = move;
			auto [O_point, X_point] = cur_board.points();
			if (O_point >= 5) Qtable.train(cur_board.transform_to_vector(), {INF});
			else if (X_point >= 5) Qtable.train(cur_board.transform_to_vector(), {-INF});
			else {
				char OPS = (OX == 'O') ? 'X' : 'O';
				vector<ActType> &&legal_moves = cur_board.legal_moves(OPS);
				auto ext = [OPS](double a, double b) {
					return OPS == 'O' ? max(a, b) : min(a, b);
				};
				double ext_val = (OPS == 'O') ? -INF : INF;

				for (const ActType &act : legal_moves) {
					RL::RL_Abalone nxt_board = cur_board;
					nxt_board.move(act, OPS);
					vector<double> &&nxt_board_vec = nxt_board.transform_to_vector();

					double nxt_val = Qtable.predict(nxt_board_vec)[0];
					ext_val = ext(ext_val, nxt_val);
				}

				int step = move_record.size();
				vector<double> &&cur_board_vec = cur_board.transform_to_vector();
				double pre_val = Qtable.predict(cur_board_vec)[0];
				double lr = learning_rate_Qtable, df = discount_factor;
				Qtable.train(cur_board_vec, {(1 - lr) * pre_val + lr * (time_reward * step + df * ext_val)});
			}
		}

	public:
		ActType select_move(char OX) {
			const RL_Abalone &cur_board = game;
			vector<ActType> &&acts = cur_board.legal_moves(OX);
			vector<future<vector<double>>> result(acts.size());
			std::cout << "received\n";
			std::cout << "wait to process: " << acts.size() << "\n";

			for (int i = 0; i < acts.size(); i++) {
				std::cout << i << "\n";
				const ActType &act = acts[i];
				RL::RL_Abalone nxt_board = cur_board;
				nxt_board.move(act, OX);
				vector<double> &&nxt_board_vec = nxt_board.transform_to_vector();

				result[i] = thread_pool.submit([&, nxt_board_vec]() { return Qtable.predict(nxt_board_vec); });
			}

			std::cout << "submitted\n";

			double max_val = -INF;
			ActType max_act;
			auto compare = [OX](double a, double b) {
				if (OX == 'O') return a > b;
				else return a < b;
			};
			for (int i = 0; i < acts.size(); i++) {
				std::cout << "waiting to get: " << i << "\n";
				double nxt_val = result[i].get()[0];
				std::cout << "get: " << nxt_val << "\n";
				const ActType &act = acts[i];
				if (compare(nxt_val, max_val)) max_val = nxt_val, max_act = act;
			}

			std::cout << "selected\n";
			return max_act;
		}

	public:
		void enemy_move(const ActType &act) {
			game.move(act, 'X');
			game.point_count();
			move_record.push({game, 'X'});
		}

	public:
		void self_move(const ActType &act) {
			game.move(act, 'O');
			game.point_count();
			move_record.push({game, 'O'});
		}

	public:
		void reset() {
			game.reset();
			while (!move_record.empty()) {
				train_board(move_record.top());
				move_record.pop();
			}
		}

	public:
		pair<int, int> points() const {
			return game.points();
		}
	};

}  // namespace RL

#endif