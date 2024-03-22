#ifndef _GAME_ACT_TYPE_HPP_
#define _GAME_ACT_TYPE_HPP_

#include <iostream>
using std::ostream;
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

private:
	inline const static int line_amount[] = {0, 5, 6, 7, 8, 9, 8, 7, 6, 5};

public:
	ActType() {}

	ActType(int _x, int _y, int _direction)
		: type(false), x(_x), y(_y), direction(_direction) {}

	ActType(int _x, int _y, int _line_direction, int _push_direction, int _amount)
		: type(true), x(_x), y(_y), line_direction(_line_direction), push_direction(_push_direction), amount(_amount) {}

	friend ostream& operator<<(ostream& out, const ActType& self) {
		out << (self.type ? "push_side\n" : "push_line\n");
		out << "x y: " << self.x << " " << self.y << "\n";
		out << "direction: " << self.direction << "\n";
		out << "ld, pd: " << self.line_direction << " " << self.push_direction << "\n";
		out << "amount: " << self.amount << "\n";

		return out;
	}

	void reverse() {
		direction = (direction + 3) % 6;
		line_direction = (line_direction + 3) % 6;
		push_direction = (push_direction + 3) % 6;
		x = line_amount[y] - x + 1;
		y = 10 - y;
	}
};
#endif