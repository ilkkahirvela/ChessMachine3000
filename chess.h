#pragma once
#include <string>

using namespace std;

// Pelaajat.
enum { VALKEA, MUSTA };

// Vakioarvot eri nappululoille (NA = tyhjä ruutu).
// typedef .... nappula
enum { wR, wN, wB, wQ, wK, wP, bR, bN, bB, bQ, bK, bP, NA };

string pieceSymbol(int piece);

// Palauttaa annetun nappulan värin (VALKEA, MUSTA).
int pieceColor(int piece);

// Palauttaa pelaajan vastustajan.
int opponent(int player);
