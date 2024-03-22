#include <iostream>

#include "./AB/AB_env.hpp"
// #include "./RL/NN.hpp"
#include "./RL/RL_env.hpp"

using std::cout;

bool play_game(GameEnv &A, GameEnv &B, int MAX_STEP) {
	static int cnt = 0;
	cout << "play game " << ++cnt << " ...\n";
	A.reset(), B.reset();
	pair<ActType, bool> result_A, result_B;
	for (int i = 0; i < MAX_STEP; i++) {
		result_A = A.self_step();
		result_A.first.reverse();
		B.enemy_step(result_A.first);
		if (result_A.second) break;

		result_B = B.self_step();
		result_B.first.reverse();
		A.enemy_step(result_B.first);
		if (result_B.second) break;
	}
	return A.self_win();
}

int main() {
	RL::RLEnv RL_abalone(0.4, 0.3, 0.8, NN::Sigmoid, NN::dSigmoid);
	AB::AB_env AB_abalone, AB1_abalone;
	for (int i = 0; i < 1; i++) {
		cout << play_game(AB_abalone, RL_abalone, 100) << "\n";
	}
}