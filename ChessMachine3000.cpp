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

	//pos.emptyBoard();
	//pos.insertTestPiece(6, 4, bP);
	//pos.insertTestPiece(0, 3, bP);
	pos.printBoard();

	pos.getAllMoves(pos._moveturn, allMoves);

	int moveCount = 0;
	for (const auto& move : allMoves) {
		cout << move.toString() << " ";
		moveCount += 1;
	}

	cout << endl << "Total possible moves in the current position: " << moveCount << endl;

	return 0;
}


//// Queen movement testing
//int main() {
//    Position position;
//    vector<Move> queenMoves;
//
//    // Queen starts at d1
//    int row = 7;
//    int col = 3;
//
//    // Loop for queen's moves
//    while (true) {
//        cout << "Current Board:" << endl;
//        position.printBoard();
//        cout << endl;
//
//        // Clear old moves
//        queenMoves.clear();
//
//        // Get possible moves for the queen
//        position.getQueenMoves(row, col, queenMoves);
//
//        // Print out moves in UCI format
//        cout << "Possible moves for the Queen at " << (char)(col + 'a') << (8 - row) << ":" << endl;
//        string uciQueenMoves = coordsToUci(queenMoves);
//        cout << uciQueenMoves << endl;
//
//        string move;
//        bool validMove = false;
//
//        // Process UCI move input
//        while (true) {
//            cout << "Enter your move in UCI format or 'q' to quit: ";
//            cin >> move;
//
//            if (move == "q") {
//                cout << "Exiting the game." << endl;
//                return 0;
//            }
//
//            // Convert UCI move to coordinates
//            auto coords = uciToCoords(move);
//            int startRow = coords.first.first;
//            int startCol = coords.first.second;
//            int endRow = coords.second.first;
//            int endCol = coords.second.second;
//
//            // Validate move
//            validMove = false;
//            for (const auto& queenMove : queenMoves) {
//                if (queenMove.endRow == endRow && queenMove.endCol == endCol) {
//                    validMove = true;
//                    break;
//                }
//            }
//
//            if (validMove) {
//                // Move queen and update position
//                position.movePiece(startRow, startCol, endRow, endCol);
//                row = endRow;
//                col = endCol;
//
//                // Break out of input loop to update the board
//                break;
//            }
//            else {
//                cout << "Invalid move. Try again." << endl;
//            }
//        }
//    }
//
//    return 0;
//}
