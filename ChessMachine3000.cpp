#include <iostream>
#include <vector>
#include <string>
#include "chess.h"
#include "move.h"
#include "position.h"

using namespace std;

int main() {
	Position pos;
	vector<Move> allMoves;
	pos._moveturn = WHITE;
	int promotedPiece;

	//pos.emptyBoard();
	//pos.insertTestPiece(1, 4, wP);
	//pos.insertTestPiece(0, 0, bP);

	while (true) {
		allMoves.clear();
		cout << "Current board:" << endl;

		pos.printBoard();

		pos.getAllMoves(pos._moveturn, allMoves);

		int moveCount = 0;
		for (const auto& move : allMoves) {
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

		if (validMove(allMoves, endRow, endCol)) {
			pos.movePiece(startRow, startCol, endRow, endCol, promotedPiece);
			pos.changeTurn();
		}
		else {
			cout << "Invalid move. Try again." << endl;
		}
	}
	
	return 0;
}
