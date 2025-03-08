#include "position.h"
#include "chess.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>

using namespace std;

const vector<pair<int, int>> Position::_knightOffsets = {
    {-2, -1}, {-2, 1}, {2, -1}, {2, 1},
    {-1, -2}, {-1, 2}, {1, -2}, {1, 2}
};

const vector<pair<int, int>> Position::_rookDirections ={
    {-1, 0}, {1, 0}, {0, -1}, {0, 1}
};

const vector<pair<int, int>> Position::_bishopDirections = {
    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
};

const vector<pair<int, int>> Position::_queenDirections = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1},
    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
};

const vector<pair<int, int>> Position::_kingMoves = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1},
    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
};

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

UndoInfo Position::movePiece(Move move) {
    UndoInfo undo;
    undo.move = move;
    // Save moved piece
    undo.movedPiece = _board[move.startRow][move.startCol];
    // Save captured piece
    undo.capturedPiece = _board[move.endRow][move.endCol];
    undo.capturedPieceSquare = { move.endRow, move.endCol };
    undo.enPassantCapture = false;

    // Save castling rights and en passant square
    undo.whiteKingMoved = _whiteKingMoved;
    undo.whiteKingsideRookMoved = _whiteKingsideRookMoved;
    undo.whiteQueensideRookMoved = _whiteQueensideRookMoved;
    undo.blackKingMoved = _blackKingMoved;
    undo.blackKingsideRookMoved = _blackKingsideRookMoved;
    undo.blackQueensideRookMoved = _blackQueensideRookMoved;
    undo.enPassantSquare = _enPassantSquare;

    undo.castlingMove = false;

    int piece = undo.movedPiece;

    // Clear en passant
    _enPassantSquare = { -1, -1 };

    // Handle en passant capture
    bool isEnPassant = (piece == wP || piece == bP) &&
        (abs(move.startCol - move.endCol) == 1) &&
        (_board[move.endRow][move.endCol] == NA);
    if (isEnPassant) {
        undo.enPassantCapture = true;
        if (piece == wP) {
            undo.capturedPiece = _board[move.endRow + 1][move.endCol];
            undo.capturedPieceSquare = { move.endRow + 1, move.endCol };
            _board[move.endRow + 1][move.endCol] = NA;
        }
        else {
            undo.capturedPiece = _board[move.endRow - 1][move.endCol];
            undo.capturedPieceSquare = { move.endRow - 1, move.endCol };
            _board[move.endRow - 1][move.endCol] = NA;
        }
    }

    // Remove piece from the start
    _board[move.startRow][move.startCol] = NA;

    // Update castling flags
    if (piece == wK)
        _whiteKingMoved = true;
    else if (piece == bK)
        _blackKingMoved = true;
    else if (piece == wR) {
        if (move.startRow == 7 && move.startCol == 0)
            _whiteQueensideRookMoved = true;
        else if (move.startRow == 7 && move.startCol == 7)
            _whiteKingsideRookMoved = true;
    }
    else if (piece == bR) {
        if (move.startRow == 0 && move.startCol == 0)
            _blackQueensideRookMoved = true;
        else if (move.startRow == 0 && move.startCol == 7)
            _blackKingsideRookMoved = true;
    }

    // Handle pawn promotion
    if ((piece == wP && move.endRow == 0) || (piece == bP && move.endRow == 7)) {
        if (move.promotion == NA)
            move.promotion = (piece == wP) ? wQ : bQ;
        _board[move.endRow][move.endCol] = move.promotion;
    }
    else {
        _board[move.endRow][move.endCol] = piece;
    }

    // Handle castling move
    if ((piece == wK || piece == bK) && abs(move.startCol - move.endCol) == 2) {
        undo.castlingMove = true;
        if (move.endCol == 6) { // Kingside
            undo.rookFromRow = move.startRow;
            undo.rookFromCol = 7;
            undo.rookToRow = move.startRow;
            undo.rookToCol = 5;
            _board[move.startRow][7] = NA;
            _board[move.startRow][5] = (piece == wK) ? wR : bR;
        }
        else if (move.endCol == 2) { // Queenside
            undo.rookFromRow = move.startRow;
            undo.rookFromCol = 0;
            undo.rookToRow = move.startRow;
            undo.rookToCol = 3;
            _board[move.startRow][0] = NA;
            _board[move.startRow][3] = (piece == wK) ? wR : bR;
        }
    }

    return undo;
}

