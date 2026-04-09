/**
 * @file main.cpp
 * @brief Main program for chess with human vs bot and bot vs bot game modes.
 *
 * At startup, the user selects the game mode:
 * - Human vs Bot: The human chooses a color and plays against the bot.
 * - Bot vs Bot:   Both sides are controlled by bots.
 *
 * The program uses iterative deepening minimax search with alpha-beta pruning.
 */

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <chrono>
#include <stack>
#include <algorithm>
#include <thread>
#include <unordered_map>
#include <conio.h>
#include "chess.h"
#include "move.h"
#include "position.h"

using namespace std;
using namespace std::chrono;

int main() {
    bool humanMode      = true;
    int  humanSide      = WHITE;
    int  timeLimitMs    = 3000;
    bool showLegalMoves = true;

    while (true) {  // outer menu loop

        // --- Settings screen ---
        while (true) {
            cout << "\033[2J\033[H";
            cout << "\033[1m=== ChessMachine3000 ===\033[0m\n\n";
            cout << "  1   Game mode        :  " << (humanMode ? "Human vs Bot" : "Bot vs Bot") << "\n";
            cout << "  2   Play as          :  " << (humanMode ? (humanSide == WHITE ? "White" : "Black") : "--") << "\n";
            cout << "  3   Bot think time   :  " << (timeLimitMs / 1000) << "s  (1-5)\n";
            cout << "  4   Show legal moves :  " << (showLegalMoves ? "Yes" : "No") << "\n";
            cout << "\n  \033[33mPress 1-4 to change  |  Enter to start  |  Q to quit\033[0m  ";

            int ch = _getch();

            if (ch == '\r' || ch == '\n') break;
            if (ch == 'q'  || ch == 'Q')  return 0;
            if (ch == 0    || ch == 224)  { _getch(); continue; }  // skip special/arrow keys

            if (ch == '1') {
                humanMode = !humanMode;
            }
            else if (ch == '2' && humanMode) {
                humanSide = (humanSide == WHITE ? BLACK : WHITE);
            }
            else if (ch == '3') {
                timeLimitMs = (timeLimitMs % 5000) + 1000;  // cycle 1s → 2s → ... → 5s → 1s
            }
            else if (ch == '4') {
                showLegalMoves = !showLegalMoves;
            }
        }

        // --- Game setup ---
        Position pos;
        const int maxDepth = 10;
        stack<UndoInfo> moveHistory;
        unordered_map<string, int> positionHistory;
        Move lastMove;
        int fullMoveNumber = 1;
        bool quitToMenu = false;

        // --- Game loop ---
        while (true) {
            // Compute status before printing header
            int kingPiece  = (pos.getTurn() == WHITE ? wK : bK);
            int attackSide = (pos.getTurn() == WHITE ? BLACK : WHITE);
            int ckRow, ckCol;
            pos.findKing(kingPiece, ckRow, ckCol);
            bool inCheck = pos.isSquareUnderAttack(ckRow, ckCol, attackSide);
            int material = pos.getMaterialBalance();

            // Header
            cout << "\033[2J\033[H";
            cout << "\033[1m=== ChessMachine3000 ===\033[0m";
            cout << "  Move " << fullMoveNumber;
            cout << "  |  \033[1m" << (pos.getTurn() == WHITE ? "White" : "Black") << " to play\033[0m";
            if (inCheck) cout << "  \033[1;31mCHECK!\033[0m";
            if (lastMove.startRow != -1)
                cout << "  |  Last: \033[1;33m" << lastMove.toString() << "\033[0m";
            cout << "\n";

            cout << "Material: ";
            if      (material > 0) cout << "\033[1;33mWhite +" << material << "\033[0m";
            else if (material < 0) cout << "\033[1;96mBlack +" << -material << "\033[0m";
            else                   cout << "Equal";
            int halfClock = pos.getHalfMoveClock();
            cout << "  |  50-move: " << halfClock << "/100";
            if (halfClock >= 80) cout << "  \033[1;31m(!)\033[0m";
            cout << "\n\n";

            pos.printBoard(lastMove);

            // Generate legal moves
            vector<Move> allMoves, legalMoves;
            pos.getAllMoves(pos.getTurn(), allMoves);
            pos.getLegalMoves(allMoves, legalMoves);

            // Draw checks
            if (pos.getHalfMoveClock() >= 100) {
                cout << "\033[1;33mDraw by 50-move rule!\033[0m\n";
                break;
            }
            string posKey = pos.getPositionKey();
            if (++positionHistory[posKey] >= 3) {
                cout << "\033[1;33mDraw by threefold repetition!\033[0m\n";
                break;
            }

            // Checkmate / stalemate
            if (legalMoves.empty()) {
                int mRow, mCol;
                pos.findKing(kingPiece, mRow, mCol);
                if (pos.isSquareUnderAttack(mRow, mCol, attackSide)) {
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
                if (showLegalMoves) {
                    cout << "Legal moves (" << legalMoves.size() << "): ";
                    for (const auto& move : legalMoves)
                        cout << move.toString() << " ";
                    cout << "\n";
                }
                cout << "\n\033[33mYour move (UCI, 'undo', or 'quit'): \033[0m";
                string inputMove;
                getline(cin, inputMove);

                if (inputMove == "quit" || inputMove == "q") {
                    quitToMenu = true;
                    break;
                }

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
                if      (eval > 1.0f)  cout << "\033[1;33mWhite\033[0m +" << fixed << setprecision(1) << eval;
                else if (eval < -1.0f) cout << "\033[1;96mBlack\033[0m +" << fixed << setprecision(1) << -eval;
                else                   cout << "Equal";
                cout << "\n";

                UndoInfo undoData = pos.movePiece(result._move);
                moveHistory.push(undoData);
                lastMove = result._move;
                if (pos.getTurn() == BLACK) fullMoveNumber++;
                pos.changeTurn();
            }
        }

        if (!quitToMenu) {
            cout << "\nPress Enter to return to menu...";
            string dummy;
            getline(cin, dummy);
        }
    }

    return 0;
}
