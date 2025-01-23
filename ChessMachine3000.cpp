#include "chess.h"
#include "position.h"
#include <iostream>

using namespace std;

int main()
{
    Position position;

    string move1 = "d1e5";
    auto coords1 = uciToCoords(move1);

    position.movePiece(coords1.first.first, coords1.first.second, coords1.second.first, coords1.second.second);
    position.printBoard();

    string move = "e5e6";
    auto coords = uciToCoords(move);

    int endCoords = coords.second.first * 10 + coords.second.second;

    vector<int> rookMoves = position.getQueenMoves(coords.first.first, coords.first.second);
    
    for (int move : rookMoves) {
        cout << move << " ";
    }

    string uciMoves = coordsToUci(rookMoves);

    cout << endl << "The UCI moves are: " << uciMoves << endl;

    cout << endCoords;

    return 0;
}
