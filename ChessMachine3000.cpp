/**
 * @file main.cpp
 * @brief Main program for chess with human vs bot and bot vs bot game modes.
 *
 * At startup, the user selects the game mode:
 * - Human vs Bot (enter "h"): The human chooses a color (white/0 or black/1) and plays against the bot.
 * - Bot vs Bot (enter "b"): Both sides are controlled by bots.
 *
 * The program uses iterative deepening minimax search with alpha-beta pruning.
 * The termination condition checks for checkmate by locating the king
 * using findKing and testing if its square is under attack with isSquareUnderAttack.
 */

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <stack>
#include <algorithm>
#include <thread>
#include <unordered_map>
#include "chess.h"
#include "move.h"
#include "position.h"

using namespace std;
using namespace std::chrono;

int main() {
    Position pos;
    int maxDepth = 10;
    int timeLimitMs = 3000;
    stack<UndoInfo> moveHistory;
    unordered_map<string, int> positionHistory;

    // Select game mode: human vs bot or bot vs bot
    char gameMode;
    cout << "\033[33mSelect game mode: (h)uman vs bot or (b)ot vs bot: \033[0m";
    cin >> gameMode;

    bool humanMode = false;
    int humanSide = -1; // Will be set to WHITE or BLACK if human mode

    if (gameMode == 'h' || gameMode == 'H') {
        humanMode = true;
        string sideInput;
        cout << "\033[33mChoose your color (w)hite or (b)lack: \033[0m";
        cin >> sideInput;
        if (sideInput == "w" || sideInput == "W") {
            humanSide = WHITE;
        }
        else if (sideInput == "b" || sideInput == "B") {
            humanSide = BLACK;
        }
        else {
            cout << "Invalid input. Defaulting to white." << endl;
            humanSide = WHITE;
        }
    }
    else if (!(gameMode == 'b' || gameMode == 'B')) {
        // If invalid selection, default to human vs bot with white
        cout << "Invalid selection. Defaulting to human vs bot, playing white." << endl;
        humanMode = true;
        humanSide = WHITE;
    }

    // Main game loop
    while (true) {
        cout << "Current board:" << endl;
        pos.printBoard();

        // Generate legal moves for the current side
        vector<Move> allMoves, legalMoves;
        pos.getAllMoves(pos.getTurn(), allMoves);
        pos.getLegalMoves(allMoves, legalMoves);

        // Check for draw by 50-move rule
        if (pos.getHalfMoveClock() >= 100) {
            pos.printBoard();
            cout << "Draw by 50-move rule!" << endl;
            break;
        }

        // Check for draw by threefold repetition
        string posKey = pos.getPositionKey();
        if (++positionHistory[posKey] >= 3) {
            pos.printBoard();
            cout << "Draw by threefold repetition!" << endl;
            break;
        }

        // Check for game termination (checkmate or stalemate)
        if (legalMoves.empty()) {
            int kingRow, kingCol;
            int kingPiece = (pos.getTurn() == WHITE ? wK : bK);
            pos.findKing(kingPiece, kingRow, kingCol);
            int opponent = (pos.getTurn() == WHITE ? BLACK : WHITE);
            if (pos.isSquareUnderAttack(kingRow, kingCol, opponent)) {
                cout << "Checkmate! ";
                if (pos.getTurn() == WHITE) {
                    cout << "Black wins!" << endl;
                }
                else {
                    cout << "White wins!" << endl;
                }
            }
            else {
                cout << "Stalemate!" << endl;
            }
            break;
        }

        // Determine if it is the human's turn (only in human vs bot mode)
        if (humanMode && pos.getTurn() == humanSide) {
            cout << "Legal moves: ";
            for (const auto& move : legalMoves)
                cout << move.toString() << " ";
            cout << "\nTotal moves: " << legalMoves.size() << endl;

            cout << "\033[33mEnter your move in UCI format (or type 'undo' to revert last two moves): \033[0m";
            string inputMove;
            cin >> inputMove;
            cout << "\n";

            // Handle undo command
            if (inputMove == "undo") {
                if (moveHistory.size() >= 2) {
                    UndoInfo botUndo = moveHistory.top(); moveHistory.pop();
                    Move botMove = botUndo.move;
                    pos.undoMove(botMove, botUndo);
                    pos.changeTurn();

                    UndoInfo humanUndo = moveHistory.top(); moveHistory.pop();
                    Move humanMove = humanUndo.move;
                    pos.undoMove(humanMove, humanUndo);
                    pos.changeTurn();

                    cout << "Undo successful. Reverted last two moves.\n";
                }
                else {
                    cout << "No moves to undo.\n";
                }
                continue;
            }

            // Convert UCI string to Move and validate it
            Move humanMove = uciToMove(inputMove);
            if (validMove(legalMoves, humanMove)) {
                UndoInfo undoData = pos.movePiece(humanMove);
                moveHistory.push(undoData);
                pos.changeTurn();
            }
            else {
                cout << "Invalid move. Try again." << endl;
            }
        }
        else {
            // Bot's turn (or both sides in bot vs bot mode)
            auto start = steady_clock::now();
            MinimaxValue minimax = pos.iterativeDeepening(maxDepth, timeLimitMs);
            auto end = steady_clock::now();
            duration<double> elapsed = end - start;

            cout << "Bot (" << (pos.getTurn() == WHITE ? "White" : "Black")
                << ") plays move: " << "\033[31m" << minimax._move.toString() << "\033[0m"
                << " (" << elapsed.count() << " seconds)" << endl;

            UndoInfo undoData = pos.movePiece(minimax._move);
            moveHistory.push(undoData);

            cout << "Position evaluation: " << pos.evaluate() << endl;
            cout << "----------------------------------------" << endl;

            pos.changeTurn();

            // Short delay before the next move
            this_thread::sleep_for(chrono::milliseconds(1000));
        }
    }

    return 0;
}
