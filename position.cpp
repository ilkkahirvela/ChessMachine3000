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

vector<Move> Position::getDirectionalMoves(int row, int column, const vector<pair<int, int>>& directions) {
    vector<Move> moves;

    for (const auto& dir : directions) {
        int r = row + dir.first;
        int c = column + dir.second;

        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            int piece = pieceColor(r, c);

            if (piece != -1) {  // There's a piece at this position
                if (piece != _moveturn) {
                    moves.push_back(Move(row, column, r, c));  // Capture enemy piece
                }
                break;  // Stop moving in this direction
            }

            moves.push_back(Move(row, column, r, c));
            r += dir.first;
            c += dir.second;
        }
    }

    return moves;
}

vector<Move> Position::getRookMoves(int row, int column) {
    static const vector<pair<int, int>> rookDirections = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    return getDirectionalMoves(row, column, rookDirections);
}

vector<Move> Position::getBishopMoves(int row, int column) {
    static const vector<pair<int, int>> bishopDirections = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };
    return getDirectionalMoves(row, column, bishopDirections);
}

vector<Move> Position::getQueenMoves(int row, int column) {
    static const vector<pair<int, int>> queenDirections = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };
    return getDirectionalMoves(row, column, queenDirections);
}

vector<Move> Position::getKingMoves(int row, int column) {
    static const vector<pair<int, int>> kingMoves = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

    vector<Move> moves;
    for (const auto& move : kingMoves) {
        int r = row + move.first;
        int c = column + move.second;

        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            int piece = pieceColor(r, c);
            if (piece == -1 || piece != _moveturn) {
                moves.push_back(Move(row, column, r, c));
            }
        }
    }
    return moves;
}

vector<Move> Position::getKnightMoves(int row, int column) {
    static const vector<pair<int, int>> knightMoves = {
        {-2, -1}, {-2, 1}, {2, -1}, {2, 1},
        {-1, -2}, {-1, 2}, {1, -2}, {1, 2}
    };

    vector<Move> moves;
    for (const auto& move : knightMoves) {
        int r = row + move.first;
        int c = column + move.second;

        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            int piece = pieceColor(r, c);
            if (piece == -1 || piece != _moveturn) {
                moves.push_back(Move(row, column, r, c));
            }
        }
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
    cout << "    ";
    for (char file = 'a'; file <= 'h'; ++file) {
        cout << file << "   ";  // Spacing between file labels
    }
    cout << endl;

    cout << "  +";
    for (int i = 0; i < boardSize; i++) {
        cout << "----";
    }
    cout << "+" << endl;

    for (int r = 0; r < boardSize; r++) {
        cout << 8 - r << " | ";

        for (int c = 0; c < boardSize; c++) {
            cout << setw(2) << pieceIndicator(_board[r][c]) << " |";
        }

        cout << endl;

        if (r < boardSize - 1) {
            cout << "  +";
            for (int i = 0; i < boardSize; i++) {
                cout << "----";
            }
            cout << "+" << endl;
        }
    }

    cout << "  +";
    for (int i = 0; i < boardSize; i++) {
        cout << "----";
    }
    cout << "+" << endl;
}

void Position::findKing(int piece, int& row, int& column) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (_board[r][c] == piece) {
                row = r;
                column = c;
                return;
            }
        }
    }
}
