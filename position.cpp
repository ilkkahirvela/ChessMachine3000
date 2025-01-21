#include "position.h"
#include "chess.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

void Position::movePiece(int startR, int startC, int endR, int endC) {
    int piece = _board[startR][startC];
    _board[startR][startC] = NA;
    _board[endR][endC] = piece;
}

bool isBlocked(int row, int column, int color) {
    if (pieceColor(row, column) == color) {
        return true;
    }
    else if (pieceColor(row, column) != color) {
        return true;
    }
    return false;
}

vector<int> Position::getRookMoves(int row, int column) {
    vector<int> moves;
    int color = _moveturn;

    for (int r = row - 1; r >= 0; --r) { // Move up
        if (isBlocked(r, column, color)) break;
        moves.push_back(r * 10 + column);
    }
    for (int r = row + 1; r < 8; ++r) { // Move down
        if (isBlocked(r, column, color)) break;
        moves.push_back(r * 10 + column);
    }

    for (int c = column - 1; c >= 0; --c) { // Move left
        if (isBlocked(row, c, color)) break;
        moves.push_back(row * 10 + c);
    }
    for (int c = column + 1; c < 8; ++c) { // Move right
        if (isBlocked(row, c, color)) break;
        moves.push_back(row * 10 + c);
    }

    return moves;
}

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
}
