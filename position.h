#pragma once
#include <vector>
#include "move.h"
#include "chess.h"

using namespace std;

class MinimaxValue {
public:
    MinimaxValue(float value, Move move) :
        _value(value), _move(move)
    {}
    float _value;
    Move _move;
};

class Position {
private:
    int _board[8][8] = {
        { bR, bN, bB, bQ, bK, bB, bN, bR },
        { bP, bP, bP, bP, bP, bP, bP, bP },
        { NA, NA, NA, NA, NA, NA, NA, NA },
        { NA, NA, NA, NA, NA, NA, NA, NA },
        { NA, NA, NA, NA, NA, NA, NA, NA },
        { NA, NA, NA, NA, NA, NA, NA, NA },
        { wP, wP, wP, wP, wP, wP, wP, wP },
        { wR, wN, wB, wQ, wK, wB, wN, wR }
    };

    static const vector<pair<int, int>> _knightOffsets;
    static const vector<pair<int, int>> _rookDirections;
    static const vector<pair<int, int>> _bishopDirections;
    static const vector<pair<int, int>> _queenDirections;
    static const vector<pair<int, int>> _kingMoves;

    bool _whiteKingMoved = false;
    bool _whiteKingsideRookMoved = false;
    bool _whiteQueensideRookMoved = false;
    bool _blackKingMoved = false;
    bool _blackKingsideRookMoved = false;
    bool _blackQueensideRookMoved = false;

    int _kaksoisaskel_linjalla = -1;

public:
    int _moveturn = WHITE;

    void changeTurn();
    int pieceColor(int row, int column) const;
    void movePiece(Move move);
    
    void getDirectionalMoves(int row, int column, const vector<pair<int, int>>& directions, vector<Move>& moves) const;
    void getRookMoves(int row, int column, vector<Move>& moves) const;
    void getBishopMoves(int row, int column, vector<Move>& moves) const;
    void getQueenMoves(int row, int column, vector<Move>& moves) const;
    void getKingMoves(int row, int column, vector<Move>& moves) const;
    void getKnightMoves(int row, int column, vector<Move>& moves) const;
    void getPawnMoves(int row, int column, int piece, vector<Move>& moves, int player) const;
    
    void getAllMoves(int player, vector<Move>& moves) const;
    void getLegalMoves(vector<Move> allMoves, vector<Move>& legalMoves) const;

    bool isSquareUnderAttack(int row, int col, int opponent) const;
    void findKing(int piece, int& row, int& column) const;

    // Evaluates the score of a chess position
    float evaluate() const;
    float endResultScore() const;
    MinimaxValue minimax(int depth, float alpha, float beta);
    MinimaxValue minimax(int depth);

    void emptyBoard();
    void insertTestPiece(int r, int c, int piece);
    void printBoard() const;
};
