#include <iostream>
#include <string>
#include <utility>

using std::pair;
using std::string;

/**
 * @brief class for Action, can be used for action space, parameter of step functions, etc.
 */
class ActType {
public:
	bool type;
	int x, y;
	// type = 0, push_line
	int direction;
	// type = 1, push_side
	int line_direction, push_direction, amount;

	ActType() {}

	ActType(int _x, int _y, int _direction)
		: type(false), x(_x), y(_y), direction(_direction) {}

	ActType(int _x, int _y, int _line_direction, int _push_direction, int _amount)
		: type(false), x(_x), y(_y), line_direction(_line_direction), push_direction(_push_direction), amount(_amount) {}
};

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
};