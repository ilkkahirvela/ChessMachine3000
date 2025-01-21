#include "position.h"
#include "chess.h"
#include <iostream>
#include <iomanip>

using namespace std;

void Position::emptyBoard() {
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			_board[r][c] = NA;
		}
	}
}

void Position::printBoard() const {
    const int boardSize = 8;

    // Print the file labels
    cout << "    ";
    for (char file = 'a'; file <= 'h'; ++file) {
        cout << file << "   ";  // Spacing between file labels
    }
    cout << endl;

    // Print the top border line
    cout << "  +";
    for (int i = 0; i < boardSize; i++) {
        cout << "----";
    }
    cout << "+" << endl;

    // Print the board rows
    for (int r = 0; r < boardSize; r++) {
        cout << 8 - r << " | ";

        for (int c = 0; c < boardSize; c++) {
            // Print each piece with fixed width
            cout << setw(2) << pieceIndicator(_board[r][c]) << " |";
        }

        cout << endl;

        // Add horizontal line between rows
        if (r < boardSize - 1) {
            cout << "  +";
            for (int i = 0; i < boardSize; i++) {
                cout << "----";
            }
            cout << "+" << endl;
        }
    }

    // Bottom border line
    cout << "  +";
    for (int i = 0; i < boardSize; i++) {
        cout << "----";
    }
    cout << "+" << endl;
}

void Position::findKing(int piece, int& row, int& column) const
{
    // Etsitään kuningasta koko laudalta
    for (int r = 0; r < 8; ++r)
    {
        for (int c = 0; c < 8; ++c)
        {
            if (_board[r][c] == piece)
            {
                row = r;
                column = c;
                return;
            }
        }
    }

    // Jos kuningasta ei löydy
    row = -1;
    column = -1;
}

