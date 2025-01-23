#include <iostream>
#include <vector>
#include <string>
#include "chess.h"
#include "move.h"
#include "position.h"

using namespace std;

// Test program for valid queen moves
int main() {
    Position position;

    // Queen starts at d1
    int row = 7;
    int col = 3;

    // Loop for queen's moves
    while (true) {
        cout << "Current Board:" << endl;
        position.printBoard();
        cout << endl;

        // Get possible moves for the queen
        vector<Move> queenMoves = position.getQueenMoves(row, col);

        // Print out moves in UCI format, removed in final version for performance??
        cout << "Possible moves for the Queen at " << (char)(col + 'a') << (8 - row) << ":" << endl;
        string uciQueenMoves = coordsToUci(queenMoves);
        cout << uciQueenMoves << endl;

        string move;
        bool validMove = false;

        // Process UCI move input
        while (true) {
            cout << "Enter your move in UCI format or 'q' to quit: ";
            cin >> move;

            if (move == "q") {
                cout << "Exiting the game." << endl;
                return 0;
            }

            // Convert UCI move to coordinates
            auto coords = uciToCoords(move);
            int startRow = coords.first.first;
            int startCol = coords.first.second;
            int endRow = coords.second.first;
            int endCol = coords.second.second;

            // Validate move
            validMove = false;
            for (const auto& queenMove : queenMoves) {
                if (queenMove.endRow == endRow && queenMove.endCol == endCol) {
                    validMove = true;
                    break;
                }
            }

            if (validMove) {
                // Move queen and update position
                position.movePiece(startRow, startCol, endRow, endCol);
                row = endRow;
                col = endCol;

                // Get new possible moves for the queen
                queenMoves = position.getQueenMoves(row, col);
                break;
            }
            else {
                cout << "Invalid move. Try again." << endl;
            }
        }
    }

    return 0;
}
