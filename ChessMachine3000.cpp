#include "chess.h"
#include "position.h"
#include <iostream>

using namespace std;

int main()
{
    Position position;

    position.printBoard();

    string move = "e2e4";
    auto coords = UciToCoords(move);

    position.movePiece(coords.first.first, coords.first.second, coords.second.first, coords.second.second);
    position.printBoard();

    //// Kuninkaan sijainti
    //int row, column;

    //position.findKing(wK, row, column);

    //if (row != -1 && column != -1) {
    //    cout << "Valkoinen kuningas sijainnissa: (" << row << ", " << column << ")";
    //}

    return 0;
}
