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

	vector<Move> allMoves;
	vector<Move> legalMoves;
	
	int promotedPiece; // Helper variable for pawn promotion

	while (true) {
		allMoves.clear();
		legalMoves.clear();

		cout << "Current board:" << endl;
		pos.printBoard();

		pos.getAllMoves(pos._moveturn, allMoves);
		pos.getLegalMoves(allMoves, legalMoves);

		int moveCount = 0;
		for (const auto& move : legalMoves) {
			cout << move.toString() << " ";
			moveCount += 1;
		}

		cout << endl << "Total possible moves in the current position: " << moveCount << endl;

		string playerMove;
		cout << "Enter your move in UCI format: ";
		cin >> playerMove;
		cout << endl;

		// Convert UCI move to coordinates
		auto coords = uciToCoords(playerMove, promotedPiece);
		int startRow = coords.first.first;
		int startCol = coords.first.second;
		int endRow = coords.second.first;
		int endCol = coords.second.second;

		if (validMove(legalMoves, startRow, startCol, endRow, endCol)) {
			pos.movePiece(startRow, startCol, endRow, endCol, promotedPiece);
			pos.changeTurn();
		}
		else {
			cout << "Invalid move. Try again." << endl;
		}
	}
	
	return 0;
}
