#include <assert.h>

#include <ctime>
#include <fstream>
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
pos1 evaluation1;
const int depth_limit1 = 5;
pos1 evaluation2;
const int depth_limit2 = 5;

using std::cout, std::cin;
using std::fstream;
using std::vector;

int main(int argc, char** argv) {
	fstream lout;
	bool with_log = (argc > 2);
	if (with_log) {
		lout.open(argv[2], fstream::out | fstream::trunc);
	}

	clock_t total_time1 = 0, total_time2 = 0, current_time = 0;

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
	if (argc > 1) {
		mode = std::stoi(argv[1]);
		cout << mode << "\n";
	}
	else {
		cin >> mode;
	}

	is_ai[1] = mode & 1, is_ai[2] = mode & 2;

	pair<EvaluationType, move> current_move;
	cout << "\nAbalone start\n";
	cout << "player1: " << (is_ai[1] ? "AI" : "You") << "\n"
		 << "player2: " << (is_ai[2] ? "AI" : "You") << "\n";

	if (with_log) {
		lout << mode << "\n";
		if (is_ai[1]) lout << typeid(evaluation1).name() << " " << depth_limit1 << "\n";
		else cout << "Human 0\n";
		if (is_ai[2]) lout << typeid(evaluation2).name() << " " << depth_limit2 << "\n";
		else cout << "Human 0\n";
	}

	bool ava = true;
	int cnt = 0;
	for (; !abalone.winner;) {
		// if (cnt == 10) break;
		if (is_ai[abalone.current_player]) {
			cout << abalone << "\n"
				 << "AI: " << abalone.current_player << "\n\n";
			cout << "score[1]: " << abalone.score[1] << "\n";
			cout << "score[2]: " << abalone.score[2] << "\n";
			cout << "cnt_turn: " << cnt << "\n";
			current_time = clock();
			// current_move = MinMax(abalone).second;
			if (abalone.current_player == 1) current_move = AlphaBeta(abalone, evaluation1, depth_limit1), current_time = clock() - current_time, total_time1 += current_time;
			else current_move = AlphaBeta(abalone, evaluation2, depth_limit2), current_time = clock() - current_time, total_time2 += current_time;
			cout << "AI move: (" << current_move.second.x1 << "," << current_move.second.y1 << ") (" << current_move.second.x2 << "," << current_move.second.y2 << ") direction: " << current_move.second.direction << "\n";
			cout << "Best result: " << current_move.first << "\n";
			assert(current_move.second.x1 != 15);
			abalone.step(current_move.second);
			if (with_log) {
				lout << current_move.second.x1 << " " << current_move.second.x1 << " " << current_move.second.y1 << " " << current_move.second.x2 << " " << current_move.second.y2 << " " << current_move.second.direction << "\n";
			}
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
			for (const move& i : moves) {
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

	cout << abalone.score[1] << " " << abalone.score[2] << "\n";
	cout << total_time1 << " " << total_time2 << "\n";
	if (with_log) {
		lout << abalone.score[1] << " " << abalone.score[2] << "\n";
		lout << total_time1 << " " << total_time2 << "\n";
	}
	lout.close();
}
