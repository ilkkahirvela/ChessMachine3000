#include <iostream>
#include <vector>
#include <string>
#include "chess.h"
#include "move.h"
#include "position.h"

using namespace std;

int main() {
	Position pos;
	pos._moveturn = WHITE;

	vector<Move> allMoves, legalMoves;
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
			cout << "\nTotal possible moves: " << legalMoves.size() << "\n";

			cout << "Position score balance: " << pos.evaluate() << "\n";

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
	}
	return 0;
}
