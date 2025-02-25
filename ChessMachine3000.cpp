#include <iostream>
#include <vector>
#include <string>
#include "chess.h"
#include "move.h"
#include "position.h"

using namespace std;

int main() {
	Position pos;

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

	// BOT VS BOT
	int moveCount = 0;
	while (moveCount < 300) {
		MinimaxValue value = pos.minimax(3);
		pos.movePiece(value._move);
		pos.printBoard();
		cout << "Move played: " << value._move.toString() << " Total moves: " << moveCount << endl;
		pos.changeTurn();
		moveCount++;
	}

	return 0;
}
