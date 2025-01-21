#include "position.h"
#include "chess.h"
#include <iostream>
#include <utility>

using namespace std;

string pieceIndicator(int piece) {
    const char* pieceIndicators[] = {
    "R", "N", "B", "Q", "K", "P",
    "r", "n", "b", "q", "k", "p",
    " "
    };
    return pieceIndicators[piece];
}

pair<pair<int, int>, pair<int, int>> UciToCoords(const string& move) {
    auto getCoords = [](char file, char rank) -> pair<int, int> {
        return { 8 - (rank - '0'), file - 'a' }; // rank to row, file to column
        };

    pair<int, int> from = getCoords(move[0], move[1]);
    pair<int, int> to = getCoords(move[2], move[3]);

    return { from, to };
}

// Kertoo nappulan värin
int pieceColor(int piece) {
    if (piece >= wR && piece <= wP) {
        return WHITE;
    }
    if (piece >= bR && piece <= bP) {
        return BLACK;
    }
    return -1;  // Invalid piece
}

// Kertoo pelaajan vastustajan värin
int opponent(int player) {
    if (player == WHITE) {
        return BLACK;
    }
    return WHITE;
}
