#include "../game_class.hpp"

namespace RL {
	class RL_Abalone : public Abalone {
	public:
		vector<double> transform_to_vector() const {
			vector<double> result;
			result.reserve(61);

			for (const auto &row : board)
				for (const auto &chess : row)
					if (chess != '#')
						result.push_back(chess == '.' ? 0.0 : (chess == 'O' ? 1.0 : -1.0));

			return result;
		}

		// 防自殺
		virtual bool legal_push_line(int x, int y, int direction, char OX) const override {
			auto [cx, cy] = trans(x, y);
			auto ops = [](char OX) -> char {
				return OX == 'O' ? 'X' : 'O';
			};

			if (cx >= 11 || cy >= 11 || cx <= 0 || cy <= 0) return false;
			if (board[cx][cy] != OX) return false;

			auto [dx, dy] = Abalone::directions[direction];
			int friend_amount = 0, enemy_amount = 0;
			bool cross = false;

			while (board[cx][cy] != '.' && board[cx][cy] != '#') {
				char cur_chess = board[cx][cy];
				if (cur_chess == OX) {
					if (cross) return false;
					friend_amount += 1;
				}
				if (cur_chess == ops(OX)) {
					cross = true;
					enemy_amount += 1;
				}
				cx += dx, cy += dy;
			}

			if (!cross && board[cx][cy] == '#') return false;
			if (friend_amount <= enemy_amount) return false;
			if (friend_amount > 3) return false;

			return true;
		}

		// 防自殺
		virtual bool legal_push_side(int sx, int sy, int line_direction, int push_direction, int amount, char OX) const override {
			auto [cx, cy] = trans(sx, sy);

			if (cx >= 11 || cy >= 11 || cx <= 0 || cy <= 0) return false;
			if (line_direction == push_direction) return false;
			if (amount > 3) return false;

			auto [ldx, ldy] = Abalone::directions[line_direction];
			auto [pdx, pdy] = Abalone::directions[push_direction];

			for (int _ = 0; _ < amount; _++) {
				if (board[cx][cy] != OX) return false;
				if (board[cx + pdx][cy + pdy] != '.') return false;
				cx += ldx, cy += ldy;
			}

			return true;
		}
	};
};	// namespace RL