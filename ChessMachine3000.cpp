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
#include <iomanip>
#include <sstream>
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
    Move lastMove;
    int fullMoveNumber = 1;

    while (true) {
        // Clear screen and print header
        cout << "\033[2J\033[H";
        cout << "\033[1m=== ChessMachine3000 ===\033[0m";
        cout << "  Move " << fullMoveNumber;
        cout << "  |  \033[1m" << (pos.getTurn() == WHITE ? "White" : "Black") << " to play\033[0m";
        if (lastMove.startRow != -1)
            cout << "  |  Last: \033[1;33m" << lastMove.toString() << "\033[0m";
        cout << "\n";

        int halfClock = pos.getHalfMoveClock();
        cout << "50-move clock: " << halfClock << "/100";
        if (halfClock >= 80) cout << "  \033[1;31m(!)\033[0m";
        cout << "\n\n";

        pos.printBoard(lastMove);

        // Generate legal moves for the current side
        vector<Move> allMoves, legalMoves;
        pos.getAllMoves(pos.getTurn(), allMoves);
        pos.getLegalMoves(allMoves, legalMoves);

        // Check for draw by 50-move rule
        if (pos.getHalfMoveClock() >= 100) {
            cout << "\033[1;33mDraw by 50-move rule!\033[0m\n";
            break;
        }

        // Check for draw by threefold repetition
        string posKey = pos.getPositionKey();
        if (++positionHistory[posKey] >= 3) {
            cout << "\033[1;33mDraw by threefold repetition!\033[0m\n";
            break;
        }

        // Check for checkmate or stalemate
        if (legalMoves.empty()) {
            int kingRow, kingCol;
            int kingPiece = (pos.getTurn() == WHITE ? wK : bK);
            pos.findKing(kingPiece, kingRow, kingCol);
            int opp = (pos.getTurn() == WHITE ? BLACK : WHITE);
            if (pos.isSquareUnderAttack(kingRow, kingCol, opp)) {
                cout << "\033[1;31mCheckmate!  "
                     << (pos.getTurn() == WHITE ? "Black" : "White")
                     << " wins!\033[0m\n";
            }
            else {
                cout << "\033[1;33mStalemate! Draw.\033[0m\n";
            }
            break;
        }

        // Human's turn
        if (humanMode && pos.getTurn() == humanSide) {
            cout << "Legal moves (" << legalMoves.size() << "): ";
            for (const auto& move : legalMoves)
                cout << move.toString() << " ";
            cout << "\n\n";

            cout << "\033[33mYour move (UCI, or 'undo'): \033[0m";
            string inputMove;
            cin >> inputMove;

            if (inputMove == "undo") {
                if (moveHistory.size() >= 2) {
                    UndoInfo botUndo = moveHistory.top(); moveHistory.pop();
                    pos.undoMove(botUndo.move, botUndo);
                    pos.changeTurn();

                    UndoInfo humanUndo = moveHistory.top(); moveHistory.pop();
                    pos.undoMove(humanUndo.move, humanUndo);
                    pos.changeTurn();

                    lastMove = moveHistory.empty() ? Move() : moveHistory.top().move;
                    if (fullMoveNumber > 1) fullMoveNumber--;
                }
                else {
                    cout << "\033[1;31mNo moves to undo.\033[0m\n";
                    this_thread::sleep_for(chrono::milliseconds(800));
                }
                continue;
            }

            Move humanMove = uciToMove(inputMove);
            if (validMove(legalMoves, humanMove)) {
                UndoInfo undoData = pos.movePiece(humanMove);
                moveHistory.push(undoData);
                lastMove = humanMove;
                if (pos.getTurn() == BLACK) fullMoveNumber++;
                pos.changeTurn();
            }
            else {
                cout << "\033[1;31mInvalid move. Try again.\033[0m\n";
                this_thread::sleep_for(chrono::milliseconds(800));
            }
        }
        else {
            // Bot's turn
            string side = (pos.getTurn() == WHITE ? "White" : "Black");
            cout << "Bot (" << side << ") is thinking...\n";

            auto start = steady_clock::now();
            MinimaxValue result = pos.iterativeDeepening(maxDepth, timeLimitMs);
            auto end_time = steady_clock::now();
            double elapsed = duration<double>(end_time - start).count();

            cout << "\n\033[1;32m>>> " << side << " plays: "
                 << result._move.toString() << " <<<\033[0m"
                 << "  (" << fixed << setprecision(2) << elapsed << "s)\n";

            float eval = pos.evaluate();
            cout << "Eval: ";
            if (eval > 1.0f)
                cout << "\033[1;97mWhite\033[0m +" << fixed << setprecision(1) << eval;
            else if (eval < -1.0f)
                cout << "\033[1;96mBlack\033[0m +" << fixed << setprecision(1) << -eval;
            else
                cout << "Equal";
            cout << "\n";

            UndoInfo undoData = pos.movePiece(result._move);
            moveHistory.push(undoData);
            lastMove = result._move;
            if (pos.getTurn() == BLACK) fullMoveNumber++;
            pos.changeTurn();

            this_thread::sleep_for(chrono::milliseconds(600));
        }
    }

    return 0;
}
