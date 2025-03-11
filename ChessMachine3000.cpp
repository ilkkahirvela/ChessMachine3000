/**
 * @file main.cpp
 * @brief Entry point for the chess program.
 *
 * This file sets up the chess board, handles user input, and alternates moves between the player and the bot.
 * The bot uses an iterative deepening minimax search with alpha-beta pruning.
 */

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

/**
 * @brief Main function that runs the chess game.
 *
 * The program asks the user to choose a color and then enters an infinite loop,
 * alternating between processing the player's move and the bot's move. It displays the current board,
 * available moves, and position evaluation. The user can also enter "undo" to revert the last two moves.
 *
 * @return int Exit code.
 */
int main() {
    Position pos;
    string playerInput;
    int playerSide, botSide;

    cout << "Choose your color (white/0 or black/1): ";
    cin >> playerInput;

    // Convert input to lowercase
    transform(playerInput.begin(), playerInput.end(), playerInput.begin(), ::tolower);

    // Determine player's side based on input
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

            // Generate and filter moves for the current player
            pos.getAllMoves(pos._moveturn, allMoves);
            pos.getLegalMoves(allMoves, legalMoves);

            cout << "Current board:" << endl;
            pos.printBoard();

            // Display available legal moves
            for (const auto& move : legalMoves)
                cout << move.toString() << " ";
            cout << endl << "Total possible moves: " << legalMoves.size() << endl;

            cout << "Position score balance: " << pos.evaluate() << endl;

            cout << "Enter your move in UCI format (or type 'undo' to revert last moves): ";
            string stringMove;
            cin >> stringMove;
            cout << "\n";

            // Handle undo command
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

            // Convert UCI string to Move object and validate move
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
            // Bot's turn: use iterative deepening minimax search
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
