#include <string>
#include <iostream>
#include "Move.h"
#include "chess.h"

using namespace std;

// Constructor to initialize move
Move::Move(int startRow, int startCol, int endRow, int endCol)
    : startRow(startRow), startCol(startCol), endRow(endRow), endCol(endCol), promotion(-1) {}

Move::Move(int startRow, int startCol, int endRow, int endCol, int promotion)
    : startRow(startRow), startCol(startCol), endRow(endRow), endCol(endCol), promotion(promotion) {}

string Move::toString() const {
    char startFile = 'a' + startCol;
    char endFile = 'a' + endCol;
    int startRank = 8 - startRow;
    int endRank = 8 - endRow;

    // Base move notation
    string moveStr = string(1, startFile) + to_string(startRank) +
        string(1, endFile) + to_string(endRank);

    // Add promotion if it exists
    if (promotion != -1) {
        moveStr += pieceIndicator(promotion);
    }

    return moveStr;
}
