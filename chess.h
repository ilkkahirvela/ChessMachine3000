#pragma once
#include <string>
#include <vector>
#include "move.h"

using namespace std;

// Players
enum { WHITE, BLACK };

// Vakioarvot eri nappululoille (NA = tyhjä ruutu).
enum Pieces { wR, wN, wB, wQ, wK, wP, bR, bN, bB, bQ, bK, bP, NA };

static const vector<int> whitePromotions{ wR, wN, wB, wQ };
static const vector<int> blackPromotions{ bR, bN, bB, bQ };

// Palauttaa annetun nappulan tulostettavan muodon (valkoiset isolla, mustat pienellä, 12 = " ")
string pieceIndicator(int piece);

pair<pair<int, int>, pair<int, int>> uciToCoords(const string& move);

string coordsToUci(const vector<Move>& moves);

// Palauttaa pelaajan vastustajan.
int opponent(int player);
