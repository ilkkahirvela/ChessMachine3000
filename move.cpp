#include <string>
#include <iostream>
#include "move.h"
#include "chess.h"

using namespace std;

Move::Move() : startRow(-1), startCol(-1), endRow(-1), endCol(-1), promotion(-1) {}

Move::Move(int startRow, int startCol, int endRow, int endCol)
    : startRow(startRow), startCol(startCol), endRow(endRow), endCol(endCol), promotion(-1) {
}

Move::Move(int startRow, int startCol, int endRow, int endCol, int promotion)
    : startRow(startRow), startCol(startCol), endRow(endRow), endCol(endCol), promotion(promotion) {
}

string Move::toString() const {
    // Convert board coordinates to standard chess notation.
    char startFile = static_cast<char>('a' + startCol);
    char endFile = static_cast<char>('a' + endCol);
    int startRank = 8 - startRow;
    int endRank = 8 - endRow;

    // Base move notation (e.g., "e2e4")
    string moveStr = string(1, startFile) + to_string(startRank) +
        string(1, endFile) + to_string(endRank);

    // Append promotion piece indicator if promotion is specified.
    if (promotion != -1) {
        moveStr += pieceIndicator(promotion);
    }

    return moveStr;
}
