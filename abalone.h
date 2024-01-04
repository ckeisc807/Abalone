#include <assert.h>

#include <iostream>
#include <utility>
#include <vector>
using std::max, std::min;
using std::ostream;
using std::swap;
using std::vector, std::pair;
const int dx[6] = {1, 1, 0, -1, -1, 0};
const int dy[6] = {0, 1, 1, 0, -1, -1};
const int inf = 1e9 + 7;
const int depth_limit = 4;
const int weight[] = {100000000, 1000000};
//                  game_end
const int pos_value[5] = {4096, 512, 64, 8, 1};
// inward -> outward

inline bool legal_point(const int &x, const int &y) {
	return 0 <= min(x, y) && max(x, y) < 9 && abs(x - y) < 5;
}
inline bool legal_direction(const int &direction) {
	return 0 <= direction && direction < 6;
}
struct move {
	//  err 15,15,15,15,15;
	int x1, y1, x2, y2, direction;
	move(const int &x1, const int &y1, const int &x2, const int &y2, const int &direction)
		: x1(x1), y1(y1), x2(x2), y2(y2), direction(direction) {
		if (x1 != 15 && y1 != 15 && x2 != 15 && y2 != 15 && direction != 15) assert(legal_point(x1, y1) && legal_point(x2, y2) && legal_direction(direction));
	}
	move(const int &val)
		: x1(val & 15), y1(val >> 4 & 15), x2(val >> 8 & 15), y2(val >> 12 & 15), direction(val >> 16 & 15) {
		if (val == -1) x1 = y1 = x2 = y2 = direction = 15;
		else assert(legal_point(x1, y1) && legal_point(x2, y2) && legal_direction(direction));
	}
	move()
		: x1(15), y1(15), x2(15), y2(15), direction(15){};
	inline int encode() {
		return x1 | y1 << 4 | x2 << 8 | y2 << 12 | direction << 16;
	}
	inline bool operator==(const move &mv) {
		return x1 == mv.x1 && y1 == mv.y1 && x2 == mv.x2 && y1 == mv.y2 && direction == mv.direction;
	}
};
struct Abalone {
	int winner = 0;
	int score[3] = {5, 0, 0};
	int cnt_turn = 0;
	int current_player = 1;
	int board[9][9];
	Abalone()
		: winner(0), cnt_turn(0), current_player(1) {
		score[0] = 5;
		score[1] = score[2] = 0;
		for (int y = 0; y < 9; y++) {
			for (int x = 0; x < 9; x++) {
				if (abs(y - x) > 4) board[x][y] = -1;
				else if (y <= 1 || y == 2 && 2 <= x && x <= 4) board[x][y] = 1;
				else if (y >= 7 || y == 6 && 4 <= x && x <= 6) board[x][y] = 2;
				else board[x][y] = 0;
			}
		}
	};
	inline void step(move mv) {
		int x1 = mv.x1, x2 = mv.x2, y1 = mv.y1, y2 = mv.y2, direction = mv.direction;
		int xm = x1 + x2 >> 1, ym = y1 + y2 >> 1;  // x_middle, y_middle
		assert(legal_point(x1, y1) && legal_point(x2, y2) && legal_direction(direction));
		assert(x1 == x2 || y1 == y2 || x1 - y1 == x2 - y2);	 // check: same line
		assert(board[x1][y1] == current_player && board[xm][ym] == current_player && board[x2][y2] == current_player);
		if (x1 == x2 && y1 == y2) {	 // one
			int nextx = x1 + dx[direction], nexty = y1 + dy[direction];
			assert(legal_point(nextx, nexty));
			assert(board[nextx][nexty] == 0);
			board[nextx][nexty] = current_player;
			board[x1][y1] = 0;
			current_player = (current_player == 1) ? 2 : 1;
			return;
		}
		int len = max(abs(x1 - x2), abs(y1 - y2)) + 1;
		assert(len == 2 || len == 3);
		if (x1 > x2 || x1 == x2 && y1 > y2) swap(x1, x2), swap(y1, y2);	 // p2->p1 direction in [0,1,2]
		int line_direction = direction % 3;
		if (x2 - x1 == (dx[line_direction]) << (len & 1) && y2 - y1 == dy[line_direction] << (len & 1)) {
			if (x2 - x1 != (dx[direction]) << (len & 1) || y2 - y1 != dy[direction] << (len & 1)) swap(x1, x2), swap(y1, y2);
			int nextx = x2 + dx[direction], nexty = y2 + dy[direction], opponent_player = (current_player == 1) ? 2 : 1, opponent_len = 0;
			for (; legal_point(nextx, nexty) && board[nextx][nexty] == opponent_player; nextx += dx[direction], nexty += dy[direction]) opponent_len++;
			assert(len > opponent_len);
			if (opponent_len == 0) {
				assert(board[nextx][nexty] == 0 && legal_point(nextx, nexty));
				board[nextx][nexty] = current_player;
				board[x1][y1] = 0;
				current_player = (current_player == 1) ? 2 : 1;
				return;
			}
			else if (legal_point(nextx, nexty) && board[nextx][nexty] == 0) {
				board[nextx][nexty] = opponent_player;
				board[x2 + dx[direction]][y2 + dy[direction]] = current_player;
				board[x1][y1] = 0;
				current_player = (current_player == 1) ? 2 : 1;
				return;
			}
			assert(!legal_point(nextx, nexty));
			score[current_player]++;
			if (score[current_player] == score[0]) winner = current_player;
			board[x2 + dx[direction]][y2 + dy[direction]] = current_player;
			board[x1][y1] = 0;
			current_player = (current_player == 1) ? 2 : 1;
			return;
		}
		assert(board[x1 + dx[direction]][y1 + dy[direction]] == 0);
		assert(board[xm + dx[direction]][ym + dy[direction]] == 0);
		assert(board[x2 + dx[direction]][y2 + dy[direction]] == 0);
		board[x1 + dx[direction]][y1 + dy[direction]] = board[xm + dx[direction]][ym + dy[direction]] = board[x2 + dx[direction]][y2 + dy[direction]] = current_player;
		board[x1][y1] = board[xm][ym] = board[x2][y2] = 0;
		current_player = (current_player == 1) ? 2 : 1;
		return;
	}
	vector<int> get_moves() {
		vector<int> moves;
		move tmp(0);
		bool success;
		for (int y = 0; y < 9; y++) {
			for (int x = 0; x < 9; x++) {
				if (board[x][y] != current_player) continue;
				for (int direction = 0, xn, yn, xnn, ynn; direction < 6; direction++) {
					xn = x + dx[direction], yn = y + dy[direction];
					if (!legal_point(xn, yn)) continue;
					if (board[xn][yn] == 0) {  // move 1
						tmp = move(x, y, x, y, direction);
						moves.push_back(tmp.encode());
					}
					else if (direction < 3 && board[xn][yn] == current_player) {
						for (int ndirection = 0, nx, nxn, ny, nyn, nxnn, nynn, tmpx, tmpy, nnx, nny, nnnx, nnny; ndirection < 6; ndirection++) {
							if (ndirection == direction) {
								nxn = xn + dx[ndirection], nyn = yn + dy[ndirection];
								success = false;
								if (!legal_point(nxn, nyn)) continue;
								if (board[nxn][nyn] == current_player) {
									xnn = nxn, ynn = nyn;
									nxnn = xnn + dx[ndirection], nynn = ynn + dy[ndirection];
									success = false;
									if (!legal_point(nxnn, nynn)) continue;
									else if (board[nxnn][nynn] == 0) success = true;
									else if (board[nxnn][nynn] == current_player) continue;
									else {
										tmpx = nxnn + dx[ndirection], tmpy = nynn + dy[ndirection];
										if (!legal_point(tmpx, tmpy)) success = true;
										else if (board[tmpx][tmpy] == 0) success = true;
										else {
											if (!legal_point(tmpx, tmpy)) success = true;
											else if (board[tmpx][tmpy] == 0) success = true;
											else if (board[tmpx][tmpy] == current_player) continue;
											else {
												tmpx += dx[ndirection], tmpy += dy[ndirection];
												if (!legal_point(tmpx, tmpy)) success = true;
												else if (board[tmpx][tmpy] == 0) success = true;
											}
										}
									}
									if (success) {	// push 3+
										tmp = move(x, y, xnn, ynn, ndirection);
										moves.push_back(tmp.encode());
									}
								}
								else {	// push 2+
									tmpx = nxn + dx[ndirection], tmpy = nyn + dy[ndirection];
									if (board[nxn][nyn] == 0) success = true;
									else if (!legal_point(tmpx, tmpy)) success = true;
									else if (board[tmpx][tmpy] == 0) success = true;
									if (success) {
										tmp = move(x, y, xn, yn, ndirection);
										moves.push_back(tmp.encode());
									}
								}
							}
							else if (ndirection == direction + 3 || ndirection + 3 == direction) {	// reverse 2
								nx = x + dx[ndirection], ny = y + dy[ndirection];
								success = false;
								if (!legal_point(nx, ny)) continue;
								else if (board[nx][ny] == 0) success = true;
								else if (board[nx][ny] == current_player) continue;
								else {
									nnx = nx + dx[ndirection], nny = ny + dy[ndirection];
									if (!legal_point(nnx, nny)) success = true;
									else if (board[nnx][nny] == 0) success = true;
								}
								if (success) {	// push 2-
									tmp = move(x, y, xn, yn, ndirection);
									moves.push_back(tmp.encode());
								}
								xnn = xn + dx[direction], ynn = yn + dy[direction];
								nnnx = nnx + dx[ndirection], nnny = nny + dy[ndirection];
								if (!legal_point(xnn, ynn)) continue;
								else if (board[xnn][ynn] != current_player) continue;
								else if (success) 0;
								else if (!legal_point(nnx, nny)) success = true;
								else if (board[nnx][nny] == current_player) continue;
								else if (board[nnx][nny] == 0) success = true;
								else if (!legal_point(nnnx, nnny)) success = true;
								else if (board[nnnx][nnny] == 0) success = true;
								if (success) {	// push 3-
									tmp = move(x, y, xnn, ynn, ndirection);
									moves.push_back(tmp.encode());
								}
							}
							else {	// side move
								nx = x + dx[ndirection], ny = y + dy[ndirection];
								nxn = xn + dx[ndirection], nyn = yn + dy[ndirection];
								if (!legal_point(nx, ny) || !legal_point(nxn, nyn)) continue;
								if (board[nx][ny] != 0 || board[nxn][nyn] != 0) continue;
								tmp = move(x, y, xn, yn, ndirection);
								moves.push_back(tmp.encode());	// move 2
								xnn = xn + dx[direction], ynn = yn + dy[direction];
								nxnn = xnn + dx[ndirection], nynn = ynn + dy[ndirection];
								if (!legal_point(xnn, ynn) || !legal_point(nxnn, nynn)) continue;
								if (board[xnn][ynn] == current_player && board[nxnn][nynn] == 0) {	// move 3
									tmp = move(x, y, xnn, ynn, ndirection);
									moves.push_back(tmp.encode());
								}
							}
						}
					}
				}
			}
		}
		return moves;
	}
	inline void step(int x1, int y1, int x2, int y2, int direction) {
		int xm = x1 + x2 >> 1, ym = y1 + y2 >> 1;  // x_middle, y_middle
		assert(legal_point(x1, y1) && legal_point(x2, y2) && legal_direction(direction));
		assert(x1 == x2 || y1 == y2 || x1 - y1 == x2 - y2);	 // check: same line
		assert(board[x1][y1] == current_player && board[xm][ym] == current_player && board[x2][y2] == current_player);
		if (x1 == x2 && y1 == y2) {	 // one
			int nextx = x1 + dx[direction], nexty = y1 + dy[direction];
			assert(legal_point(nextx, nexty));
			assert(board[nextx][nexty] == 0);
			board[nextx][nexty] = current_player;
			board[x1][y1] = 0;
			current_player = (current_player == 1) ? 2 : 1;
			return;
		}
		int len = max(abs(x1 - x2), abs(y1 - y2)) + 1;
		assert(len == 2 || len == 3);
		if (x1 > x2 || x1 == x2 && y1 > y2) swap(x1, x2), swap(y1, y2);	 // p2->p1 direction in [0,1,2]
		int line_direction = direction % 3;
		if (x2 - x1 == (dx[line_direction]) << (len & 1) && y2 - y1 == dy[line_direction] << (len & 1)) {
			if (x2 - x1 != (dx[direction]) << (len & 1) || y2 - y1 != dy[direction] << (len & 1)) swap(x1, x2), swap(y1, y2);
			int nextx = x2 + dx[direction], nexty = y2 + dy[direction], opponent_player = (current_player == 1) ? 2 : 1, opponent_len = 0;
			for (; legal_point(nextx, nexty) && board[nextx][nexty] == opponent_player; nextx += dx[direction], nexty += dy[direction]) opponent_len++;
			assert(len > opponent_len);
			if (opponent_len == 0) {
				assert(board[nextx][nexty] == 0 && legal_point(nextx, nexty));
				board[nextx][nexty] = current_player;
				board[x1][y1] = 0;
				current_player = (current_player == 1) ? 2 : 1;
				return;
			}
			else if (legal_point(nextx, nexty) && board[nextx][nexty] == 0) {
				board[nextx][nexty] = opponent_player;
				board[x2 + dx[direction]][y2 + dy[direction]] = current_player;
				board[x1][y1] = 0;
				current_player = (current_player == 1) ? 2 : 1;
				return;
			}
			assert(!legal_point(nextx, nexty));
			score[current_player]++;
			if (score[current_player] == score[0]) winner = current_player;
			board[x2 + dx[direction]][y2 + dy[direction]] = current_player;
			board[x1][y1] = 0;
			current_player = (current_player == 1) ? 2 : 1;
			return;
		}
		assert(board[x1 + dx[direction]][y1 + dy[direction]] == 0);
		assert(board[xm + dx[direction]][ym + dy[direction]] == 0);
		assert(board[x2 + dx[direction]][y2 + dy[direction]] == 0);
		board[x1 + dx[direction]][y1 + dy[direction]] = board[xm + dx[direction]][ym + dy[direction]] = board[x2 + dx[direction]][y2 + dy[direction]] = current_player;
		board[x1][y1] = board[xm][ym] = board[x2][y2] = 0;
		current_player = (current_player == 1) ? 2 : 1;
		return;
	}
};
ostream &operator<<(ostream &out, Abalone abalone) {
	for (int y = 0; y < 9; y++) {
		for (int s = abs(4 - y); s--;) out << ' ';
		for (int x = max(0, y - 4); x < 9 && abalone.board[x][y] != -1; x++) out << abalone.board[x][y] << ' ';
		out << '\n';
	}
	return out;
}

