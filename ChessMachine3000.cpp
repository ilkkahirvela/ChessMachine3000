#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <stack>
#include "chess.h"
#include "move.h"
#include "position.h"

using namespace std;
using namespace std::chrono;

int main() {
	Position pos;


	//// BOT VS BOT
	//int moveCount = 0;
	//double maxDuration = 0.0;

	//while (moveCount < 150) {
	//	auto start = steady_clock::now();

	//	MinimaxValue value = pos.minimax(4);
	//	pos.movePiece(value._move);
	//	pos.printBoard();
	//	cout << "Move played: " << value._move.toString()
	//		<< " Total moves: " << moveCount << endl;
	//	pos.changeTurn();

	//	auto end = steady_clock::now();
	//	duration<double> elapsed = end - start;
	//	cout << "Time taken for this move: " << elapsed.count() << " seconds" << endl;

	//	if (elapsed.count() > maxDuration) {
	//		maxDuration = elapsed.count();
	//	}

	//	moveCount++;
	//}

	//cout << "Longest move duration: " << maxDuration << " seconds" << endl;


	// PLAYER VS BOT
    vector<Move> allMoves, legalMoves;
    Move playerMove;
    stack<UndoInfo> moveHistory;

    while (true) {
        if (pos._moveturn == WHITE) {
            allMoves.clear();
            legalMoves.clear();

            pos.getAllMoves(pos._moveturn, allMoves);
            pos.getLegalMoves(allMoves, legalMoves);

            cout << "Current board:" << endl;
            pos.printBoard();

            for (const auto& move : legalMoves)
                cout << move.toString() << " ";
            cout << endl << "Total possible moves: " << legalMoves.size() << endl;

            cout << "Position score balance: " << pos.evaluate() << endl;

            cout << "Enter your move in UCI format (or type 'undo' to revert last moves): ";
            string stringMove;
            cin >> stringMove;
            cout << "\n";

            if (stringMove == "undo") {
                if (moveHistory.size() >= 2) {
                    UndoInfo botUndo = moveHistory.top(); moveHistory.pop();
                    Move botMove = botUndo.move;
                    pos.undoMove(botMove, botUndo);
                    pos.changeTurn();

                    UndoInfo playerUndo = moveHistory.top(); moveHistory.pop();
                    Move playerMove = playerUndo.move;
                    pos.undoMove(playerMove, playerUndo);
                    pos.changeTurn();

                    cout << "Undo successful. Reverted last two moves.\n";
                }
                else {
                    cout << "No moves to undo.\n";
                }
                continue;
            }

            playerMove = uciToMove(stringMove);
            if (validMove(legalMoves, playerMove)) {
                UndoInfo undoData = pos.movePiece(playerMove);
                moveHistory.push(undoData);
                pos.changeTurn();
            }
            else {
                cout << "Invalid move. Try again." << endl;
            }
        }
        else {
            auto start = steady_clock::now();
            MinimaxValue minimax = pos.iterativeDeepening(8, 3500);
            cout << "Minimax value of the move made: " << minimax._value << endl;

            UndoInfo undoData = pos.movePiece(minimax._move);
            moveHistory.push(undoData);

            auto end = steady_clock::now();
            duration<double> elapsed = end - start;

            cout << "The bot did the move: " << minimax._move.toString()
                << " in " << elapsed.count() << " seconds." << endl;

            pos.changeTurn();
        }
    }

	return 0;
}
