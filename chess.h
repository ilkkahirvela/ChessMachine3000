#pragma once
#include <string>

using namespace std;

// Pelaajat.
enum { WHITE, BLACK };

// Vakioarvot eri nappululoille (NA = tyhjä ruutu).
enum Pieces { wR, wN, wB, wQ, wK, wP, bR, bN, bB, bQ, bK, bP, NA };

// Palauttaa annetun nappulan tulostettavan muodon (valkoiset isolla, mustat pienellä, 12 = " ")
string pieceIndicator(int piece);

pair<pair<int, int>, pair<int, int>> UciToCoords(const string& move);

// Palauttaa annetun nappulan värin (VALKEA, MUSTA)
int pieceColor(int piece);

// Palauttaa pelaajan vastustajan.
int opponent(int player);
