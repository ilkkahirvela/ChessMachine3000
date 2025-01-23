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

pair<pair<int, int>, pair<int, int>> uciToCoords(const string& move) {
    auto getCoords = [](char file, char rank) -> pair<int, int> {
        int row = 8 - (rank - '0');
        int col = file - 'a';
        return { row, col };
        };

    pair<int, int> from = getCoords(move[0], move[1]);
    pair<int, int> to = getCoords(move[2], move[3]);

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

// Kertoo pelaajan vastustajan värin
int opponent(int player) {
    if (player == WHITE) {
        return BLACK;
    }
    return WHITE;
}
