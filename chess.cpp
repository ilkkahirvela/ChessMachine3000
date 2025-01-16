#include "position.h"
#include "chess.h"
#include <iostream>

using namespace std;

string pieceSymbol(int piece) {
    switch (piece) {
    case wR: return "wR";
    case wN: return "wN";
    case wB: return "wB";
    case wQ: return "wQ";
    case wK: return "wK";
    case wP: return "wP";
    case bR: return "bR";
    case bN: return "bN";
    case bB: return "bB";
    case bQ: return "bQ";
    case bK: return "bK";
    case bP: return "bP";
    case NA: return "NA";
    default: return "??";
    }
}
