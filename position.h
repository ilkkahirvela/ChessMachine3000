#pragma once
#include "chess.h"

// Pelin tilan kuvaaminen ja siihen liittyvät operaatiot.
class Position
{
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

	int _siirtovuoro = WHITE;

	// Kirjanpito siitä, onko kuningas tai torni liikkunut.
	bool _valkea_lyhyt_linna_sallittu = true;
	bool _valkea_pitka_linna_sallittu = true;
	bool _musta_lyhyt_linna_sallittu = true;
	bool _musta_pitka_linna_sallittu = true;

	// Ohestalyöntiin tarvittava info.
	int _kaksoisaskel_linjalla = -1;

	// Tyhjentää laudan.
	void emptyBoard();

	// Tulosta lauta ascii-grafiikkana (mahdollisesti flagit
	// voisi myös tulostaa jotenkin siististi).
	void printBoard() const;

	// etsii annetun kuninkaan (wK tai mK) rivin ja linjan
	void findKing(int piece, int& row, int& column) const;

};
