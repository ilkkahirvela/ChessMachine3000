#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <stack>
#include <algorithm>
#include "chess.h"
#include "move.h"
#include "position.h"

using namespace std;
using namespace std::chrono;

int main() {
    Position pos;
    string playerInput;
    int playerSide, botSide;

    cout << "Choose your color (white/0 or black/1): ";
    cin >> playerInput;

    transform(playerInput.begin(), playerInput.end(), playerInput.begin(), ::tolower);

    if (playerInput == "white" || playerInput == "0") {
        playerSide = WHITE;
    }
    else if (playerInput == "black" || playerInput == "1") {
        playerSide = BLACK;
    }
    else {
        cout << "Invalid input. Defaulting to white." << endl;
        playerSide = WHITE;
    }

    vector<Move> allMoves, legalMoves;
    Move playerMove;
    stack<UndoInfo> moveHistory;

    while (true) {
        if (pos._moveturn == playerSide) {
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
            MinimaxValue minimax = pos.iterativeDeepening(8, 3000);
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
