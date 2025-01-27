#ifndef MOVE_H
#define MOVE_H

#include <string>

using namespace std;

class Move {
public:
    int startRow, startCol, endRow, endCol;

    // Constructor to initialize move
    Move(int startRow, int startCol, int endRow, int endCol);

    // Utility method to return move as a string
    string toString() const;
};

#endif