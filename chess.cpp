#include "position.h"
#include "chess.h"
#include <iostream>
#include <utility>
#include <vector>
#include <string>

using namespace std;

string pieceIndicator(int piece) {
    const char* pieceIndicators[] = {
    "R", "N", "B", "Q", "K", "P",
    "r", "n", "b", "q", "k", "p",
    " "
    };
    return pieceIndicators[piece];
}

Move uciToMove(const string& moveStr) {
    auto getCoords = [](char file, char rank) -> pair<int, int> {
        int row = 8 - (rank - '0');  
        int col = file - 'a';        
        return { row, col };
        };

    pair<int, int> from = getCoords(moveStr[0], moveStr[1]);
    pair<int, int> to = getCoords(moveStr[2], moveStr[3]);

    int promotedPiece = NA;

    if (moveStr.length() == 5) {
        char promoChar = moveStr[4];
        switch (promoChar) {
        case 'Q': promotedPiece = wQ; break;
        case 'R': promotedPiece = wR; break;
        case 'B': promotedPiece = wB; break;
        case 'N': promotedPiece = wN; break;
        case 'q': promotedPiece = bQ; break;
        case 'r': promotedPiece = bR; break;
        case 'b': promotedPiece = bB; break;
        case 'n': promotedPiece = bN; break;
        default:
            break;
        }
    }

    return Move(from.first, from.second, to.first, to.second, promotedPiece);
}

bool validMove(const vector<Move>& allMoves, Move playerMove) {
    for (const auto& move : allMoves) {
        if (move.startRow == playerMove.startRow && move.startCol == playerMove.startCol && move.endRow == playerMove.endRow && move.endCol == playerMove.endCol) {
            return true; // Return true if a matching move is found
        }
    }
    return false; // Return false if no matching move is found
}

int opponent(int player) {
    if (player == WHITE) {
        return BLACK;
    }
    return WHITE;
}