// unnecessary
using std::istream;
istream &operator>>(istream &in, Abalone &abalone) {
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			if (abalone.board[x][y] == -1) continue;
			in >> abalone.board[x][y];
		}
	}
	return in;
}
// evaluates
inline int evaluate(const Abalone &abalone) {
	int current_player = abalone.current_player, opponent_player = (current_player == 1) ? 2 : 1;
	int delta_score = abalone.score[current_player] - abalone.score[opponent_player];
	if (delta_score == abalone.score[0]) return weight[0];
	else if (delta_score == -abalone.score[0]) return -weight[0];
	int value, tmp_value, tmp;
	value = weight[1] * (abalone.score[current_player] - abalone.score[opponent_player]);
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			if (abalone.board[x][y] == -1) continue;
			tmp = max(abs(x - 4), max(abs(y - 4), abs(abs(x - y) - 4)));
			tmp_value = pos_value[tmp];
			if (abalone.board[x][y] == current_player) value += tmp_value;
			else if (abalone.board[x][y] == opponent_player) value -= tmp_value;
		}
	}
	return value;
}
const move empty_move;
pair<int, move> AlphaBeta(Abalone &abalone, int depth = 0, int alpha = -inf, int beta = inf) {
	int current_player = abalone.current_player, opponent_player = (current_player == 1) ? 2 : 1;
	if (abalone.winner == current_player) return {weight[0], empty_move};
	else if (abalone.winner == opponent_player) return {-weight[0], empty_move};
	else if (depth == depth_limit) return {evaluate(abalone), empty_move};
	int best_score = -inf, score;
	move best_move = empty_move;
	Abalone temp;
	for (const move &mv : abalone.get_moves()) {
		temp = abalone;
		temp.step(mv);
		score = -AlphaBeta(temp, depth + 1, -beta, -alpha).first;
		if (score >= beta) return {beta, empty_move};
		if (score > alpha) alpha = score, best_move = mv;
		if (best_move == empty_move) best_move = empty_move;
	}
	return {alpha, best_move};
}