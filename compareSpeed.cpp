#include <assert.h>

#include <iostream>

#define EvaluationType float
const EvaluationType inf = 3e34;
#include "abalone.h"
#include "alphaBeta.h"
#include "evaluations/countline.h"
#include "evaluations/pos1.h"
#include "evaluations/pos2.h"
#include "evaluations/pos3.h"
#include "evaluations/random.h"
#include "minMax.h"
pos1 evaluation1;
const int depth_limit1 = 3;
pos3 evaluation2;
const int depth_limit2 = 3;

using std::cout, std::cin;
using std::vector;

int main() {
	Abalone abalone;
	int x1, x2, y1, y2, direction;
	vector<int> moves;
	bool is_ai[3]{false};
	cout << "please choose game mode: []\n"
		 << "0. player versus player\n"
		 << "1. AI versus player\n"
		 << "2. player versus AI\n"
		 << "3. AI versus AI\n";
	int mode;
	cin >> mode;
	is_ai[1] = mode & 1, is_ai[2] = mode & 2;
	pair<EvaluationType, move> current_move;
	cout << "\nAbalone start\n";
	cout << "player1: " << (is_ai[1] ? "AI" : "You") << "\n"
		 << "player2: " << (is_ai[2] ? "AI" : "You") << "\n";
	bool ava = true;
	time_t start_time = time(NULL);
	int cnt = 0;
	for (; !abalone.winner;) {
		// if (cnt == 10) break;
		if (is_ai[abalone.current_player]) {
			cout << abalone << "\n"
				 << "AI: " << abalone.current_player << "\n\n";
			cout << "score[1]: " << abalone.score[1] << "\n";
			cout << "score[2]: " << abalone.score[2] << "\n";
			cout << "cnt_turn: " << cnt << "\n";
			// current_move = MinMax(abalone).second;
			if (abalone.current_player == 1) current_move = AlphaBeta(abalone, evaluation1, depth_limit1);
			else current_move = AlphaBeta(abalone, evaluation2, depth_limit2);
			cout << "AI move: (" << current_move.second.x1 << "," << current_move.second.y1 << ") (" << current_move.second.x2 << "," << current_move.second.y2 << ") direction: " << current_move.second.direction << "\n";
			cout << "Best result: " << current_move.first << "\n";
			assert(current_move.second.x1 != 15);
			abalone.step(current_move.second);
			cnt++;
		}
		else {
			moves = abalone.get_moves();
			/* // show avalible moves
						cout << abalone << "\n"
							 << "player: " << abalone.current_player << "\n\n";
						cout << "avalible moves: " << moves.size() << "\n"
							 << "x1\ty1\tx2\ty2\tdirection\n";
						for (const move &i : moves) cout << "(" << i.x1 << "," << i.y1 << ") (" << i.x2 << "," << i.y2 << ") direction: " << i.direction << "\n";
			*/

			cout << "\n";
			cout << abalone << "\n"
				 << "player: " << abalone.current_player << "\n\n";
			cout << "score[1]: " << abalone.score[1] << "\n";
			cout << "score[2]: " << abalone.score[2] << "\n";
			cout << "cnt_turn: " << cnt << "\n";
			if (!ava) cout << "invaild move\n";
			cin >> x1 >> y1 >> x2 >> y2 >> direction;
			ava = false;
			for (const move &i : moves) {
				if (x1 == i.x1 && y1 == i.y1 && x2 == i.x2 && y2 == i.y2 && direction == i.direction) {
					ava = true;
					break;
				}
				else if (x1 == i.x2 && y1 == i.y2 && x2 == i.x1 && y2 == i.y1 && direction == i.direction) {
					ava = true;
					break;
				}
			}
			if (ava) abalone.step(x1, y1, x2, y2, direction);
		}
	}
	cout << "time_cost: " << time(NULL) - start_time << "\n";
}

// Alpha Beta 58 seconds / 100 steps
// Min Max