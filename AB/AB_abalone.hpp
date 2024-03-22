#ifndef _AB_ABALONE_HPP_
#define _AB_ABALONE_HPP_

#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

#include "../game_class.hpp"

using std::pair;
using std::tuple;
using std::vector;

const int INF = 1e9;

namespace AB {
	class AB_abalone : public Abalone {
	public:
		int evaluate() const {
			if (O_point >= 5) return INF;
			if (X_point >= 5) return -INF;

			int result = 0;

			auto score_table = [](char OX, int amount) -> int {
				int result;
				switch (amount) {
					case 0:
						result = 0;
						break;

					case 1:
						result = 1;
						break;

					case 2:
						result = 100;
						break;

					case 3:
						result = 500;
						break;

					default:
						result = 510;
						break;
				}

				if (OX == 'X') result = -result;
				return result;
			};

			for (const auto &[sx, sy, d] : (vector<tuple<int, int, int>>){{1, 1, 0}, {5, 1, 1}, {9, 5, 2}}) {
				auto [scx, scy] = trans(sx, sy);
				int line_sd1 = (d + 2) % 6, line_sd2 = (d + 1) % 6;
				scx -= directions[line_sd1].first, scy -= directions[line_sd1].second;

				for (int i = 0; i < 5; i++) {
					scx += directions[line_sd1].first, scy += directions[line_sd1].second;
					int O_amount = 0, X_amount = 0;
					int cx = scx, cy = scy;
					for (int _ = 0; _ < i + 5; _++) {
						if (board[cx][cy] != 'O') O_amount = 0, result += score_table('O', O_amount);
						else O_amount++;

						if (board[cx][cy] != 'X') X_amount = 0, result += score_table('X', X_amount);
						else X_amount++;

						cx += directions[d].first, cy += directions[d].second;
					}
				}

				for (int i = 0; i < 4; i++) {
					scx += directions[line_sd2].first, scy += directions[line_sd2].second;
					int O_amount = 0, X_amount = 0;
					int cx = scx, cy = scy;
					for (int _ = 0; _ < 9 - i; _++) {
						if (board[cx][cy] != 'O') O_amount = 0, result += score_table('O', O_amount);
						else O_amount++;

						if (board[cx][cy] != 'X') X_amount = 0, result += score_table('X', X_amount);
						else X_amount++;

						cx += directions[d].first, cy += directions[d].second;
					}
				}
			}

			result += O_point * 50;
			result -= X_point * 50;
			return result;
		}

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
}  // namespace AB

#endif