void Position::undoMove(Move move, const UndoInfo& undo) {
    int piece = undo.movedPiece;

    // Remove moved piece from destination and restore it to the start
    _board[move.endRow][move.endCol] = NA;
    _board[move.startRow][move.startCol] = piece;

    // Restore captured piece
    if (undo.capturedPiece != NA) {
        _board[undo.capturedPieceSquare.first][undo.capturedPieceSquare.second] = undo.capturedPiece;
    }

    // Undo castling by moving the rook back.
    if (undo.castlingMove) {
        _board[undo.rookFromRow][undo.rookFromCol] = _board[undo.rookToRow][undo.rookToCol];
        _board[undo.rookToRow][undo.rookToCol] = NA;
    }

    // Restore castling rights and en passant square.
    _whiteKingMoved = undo.whiteKingMoved;
    _whiteKingsideRookMoved = undo.whiteKingsideRookMoved;
    _whiteQueensideRookMoved = undo.whiteQueensideRookMoved;
    _blackKingMoved = undo.blackKingMoved;
    _blackKingsideRookMoved = undo.blackKingsideRookMoved;
    _blackQueensideRookMoved = undo.blackQueensideRookMoved;
    _enPassantSquare = undo.enPassantSquare;
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
    getDirectionalMoves(row, column, _rookDirections, moves);
}

void Position::getBishopMoves(int row, int column, vector<Move>& moves) const {
    getDirectionalMoves(row, column, _bishopDirections, moves);
}

void Position::getQueenMoves(int row, int column, vector<Move>& moves) const {
    getDirectionalMoves(row, column, _queenDirections, moves);
}

void Position::getKingMoves(int row, int column, vector<Move>& moves) const {
    // Normal king moves
    for (const auto& offset : _kingMoves) {
        int r = row + offset.first;
        int c = column + offset.second;
        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            int p = pieceColor(r, c);
            if (p == -1 || p != _moveturn)
                moves.push_back(Move(row, column, r, c));
        }
    }

    // Castling check function
    auto canCastle = [&](int row, int kingCol, const vector<int>& path, int rookCol, int opponent) -> bool {
        // Ensure the rook is present and hasn't moved
        if (_board[row][rookCol] != ((_moveturn == WHITE) ? wR : bR))
            return false;
        // Ensure the path between king and rook is clear
        for (int col : path)
            if (_board[row][col] != NA)
                return false;
        // Ensure king's path is not under attack
        for (int col : { kingCol, path.back() })
            if (isSquareUnderAttack(row, col, opponent))
                return false;
        return true;
        };

    // Castling moves
    if (_moveturn == WHITE && !_whiteKingMoved) {
        if (!_whiteKingsideRookMoved && canCastle(7, 4, { 5, 6 }, 7, BLACK))
            moves.push_back(Move(7, 4, 7, 6));
        if (!_whiteQueensideRookMoved && canCastle(7, 4, { 1, 2, 3 }, 0, BLACK))
            moves.push_back(Move(7, 4, 7, 2));
    }
    else if (_moveturn == BLACK && !_blackKingMoved) {
        if (!_blackKingsideRookMoved && canCastle(0, 4, { 5, 6 }, 7, WHITE))
            moves.push_back(Move(0, 4, 0, 6));
        if (!_blackQueensideRookMoved && canCastle(0, 4, { 1, 2, 3 }, 0, WHITE))
            moves.push_back(Move(0, 4, 0, 2));
    }
}

