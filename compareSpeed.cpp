#include <assert.h>

#include <ctime>
#include <fstream>
#include <iostream>

#define EvaluationType float
const EvaluationType inf = 3e34;
#include "abalone.h"
#include "alphaBeta.h"
#include "minMax.h"
//
#include "evaluations/countline.h"
#include "evaluations/pos1.h"
#include "evaluations/pos2.h"
#include "evaluations/pos3.h"
#include "evaluations/pos_norand.h"
#include "evaluations/random.h"
pos_norand evaluation1;
const int depth_limit1 = 3;
Random evaluation2;
const int depth_limit2 = 3;

using std::cout, std::cin;
using std::fstream;
using std::vector;

int main(int argc, char** argv) {
	fstream lout;
	bool with_log = (argc > 2);
	if (with_log) {
		lout.open(argv[2], fstream::out | fstream::trunc);
	}

	clock_t total_time1_ab = 0, total_time1_mm = 0, total_time2_ab = 0, total_time2_mm = 0, current_time = 0;

	Abalone abalone;
	int x1, x2, y1, y2, direction;
	vector<int> moves;
	bool is_ai[3]{false};
	cout << "please choose game mode: []\n"
		 << "0. random versus random\n"
		 << "1. AI versus random\n"
		 << "2. random versus AI\n"
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

	pair<EvaluationType, move> current_move, tmp_move;
	cout << "\nAbalone start\n";
	cout << "player1: " << (is_ai[1] ? "AI" : "You") << "\n"
		 << "player2: " << (is_ai[2] ? "AI" : "You") << "\n";

	if (with_log) {
		lout << mode << "\n";
		if (is_ai[1]) lout << typeid(evaluation1).name() << " " << depth_limit1 << "\n";
		else lout << "Human 0\n";
		if (is_ai[2]) lout << typeid(evaluation2).name() << " " << depth_limit2 << "\n";
		else lout << "Human 0\n";
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
			cout << "time: " << current_time << '\n';
			if (abalone.current_player == 1) current_move = AlphaBeta(abalone, evaluation1, depth_limit1), current_time = clock() - current_time, total_time1_ab += current_time;
			else current_move = AlphaBeta(abalone, evaluation2, depth_limit2), current_time = clock() - current_time, total_time2_ab += current_time;

			current_time = clock();
			cout << "time: " << current_time << '\n';
			if (abalone.current_player == 1) tmp_move = MinMax(abalone, evaluation1, depth_limit1), current_time = clock() - current_time, total_time1_mm += current_time;
			else tmp_move = MinMax(abalone, evaluation2, depth_limit2), current_time = clock() - current_time, total_time2_mm += current_time;

			cout << "AI move: (" << current_move.second.x1 << "," << current_move.second.y1 << ") (" << current_move.second.x2 << "," << current_move.second.y2 << ") direction: " << current_move.second.direction << "\n";
			cout << "AI move: (" << tmp_move.second.x1 << "," << tmp_move.second.y1 << ") (" << tmp_move.second.x2 << "," << tmp_move.second.y2 << ") direction: " << tmp_move.second.direction << "\n";
			cout << "Best result: " << current_move.first << "\n";
			assert(current_move.second.x1 != 15);
			assert(current_move.second == tmp_move.second);
			abalone.step(current_move.second);
			if (with_log) {
				lout << current_move.second.x1 << " " << current_move.second.y1 << " " << current_move.second.x2 << " " << current_move.second.y2 << " " << current_move.second.direction << "\n";
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
			/*
			cin >> x1 >> y1 >> x2 >> y2 >> direction;
			*/
			move mv = moves[rd() % moves.size()];
			x1 = mv.x1, y1 = mv.y1, x2 = mv.x2, y2 = mv.y2, direction = mv.direction;
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
	cout << total_time1_ab << " " << total_time1_mm << "\n";
	cout << total_time2_ab << " " << total_time2_mm << "\n";
	if (with_log) {
		lout << abalone.score[1] << " " << abalone.score[2] << "\n";
		lout << total_time1_ab << " " << total_time1_mm << "\n";
		lout << total_time2_ab << " " << total_time2_mm << "\n";
	}
	lout.close();
}
