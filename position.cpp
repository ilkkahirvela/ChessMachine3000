#include "position.h"
#include "chess.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

void Position::changeTurn() {
    if (_moveturn == WHITE) {
        _moveturn = BLACK;
    }
    else {
        _moveturn = WHITE;
    }
}

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

void Position::movePiece(int startR, int startC, int endR, int endC, int promotedPiece) {
    int piece = _board[startR][startC];
    _board[startR][startC] = NA;

    // Check if the move is a pawn promotion
    if ((piece == wP && endR == 0) || (piece == bP && endR == 7)) {
        // If promotedPiece is not specified, default to queen
        if (promotedPiece == NA) {
            promotedPiece = (piece == wP) ? wQ : bQ;
        }
        _board[endR][endC] = promotedPiece;
    }
    else {
        _board[endR][endC] = piece;
    }
}

void Position::getDirectionalMoves(int row, int column, const vector<pair<int, int>>& directions, vector<Move>& moves) const {
    for (const auto& dir : directions) {
        int r = row + dir.first;
        int c = column + dir.second;

        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            int piece = pieceColor(r, c);

            if (piece != -1) {  // There's a piece at this position, not empty square
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
}

void Position::getRookMoves(int row, int column, vector<Move>& moves) const {
    static const vector<pair<int, int>> rookDirections = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    getDirectionalMoves(row, column, rookDirections, moves);
}

void Position::getBishopMoves(int row, int column, vector<Move>& moves) const {
    static const vector<pair<int, int>> bishopDirections = {
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };
    getDirectionalMoves(row, column, bishopDirections, moves);
}

void Position::getQueenMoves(int row, int column, vector<Move>& moves) const {
    static const vector<pair<int, int>> queenDirections = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };
    getDirectionalMoves(row, column, queenDirections, moves);
}

void Position::getKingMoves(int row, int column, vector<Move>& moves) const {
    static const vector<pair<int, int>> kingMoves = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
    };

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
}

void Position::getKnightMoves(int row, int column, vector<Move>& moves) const {
    static const vector<pair<int, int>> knightMoves = {
        {-2, -1}, {-2, 1}, {2, -1}, {2, 1},
        {-1, -2}, {-1, 2}, {1, -2}, {1, 2}
    };

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
}

void Position::getPawnMoves(int row, int column, int piece, vector<Move>& moves, int player) const {
    int direction = (piece == wP) ? -1 : 1; // White pawns move up (-1), black pawns move down (+1)
    int startRow = (piece == wP) ? 6 : 1;
    int promotionRow = (piece == wP) ? 0 : 7; 

    // Single forward move
    int forwardRow = row + direction;
    if (forwardRow >= 0 && forwardRow < 8 && _board[forwardRow][column] == NA) {
        // Promotion handling
        if (forwardRow == promotionRow) {
            if (player == WHITE) {
                for (const auto& promotion : whitePromotions) {
                    moves.push_back(Move(row, column, forwardRow, column, promotion));
                }
            }
            else if (player == BLACK) {
                for (const auto& promotion : blackPromotions) {
                    moves.push_back(Move(row, column, forwardRow, column, promotion));
                }
            }
        }
        // No promotion available
        else {
            moves.push_back(Move(row, column, forwardRow, column));

            // Double forward move from the starting position
            int doubleForwardRow = row + 2 * direction;
            if (row == startRow && _board[doubleForwardRow][column] == NA) {
                moves.push_back(Move(row, column, doubleForwardRow, column));
            }
        }
    }

    // Capturing diagonally
    for (int dc : {-1, 1}) {  // Diagonal directions
        int captureRow = row + direction;
        int captureCol = column + dc;
        if (captureRow >= 0 && captureRow < 8 && captureCol >= 0 && captureCol < 8) {
            int targetPieceColor = pieceColor(captureRow, captureCol);
            if (targetPieceColor != -1 && targetPieceColor != _moveturn) {
                moves.push_back(Move(row, column, captureRow, captureCol));
            }
        }
    }

    // En passant
    
}

void Position::getAllMoves(int player, vector<Move>& moves) const {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            int piece = _board[r][c];
            if (pieceColor(r, c) == player) {
                switch (piece) {
                case wR: case bR:
                    getRookMoves(r, c, moves);
                    break;
                case wB: case bB:
                    getBishopMoves(r, c, moves);
                    break;
                case wQ: case bQ:
                    getQueenMoves(r, c, moves);
                    break;
                case wK: case bK:
                    getKingMoves(r, c, moves);
                    break;
                case wN: case bN:
                    getKnightMoves(r, c, moves);
                    break;
                case wP: case bP:
                    getPawnMoves(r, c, piece, moves, player);
                    break;
                default:
                    break;  // Empty square
                }
            }
        }
    }
}

void Position::emptyBoard() {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            _board[r][c] = NA;
        }
    }
}

void Position::insertTestPiece(int r, int c, int piece) {
    _board[r][c] = piece;
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