void Position::getKnightMoves(int row, int column, vector<Move>& moves) const {
    for (const auto& move : _knightOffsets) {
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
    int direction = (piece == wP) ? -1 : 1; // White moves up (-1), Black moves down (+1)
    int startRow = (piece == wP) ? 6 : 1;
    int promotionRow = (piece == wP) ? 0 : 7;
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
        // Normal move
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
    for (int dc : {-1, 1}) {
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
    if (_enPassantSquare.first != -1) {
        // For white pawn capturing en passant:
        if (player == WHITE && row == 3) {
            for (int dc : {-1, 1}) {
                int targetCol = column + dc;
                if (targetCol >= 0 && targetCol < 8) {
                    if (_enPassantSquare.first == row - 1 && _enPassantSquare.second == targetCol) {
                        moves.push_back(Move(row, column, row - 1, targetCol));
                    }
                }
            }
        }
        // For black pawn capturing en passant:
        else if (player == BLACK && row == 4) {
            for (int dc : {-1, 1}) {
                int targetCol = column + dc;
                if (targetCol >= 0 && targetCol < 8) {
                    if (_enPassantSquare.first == row + 1 && _enPassantSquare.second == targetCol) {
                        moves.push_back(Move(row, column, row + 1, targetCol));
                    }
                }
            }
        }
    }
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

void Position::getLegalMoves(vector<Move> allMoves, vector<Move>& legalMoves) const {
    int kingRow, kingCol;
    int player = _moveturn; // Store the moving player's color
    int opponent = (player == WHITE ? BLACK : WHITE);

    for (Move& move : allMoves) {
        Position posCopy = *this;

        posCopy.movePiece(move);
        posCopy.findKing(player == WHITE ? wK : bK, kingRow, kingCol);
        if (!posCopy.isSquareUnderAttack(kingRow, kingCol, opponent)) {
            legalMoves.push_back(move); // The move is legal
        }
    }
}

bool Position::isSquareUnderAttack(int row, int col, int opponent) const {
    // Pawn attacks
    if (opponent == WHITE) {
        if ((row - 1 >= 0 && col - 1 >= 0 && _board[row - 1][col - 1] == wP) ||
            (row - 1 >= 0 && col + 1 < 8 && _board[row - 1][col + 1] == wP)) {
            return true;
        }
    }
    else {
        if ((row + 1 < 8 && col - 1 >= 0 && _board[row + 1][col - 1] == bP) ||
            (row + 1 < 8 && col + 1 < 8 && _board[row + 1][col + 1] == bP)) {
            return true;
        }
    }

    // Knight attacks
    for (const auto& offset : _knightOffsets) {
        int r = row + offset.first, c = col + offset.second;
        if (r >= 0 && r < 8 && c >= 0 && c < 8 && _board[r][c] == (opponent == WHITE ? wN : bN)) {
            return true;
        }
    }

    // King attacks
    for (const auto& move : _kingMoves) {
        int r = row + move.first, c = col + move.second;
        if (r >= 0 && r < 8 && c >= 0 && c < 8 && _board[r][c] == (opponent == WHITE ? wK : bK)) {
            return true;
        }
    }

    // Rook, Bishop, Queen
    for (const auto& dir : _queenDirections) {
        int r = row + dir.first, c = col + dir.second;
        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            int piece = _board[r][c];
            if (piece != NA) {
                if ((dir == _rookDirections[0] || dir == _rookDirections[1] ||
                    dir == _rookDirections[2] || dir == _rookDirections[3]) &&
                    (piece == (opponent == WHITE ? wR : bR) || piece == (opponent == WHITE ? wQ : bQ))) {
                    return true;
                }
                if ((dir == _bishopDirections[0] || dir == _bishopDirections[1] ||
                    dir == _bishopDirections[2] || dir == _bishopDirections[3]) &&
                    (piece == (opponent == WHITE ? wB : bB) || piece == (opponent == WHITE ? wQ : bQ))) {
                    return true;
                }
                break; // Blocked path
            }
            r += dir.first;
            c += dir.second;
        }
    }

    return false;
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

float Position::evaluate() const {
    float score = 0.0f;

    // Piece-square tables for White, Black's values are mirrored vertically
    // Values could be fine-tuned
    static const float pawnTable[8][8] = {
        { 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f},
        { 5.0f,  5.0f,  5.0f,  5.0f,  5.0f,  5.0f,  5.0f,  5.0f},
        { 1.0f,  1.0f,  2.0f,  3.0f,  3.0f,  2.0f,  1.0f,  1.0f},
        {0.5f,  0.5f,  1.0f, 2.5f, 2.5f,  1.0f, 0.5f, 0.5f},
        { 0.0f,  0.0f,  0.0f,  2.0f,  2.0f,  0.0f,  0.0f,  0.0f},
        {0.5f, -0.5f, -1.0f,  0.0f,  0.0f, -1.0f, -0.5f, 0.5f},
        {0.5f,  1.0f,  1.0f, -2.0f, -2.0f,  1.0f,  1.0f, 0.5f},
        { 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f}
    };

    static const float knightTable[8][8] = {
        {-5.0f, -4.0f, -3.0f, -3.0f, -3.0f, -3.0f, -4.0f, -5.0f},
        {-4.0f, -2.0f,  0.0f,  0.0f,  0.0f,  0.0f, -2.0f, -4.0f},
        {-3.0f,  0.0f,  1.0f,  1.5f,  1.5f,  1.0f,  0.0f, -3.0f},
        {-3.0f,  0.5f,  1.5f,  2.0f,  2.0f,  1.5f,  0.5f, -3.0f},
        {-3.0f,  0.5f,  1.5f,  2.0f,  2.0f,  1.5f,  0.5f, -3.0f},
        {-3.0f,  0.0f,  1.0f,  1.5f,  1.5f,  1.0f,  0.0f, -3.0f},
        {-4.0f, -2.0f,  0.0f,  0.5f,  0.5f,  0.0f, -2.0f, -4.0f},
        {-5.0f, -4.0f, -3.0f, -3.0f, -3.0f, -3.0f, -4.0f, -5.0f}
    };

    static const float bishopTable[8][8] = {
        {-2.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -2.0f},
        {-1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f},
        {-1.0f,  0.0f,  0.5f,  1.0f,  1.0f,  0.5f,  0.0f, -1.0f},
        {-1.0f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,  0.5f, -1.0f},
        {-1.0f,  0.5f,  1.0f,  1.0f,  1.0f,  1.0f,  0.5f, -1.0f},
        {-1.0f,  0.0f,  0.5f,  1.0f,  1.0f,  0.5f,  0.0f, -1.0f},
        {-1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f},
        {-2.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -2.0f}
    };

    static const float rookTable[8][8] = {
        { 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f},
        {0.5f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, 0.5f},
        {-0.5f, 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,-0.5f},
        {-0.5f, 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,-0.5f},
        {-0.5f, 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,-0.5f},
        {-0.5f, 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,-0.5f},
        {-0.5f, 0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,-0.5f},
        { 0.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.0f,  0.0f,  0.0f}
    };

    static const float queenTable[8][8] = {
        {-2.0f, -1.0f, -1.0f, -0.5f, -0.5f, -1.0f, -1.0f, -2.0f},
        {-1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f},
        {-1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.5f,  0.0f, -1.0f},
        {-0.5f,  0.0f,  0.5f,  0.5f,  0.5f,  0.5f,  0.0f, -0.5f},
        { 0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.5f,  0.0f, -0.5f},
        {-1.0f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.0f, -1.0f},
        {-1.0f,  0.0f,  0.5f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f},
        {-2.0f, -1.0f, -1.0f, -0.5f, -0.5f, -1.0f, -1.0f, -2.0f}
    };

    static const float kingTable[8][8] = {
        {-3.0f, -4.0f, -4.0f, -5.0f, -5.0f, -4.0f, -4.0f, -3.0f},
        {-3.0f, -4.0f, -4.0f, -5.0f, -5.0f, -4.0f, -4.0f, -3.0f},
        {-3.0f, -4.0f, -4.0f, -5.0f, -5.0f, -4.0f, -4.0f, -3.0f},
        {-3.0f, -4.0f, -4.0f, -5.0f, -5.0f, -4.0f, -4.0f, -3.0f},
        {-2.0f, -3.0f, -3.0f, -4.0f, -4.0f, -3.0f, -3.0f, -2.0f},
        {-1.0f, -2.0f, -2.0f, -2.0f, -2.0f, -2.0f, -2.0f, -1.0f},
        { 2.0f,  2.0f,  0.0f,  0.0f,  0.0f,  0.0f,  2.0f,  2.0f},
        { 2.0f,  3.0f,  1.0f,  0.0f,  0.0f,  1.0f,  3.0f,  2.0f}
    };

    // Iterate through board squares and accumulate score
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            switch (_board[r][c]) {
            case wP:
                score += 1.0f + pawnTable[r][c];
                break;
            case wN:
                score += 3.0f + knightTable[r][c];
                break;
            case wB:
                score += 3.0f + bishopTable[r][c];
                break;
            case wR:
                score += 5.0f + rookTable[r][c];
                break;
            case wQ:
                score += 9.0f + queenTable[r][c];
                break;
            case wK:
                score += kingTable[r][c];
                break;
            case bP:
                score -= 1.0f + pawnTable[7 - r][c];
                break;
            case bN:
                score -= 3.0f + knightTable[7 - r][c];
                break;
            case bB:
                score -= 3.0f + bishopTable[7 - r][c];
                break;
            case bR:
                score -= 5.0f + rookTable[7 - r][c];
                break;
            case bQ:
                score -= 9.0f + queenTable[7 - r][c];
                break;
            case bK:
                score -= kingTable[7 - r][c];
                break;
            default: // empty square
                break;
            }
        }
    }
    return score;
}

// End-game scoring: White mate = +1e6, stalemate = 0, Black mate = -1e6.
// Invoked when no legal moves remain (getLegalMoves returned an empty vector).
float Position::endResultScore(int depth) const {
    int kingRow, kingCol;
    if (_moveturn == WHITE) {
        findKing(wK, kingRow, kingCol);
        if (isSquareUnderAttack(kingRow, kingCol, BLACK)) {
            return -1000000.0f - depth;
        }
        else {
            return 0.0f;
        }
    }
    else {
        findKing(bK, kingRow, kingCol);
        if (isSquareUnderAttack(kingRow, kingCol, WHITE)) {
            return 1000000.0f + depth;
        }
        else {
            return 0.0f;
        }
    }
}

MinimaxValue Position::minimax(int depth, float alpha, float beta) {
    // Generate legal moves for the current side.
    vector<Move> allMoves;
    allMoves.reserve(100);
    getAllMoves(_moveturn, allMoves);
    vector<Move> legalMoves;
    legalMoves.reserve(100);
    getLegalMoves(allMoves, legalMoves);

    // Terminal conditions.
    if (legalMoves.empty())
        return { endResultScore(depth), Move() };
    if (depth == 0)
        return { evaluate(), Move() };

    Move bestMove;
    float bestValue;

    if (_moveturn == WHITE) {
        bestValue = numeric_limits<float>::lowest();
        // Maximizing player.
        for (const auto& move : legalMoves) {
            // Make the move in-place and record the undo state.
            UndoInfo undo = movePiece(move);
            // Switch turn so that minimax is called for the opponent.
            changeTurn();
            MinimaxValue childVal = minimax(depth - 1, alpha, beta);
            // Revert the turn back.
            changeTurn();
            // Unmake the move to restore the board state.
            undoMove(move, undo);

            if (childVal._value > bestValue) {
                bestValue = childVal._value;
                bestMove = move;
            }
            alpha = max(alpha, bestValue);
            if (beta <= alpha)
                break;  // Beta cutoff.
        }
    }
    else {
        bestValue = numeric_limits<float>::max();
        // Minimizing player.
        for (const auto& move : legalMoves) {
            UndoInfo undo = movePiece(move);
            changeTurn();
            MinimaxValue childVal = minimax(depth - 1, alpha, beta);
            changeTurn();
            undoMove(move, undo);

            if (childVal._value < bestValue) {
                bestValue = childVal._value;
                bestMove = move;
            }
            beta = min(beta, bestValue);
            if (beta <= alpha)
                break;  // Alpha cutoff.
        }
    }

    return { bestValue, bestMove };
}

// Calls the alpha-beta version with initial values.
MinimaxValue Position::minimax(int depth) {
    return minimax(depth, numeric_limits<float>::lowest(), numeric_limits<float>::max());
}

// --- BOARD VISUALISATION ---
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
