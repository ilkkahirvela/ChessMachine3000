#include "position.h"
#include "chess.h"
#include <iostream>

using namespace std;

string pieceIndicator(int piece) {
    const char* pieceIndicators[] = {
    "R", "N", "B", "Q", "K", "P",
    "r", "n", "b", "q", "k", "p",
    " "
    };
    return pieceIndicators[piece];
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
