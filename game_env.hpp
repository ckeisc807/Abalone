#ifndef _GAME_ENV_HPP_
#define _GAME_ENV_HPP_

#include <iostream>
#include <string>
#include <utility>

#include "game_act_type.hpp"

using std::pair;
using std::string;

/**
 * @brief Base Class for game environment
 */
class GameEnv {
public:
	/**
	 * @brief When a game is created or previous game has ended, reset the game
	 */
	virtual void reset() = 0;

	/**
	 * @brief Enemy's decision, update the board with step
	 */
	virtual void enemy_step(const ActType &step) = 0;
	/**
	 * @brief According to the current board, decide what to do, return {step, terminated or truncated}
	 */
	virtual pair<ActType, bool> self_step() = 0;

	virtual bool self_win() = 0;
};

#endif