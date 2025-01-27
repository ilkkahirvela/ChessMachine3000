#include "Move.h"
#include <string>
#include <iostream>

using namespace std;

// Constructor to initialize move
Move::Move(int startRow, int startCol, int endRow, int endCol)
    : startRow(startRow), startCol(startCol), endRow(endRow), endCol(endCol) {
}

string Move::toString() const {
    char startFile = 'a' + startCol;
    char endFile = 'a' + endCol;
    int startRank = 8 - startRow;
    int endRank = 8 - endRow;
    return string(1, startFile) + to_string(startRank) +
        string(1, endFile) + to_string(endRank);
}
