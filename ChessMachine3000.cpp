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

	// Castling test
	//pos.emptyBoard();
	//pos.insertTestPiece(7, 4, wK);
	//pos.insertTestPiece(7, 0, wR);
	//pos.insertTestPiece(7, 7, wR);
	//pos.insertTestPiece(0, 4, bK);
	//pos.insertTestPiece(0, 0, bR);
	//pos.insertTestPiece(0, 7, bR);

	while (true) {
		allMoves.clear();
		legalMoves.clear();

		cout << "Current board:" << endl;
		pos.printBoard();

		pos.getAllMoves(pos._moveturn, allMoves);
		pos.getLegalMoves(allMoves, legalMoves);

		for (const auto& move : legalMoves)
			cout << move.toString() << " ";
		cout << "\nTotal possible moves: " << legalMoves.size() << "\n";
		
		cout << "Material balance: " << pos.material() << "\n";

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
	return 0;
}
