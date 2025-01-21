#include "position.h"
#include "chess.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

int Position::pieceColor(int row, int column) const {
    int piece = _board[row][column];

    if (piece >= wR && piece <= wP) {
        return WHITE;
    }
    if (piece >= bR && piece <= bP) {
        return BLACK;
    }
    return -1;  // Empty square or invalid piece
}

void Position::movePiece(int startR, int startC, int endR, int endC) {
    int piece = _board[startR][startC];
    _board[startR][startC] = NA;
    _board[endR][endC] = piece;
}

bool Position::isBlocked(int row, int column) {
    int piece = _board[row][column];
    int player = _moveturn;

    if (pieceColor(row, column) == -1) { // Empty square
        return false;
    }
    else if (pieceColor(row, column) == player) { // Friendly piece
        return true;
    }
    else { // Enemy piece
        return true;  // Block after first enemy piece
    }
}

vector<int> Position::getRookMoves(int row, int column) {
    vector<int> moves;

    // Move up
    for (int r = row - 1; r >= 0; --r) {
        if (isBlocked(r, column)) {
            if (pieceColor(r, column) != _moveturn && pieceColor(r, column) != -1) {
                moves.push_back(r * 10 + column); // Capture the first enemy piece
            }
            break;
        }
        moves.push_back(r * 10 + column);
    }

    // Move down
    for (int r = row + 1; r < 8; ++r) {
        if (isBlocked(r, column)) {
            if (pieceColor(r, column) != _moveturn && pieceColor(r, column) != -1) {
                moves.push_back(r * 10 + column); // Capture the first enemy piece
            }
            break;
        }
        moves.push_back(r * 10 + column);
    }

    // Move left
    for (int c = column - 1; c >= 0; --c) {
        if (isBlocked(row, c)) {
            if (pieceColor(row, c) != _moveturn && pieceColor(row, c) != -1) {
                moves.push_back(row * 10 + c); // Capture the first enemy piece
            }
            break;
        }
        moves.push_back(row * 10 + c);
    }

    // Move right
    for (int c = column + 1; c < 8; ++c) {
        if (isBlocked(row, c)) {
            if (pieceColor(row, c) != _moveturn && pieceColor(row, c) != -1) {
                moves.push_back(row * 10 + c); // Capture the first enemy piece
            }
            break;
        }
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
