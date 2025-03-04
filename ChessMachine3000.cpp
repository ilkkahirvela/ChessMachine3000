#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include "chess.h"
#include "move.h"
#include "position.h"

using namespace std;
using namespace std::chrono;

int main() {
	Position pos;

	// BOT VS BOT
	int moveCount = 0;
	double maxDuration = 0.0;

	while (moveCount < 150) {
		auto start = steady_clock::now();

		MinimaxValue value = pos.minimax(4);
		pos.movePiece(value._move);
		pos.printBoard();
		cout << "Move played: " << value._move.toString()
			<< " Total moves: " << moveCount << endl;
		pos.changeTurn();

		auto end = steady_clock::now();
		duration<double> elapsed = end - start;
		cout << "Time taken for this move: " << elapsed.count() << " seconds" << endl;

		if (elapsed.count() > maxDuration) {
			maxDuration = elapsed.count();
		}

		moveCount++;
	}

	cout << "Longest move duration: " << maxDuration << " seconds" << endl;

	/*vector<Move> allMoves, legalMoves;
	Move playerMove;

	while (true) {
		if (pos._moveturn == WHITE){
			allMoves.clear();
			legalMoves.clear();

			pos.getAllMoves(pos._moveturn, allMoves);
			pos.getLegalMoves(allMoves, legalMoves);

			cout << "Current board:" << endl;
			pos.printBoard();

			for (const auto& move : legalMoves)
				cout << move.toString() << " ";
			cout << endl << "Total possible moves : " << legalMoves.size() << endl;

			cout << "Position score balance: " << pos.evaluate() << endl;

			cout << "Enter your move in UCI format: ";
			string stringMove;
			cin >> stringMove;
			cout << "\n";

			playerMove = uciToMove(stringMove);
			if (validMove(legalMoves, playerMove)) {
				pos.movePiece(playerMove);
				pos.changeTurn();
			}
			else {
				cout << "Invalid move. Try again." << endl;
			}
		}
		else {
			MinimaxValue value = pos.minimax(4);
			cout << "Minimaxvalue of the move made: " << value._value << endl;
			pos.movePiece(value._move);

			Move botMove = value._move;
			cout << "The bot did the move: " << botMove.toString() << endl;

			pos.changeTurn();
		}
	}*/

	return 0;
}
