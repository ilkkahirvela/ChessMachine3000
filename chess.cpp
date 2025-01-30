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

pair<pair<int, int>, pair<int, int>> uciToCoords(const string& move, int& promotedPiece) {
    auto getCoords = [](char file, char rank) -> pair<int, int> {
        int row = 8 - (rank - '0');
        int col = file - 'a';
        return { row, col };
        };

    pair<int, int> from = getCoords(move[0], move[1]);
    pair<int, int> to = getCoords(move[2], move[3]);

    // Handle promotion
    promotedPiece = NA; // Default to no promotion
    if (move.length() == 5) {
        char promoChar = move[4];
        switch (promoChar) {
        case 'Q': promotedPiece = wQ; break;
        case 'R': promotedPiece = wR; break;
        case 'B': promotedPiece = wB; break;
        case 'N': promotedPiece = wN; break;
        case 'q': promotedPiece = bQ; break;
        case 'r': promotedPiece = bR; break;
        case 'b': promotedPiece = bB; break;
        case 'n': promotedPiece = bN; break;
        default: break; // Invalid promotion character make an error handler <----!!!!
        }
    }

    return { from, to };
}

string coordsToUci(const vector<Move>& moves) {
    auto getMove = [](int row, int col) -> string {
        char file = col + 'a';
        char rank = '8' - row;
        return { file, rank };
        };

    string uciMoves = "";
    for (const auto& move : moves) {
        string from = getMove(move.startRow, move.startCol);
        string to = getMove(move.endRow, move.endCol);

        uciMoves += from + to + " ";
    }

    return uciMoves;
}

bool validMove(const vector<Move>& allMoves, int startRow, int startCol, int endRow, int endCol) {
    for (const auto& move : allMoves) {
        if (move.startRow == startRow && move.startCol == startCol && move.endRow == endRow && move.endCol == endCol) {
            return true; // Return true if a matching move is found
        }
    }
    return false; // Return false if no matching move is found
}

// Kertoo pelaajan vastustajan värin
int opponent(int player) {
    if (player == WHITE) {
        return BLACK;
    }
    return WHITE;
}
