#pragma once

using namespace std;

// Struct to represent a move in the chess game
struct Move {
    int startRow;
    int startCol;
    int endRow;
    int endCol;

    // Constructor for easy initialization
    Move(int sr, int sc, int er, int ec) : startRow(sr), startCol(sc), endRow(er), endCol(ec) {}

    // Utility method to return move as a string
    string toString() const {
        return to_string(startRow) + "," + to_string(startCol) + " -> " +
            to_string(endRow) + "," + to_string(endCol);
    }
};
