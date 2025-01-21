#pragma once
#include <string>
#include <vector>

using namespace std;

// Pelaajat.
enum { WHITE, BLACK };

// Vakioarvot eri nappululoille (NA = tyhjä ruutu).
enum Pieces { wR, wN, wB, wQ, wK, wP, bR, bN, bB, bQ, bK, bP, NA };

// Palauttaa annetun nappulan tulostettavan muodon (valkoiset isolla, mustat pienellä, 12 = " ")
string pieceIndicator(int piece);

pair<pair<int, int>, pair<int, int>> uciToCoords(const string& move);

string coordsToUci(const vector<int>& moves);

// Palauttaa pelaajan vastustajan.
int opponent(int player);
