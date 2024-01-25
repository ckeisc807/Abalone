#ifndef _GAME_CLASS_HPP_
#define _GAME_CLASS_HPP_

#include <iostream>
#include <utility>
#include <vector>

#include "game_act_type.hpp"

using std::ostream;
using std::pair;
using std::vector;
namespace RL {
	class Abalone {
		inline const static vector<pair<int, int>> directions = {{1, 0}, {1, 1}, {0, 1}, {-1, 0}, {-1, -1}, {0, -1}};
		vector<vector<char>> board;
		int O_point, X_point;

	public:
		Abalone() {
			board.resize(11, vector<char>(11, '#'));
			O_point = X_point = 0;

			for (int i = 1; i < 6; i++)
				for (int j = 1; j < i + 5; j++)
					board[j][i] = '.';
			for (int i = 6; i < 10; i++)
				for (int j = i - 4; j < 10; j++)
					board[j][i] = '.';

			return;
		}

	public:
		friend ostream &operator<<(ostream &out, const Abalone &self) {
			for (int i = 1; i < 6; i++) {
				for (int j = 0; j < 5 - i; j++)
					out << " ";
				for (int j = 1; j < i + 5; j++)
					out << self.board[j][i] << " ";
				out << "\n";
			}
			for (int i = 6; i < 10; i++) {
				for (int j = 0; j < i - 5; j++)
					out << " ";
				for (int j = i - 4; j < 10; j++)
					out << self.board[j][i] << " ";
				out << "\n";
			}
			return out;
		}

	public:
		pair<int, int> points() {
			return {O_point, X_point};
		}

	public:
		void reset() {
			for (int i = 1; i < 5; i++)
				for (int j = i; j < 6; j++)
					board[j][i] = 'O';
			for (int i = 6; i < 10; i++)
				for (int j = 5; j < i + 1; j++)
					board[j][i] = 'X';
		}

	private:
		pair<int, int> trans(int x, int y) {
			if (y >= 5) return {x + y - 5, y};
			return {x, y};
		}

	public:
		void point_count() {
			int cur_x = 0, cur_y = 0;
			for (const auto &[dx, dy] : Abalone::directions) {
				for (int _ = 0; _ < 5; _++) {
					if (board[cur_x][cur_y] == 'O') X_point += 1;
					if (board[cur_x][cur_y] == 'X') O_point += 1;
					board[cur_x][cur_y] = '#';
					cur_x += dx, cur_y += dy;
				}
			}
			return;
		}

	public:
		void push_line(int x, int y, int direction, char OX) {
			auto [cx, cy] = trans(x, y);
			auto [dx, dy] = directions[direction];

			while (board[cx][cy] != '.' && board[cx][cy] != '#')
				cx += dx, cy += dy;

			auto [start_x, start_y] = trans(x, y);
			while (cx != start_x && cy != start_y) {
				board[cx][cy] = board[cx - dx][cy - dy];
				cx -= dx, cy -= dy;
			}

			board[start_x][start_y] = '.';
		}

	public:
		void push_side(int sx, int sy, int line_direction, int push_direction, int amount, char OX) {
			auto [cx, cy] = trans(sx, sy);
			auto [ldx, ldy] = Abalone::directions[line_direction];
			auto [pdx, pdy] = Abalone::directions[push_direction];

			for (int _ = 0; _ < amount; _++)
				cx += ldx, cy += ldy;

			for (int _ = 0; _ < amount; _++) {
				cx -= ldx, cy -= ldy;
				board[cx + pdx][cy + pdy] = board[cx][cy];
				board[cx][cy] = '.';
			}

			return;
		}

	public:
		bool legal_push_line(int x, int y, int direction, char OX) {
			auto [cx, cy] = trans(x, y);
			auto ops = [](char OX) -> char {
				return OX == 'O' ? 'X' : 'O';
			};

			if (cx >= 11 || cy >= 11 || cx < 0 || cy < 0) return false;
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

			return true;
		}

	public:
		bool legal_push_side(int sx, int sy, int line_direction, int push_direction, int amount, char OX) {
			auto [cx, cy] = trans(sx, sy);

			if (cx >= 11 || cy >= 11 || cx < 0 || cy < 0) return false;
			if (line_direction == push_direction) return false;

			auto [ldx, ldy] = Abalone::directions[line_direction];
			auto [pdx, pdy] = Abalone::directions[push_direction];

			for (int _ = 0; _ < amount; _++) {
				if (board[cx][cy] != OX) return false;
				if (board[cx + pdx][cy + pdy] != '.' && board[cx + pdx][cy + pdy] != '#') return false;
				cx += ldx, cy += ldy;
			}

			return true;
		}

	public:
		vector<ActType> legal_moves(char OX) {
			vector<ActType> result;
			for (int x = 0; x < 9; x++) {
				for (int y = 0; y < 9; y++) {
					if (board[x][y] != OX) continue;
					for (int d = 0; d < 6; d++)
						if (legal_push_line(x, y, d, OX))
							result.push_back(ActType(x, y, d));
				}
			}

			for (int x = 0; x < 9; x++) {
				for (int y = 0; y < 9; y++) {
					if (board[x][y] != OX) continue;
					for (int ld = 0; ld < 6; ld++)
						for (int pd = 0; pd < 6; pd++)
							for (int amount = 0; amount < 3; amount++)
								if (legal_push_side(x, y, ld, pd, amount, OX)) result.push_back(ActType(x, y, ld, pd, amount));
				}
			}

			return result;
		}

	public:
		vector<double> transform_to_vector() {
			vector<double> result;
			result.reserve(61);

			for (const auto &row : board)
				for (const auto &chess : row)
					if (chess != '#')
						result.push_back(chess == '.' ? 0.0 : (chess == 'O' ? 1.0 : -1.0));

			return result;
		}

	public:
		void move(const ActType &act, char OX) {
			if (act.type == false) push_line(act.x, act.y, act.direction, OX);
			else push_side(act.x, act.y, act.line_direction, act.push_direction, act.amount, OX);
		}
	};

}  // namespace RL

#endif