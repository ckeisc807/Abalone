#include <assert.h>

#include <iostream>

#include "abalone.h"
using std::cout, std::cin;
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
	move current_move;
	cout << "\nAbalone start\n";
	cout << "player1: " << (is_ai[1] ? "AI" : "You") << "\n"
		 << "player2: " << (is_ai[2] ? "AI" : "You") << "\n";
	bool ava = true;
	for (; !abalone.winner;) {
		if (is_ai[abalone.current_player]) {
			cout << abalone << "\n"
				 << "AI: " << abalone.current_player << "\n\n";
			moves = abalone.get_moves();
			cout << "avalible moves: " << moves.size() << "\n";
			for (const move &i : moves) cout << "(" << i.x1 << "," << i.y1 << ") (" << i.x2 << "," << i.y2 << ") direction: " << i.direction << "\n";
			cout << "\nevaluate value: " << evaluate(abalone) << "\n";
			cout << "\n";
			cout << abalone << "\n"
				 << "AI: " << abalone.current_player << "\n\n";
			current_move = AlphaBeta(abalone).second;
			cout << "AI move: (" << current_move.x1 << "," << current_move.y1 << ") (" << current_move.x2 << "," << current_move.y2 << ") direction: " << current_move.direction << "\n";
			assert(current_move.x1 != 15);
			abalone.step(current_move);
			cout << "AI move: "
				 << "(" << current_move.x1 << "," << current_move.y1 << ") (" << current_move.x2 << "," << current_move.y2 << ") direction: " << current_move.direction << "\n";
		}
		else {
			cout << abalone << "\n"
				 << "player: " << abalone.current_player << "\n\n";
			moves = abalone.get_moves();
			cout << "avalible moves: " << moves.size() << "\n"
				 << "x1\ty1\tx2\ty2\tdirection\n";

			for (const move &i : moves) cout << "(" << i.x1 << "," << i.y1 << ") (" << i.x2 << "," << i.y2 << ") direction: " << i.direction << "\n";

			cout << "evaluate value: " << evaluate(abalone) << "\n";
			cout << "\n";
			cout << abalone << "\n"
				 << "player: " << abalone.current_player << "\n\n";
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
}
