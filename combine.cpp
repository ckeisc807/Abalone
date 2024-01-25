#include <memory>

#include "NN.hpp"
#include "RL_env.hpp"
#include "game_env.hpp"

bool play_game(GameEnv &A, GameEnv &B, int MAX_STEP) {
	A.reset(), B.reset();
	pair<ActType, bool> result_A, result_B;
	for (int i = 0; i < MAX_STEP; i++) {
		result_A = A.self_step();
		if (result_A.second) break;
		B.enemy_step(result_A.first);
		result_B = B.self_step();
		if (result_B.second) break;
		A.enemy_step(result_B.first);
	}
	return A.self_win();
}

int main() {
	RL::RL_env RL_abalone(0.4, 0.3, 0.8, NN::Sigmoid, NN::dSigmoid);
	GameEnv AB_abalone;
	for (int i = 0; i < 10; i++)
		play_game(RL_abalone, AB_abalone, 1000);
}