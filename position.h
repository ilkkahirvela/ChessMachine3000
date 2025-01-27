#pragma once
#include <vector>
#include "move.h"
#include "chess.h"

using namespace std;

class Position {
private:
    int _board[8][8] = {
        { bR, bN, bB, bQ, bK, bB, bN, bR },
        { bP, bP, bP, bP, bP, bP, bP, bP },
        { NA, NA, NA, NA, NA, NA, NA, NA },
        { NA, NA, NA, NA, NA, NA, NA, NA },
        { NA, NA, NA, NA, NA, NA, NA, NA },
        { NA, NA, NA, NA, NA, NA, NA, NA },
        { wP, wP, wP, NA, wP, wP, wP, wP },
        { wR, wN, wB, wQ, wK, wB, wN, wR }
    };

    int _moveturn = WHITE;
    bool _valkea_lyhyt_linna_sallittu = true;
    bool _valkea_pitka_linna_sallittu = true;
    bool _musta_lyhyt_linna_sallittu = true;
    bool _musta_pitka_linna_sallittu = true;
    int _kaksoisaskel_linjalla = -1;

public:
    int pieceColor(int row, int column) const;
    void movePiece(int startR, int startC, int endR, int endC);

    void getDirectionalMoves(int row, int column, const vector<pair<int, int>>& directions, vector<Move>& moves) const;
    void getRookMoves(int row, int column, vector<Move>& moves) const;
    void getBishopMoves(int row, int column, vector<Move>& moves) const;
    void getQueenMoves(int row, int column, vector<Move>& moves) const;
    void getKingMoves(int row, int column, vector<Move>& moves) const;
    void getKnightMoves(int row, int column, vector<Move>& moves) const;
    void getPawnMoves(int row, int column, int piece, vector<Move>& moves) const;
    void getAllMoves(int player, vector<Move>& moves) const;

    void emptyBoard();
    void printBoard() const;
    void findKing(int piece, int& row, int& column) const;
};
