#include "Move.h"
#include <string>
#include <iostream>

// Constructor to initialize move
Move::Move(int startRow, int startCol, int endRow, int endCol)
    : startRow(startRow), startCol(startCol), endRow(endRow), endCol(endCol) {
}

// Utility method to return move as a string
string Move::toString() const {
    return to_string(startRow) + "," + to_string(startCol) + " -> " +
        to_string(endRow) + "," + to_string(endCol);
}
