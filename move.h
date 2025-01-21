#pragma once
#include <string>
#include "chess.h"
#include "position.h"

using namespace std;

// Kuvaa aseman muutosta.
class Move
{
public:

	// Oletuskonstruktori (ei tee oikeastaan mitään).
	Move()
	{}

	// Alustaa siirron koordinaateista. (startR, startC) => (endR, endC)
	Move(int startR, int startC, int endR, int endC) :
		_startR(startR), _startC(startC), _endR(endR), _endC(endC)
	{}

	// Konstruktori, jonka avulla siirto-olio alustetaan
	// annetusta merkkijonosta, esim. "e2e4" tai "g8f6".
	Move(const string& m)
	{}


private:

	int _startR; // siirron alkuruudun rivi
	int _startC; // siirron alkuruudu linja (eli sarake)

	int _endR; // siirron loppuruudun rivi
	int _endC; // siirron loppuruudun linja

	int _korotettava_nappula = NA; // esim. wQ, wR, bN jne.
	bool _ohestalyonti = false;

	friend class Asema;
};
