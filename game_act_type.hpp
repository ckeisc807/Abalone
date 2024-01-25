#ifndef _GAME_ACT_TYPE_HPP_
#define _GAME_ACT_TYPE_HPP_

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
		: type(true), x(_x), y(_y), line_direction(_line_direction), push_direction(_push_direction), amount(_amount) {}
};
#endif