#pragma once
#include <vector>
#include "chess.h"
#include "move.h"

using namespace std;

// Pelin tilan kuvaaminen ja siihen liittyvät operaatiot.
class Position
{
private:
	// bool isBlocked(int row, int column);

public:

	// [rivi][linja]
	//
	// [0][0] : vasen ylänurkka ("a8")
	// [7][0] : vasen alanurkka ("a1")
	// [7][7] : oikea alanurkka ("h1")
	//
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

	int _moveturn = WHITE;

	// Kirjanpito siitä, onko kuningas tai torni liikkunut.
	bool _valkea_lyhyt_linna_sallittu = true;
	bool _valkea_pitka_linna_sallittu = true;
	bool _musta_lyhyt_linna_sallittu = true;
	bool _musta_pitka_linna_sallittu = true;

	// Ohestalyöntiin tarvittava info.
	int _kaksoisaskel_linjalla = -1;

	int pieceColor(int row, int column) const;

	// Tekee annetun siirron laudalla. Voidaan olettaa, että siirto on laillinen.
	void movePiece(int startR, int startC, int endR, int endC);

	vector<int> getDirectionalMoves(int row, int column, const vector<pair<int, int>>& directions);

	vector<int> getRookMoves(int row, int column);

	vector<int> getBishopMoves(int row, int column);

	vector<int> getQueenMoves(int row, int column);

	vector<int> getKingMoves(int row, int column);

	vector<int> getKnightMoves(int row, int column);

	// Tyhjentää laudan.
	void emptyBoard();

	// Tulostaa laudan ascii-grafiikkana
	void printBoard() const;

	// etsii annetun kuninkaan (wK tai mK) rivin ja linjan
	void findKing(int piece, int& row, int& column) const;

};
