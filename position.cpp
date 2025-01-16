#include "position.h"
#include "chess.h"
#include <iostream>

using namespace std;

void Position::emptyBoard() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			_board[i][j] = NA;
		}
	}
}

void Position::printBoard() const{
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << pieceSymbol(_board[i][j]) << " ";
			if (j == 7) {
				cout << endl;
			}
		}
	}
}

void Position::findKing(int nappula, int& rivi, int& linja) {

}
