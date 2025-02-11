#include "position.h"
#include "chess.h"
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

const vector<pair<int, int>> Position::_knightOffsets = {
    {-2, -1}, {-2, 1}, {2, -1}, {2, 1},
    {-1, -2}, {-1, 2}, {1, -2}, {1, 2}
};

const vector<pair<int, int>> Position::_rookDirections ={
    {-1, 0}, {1, 0}, {0, -1}, {0, 1}
};

const vector<pair<int, int>> Position::_bishopDirections = {
    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
};

const vector<pair<int, int>> Position::_queenDirections = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1},
    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
};

const vector<pair<int, int>> Position::_kingMoves = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1},
    {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
};

void Position::changeTurn() {
    if (_moveturn == WHITE) {
        _moveturn = BLACK;
    }
    else {
        _moveturn = WHITE;
    }
}

int Position::pieceColor(int row, int column) const {
    int piece = _board[row][column];

    if (piece >= wR && piece <= wP) {
        return WHITE;
    }
    if (piece >= bR && piece <= bP) {
        return BLACK;
    }
    return -1;  // Empty square or invalid piece
}

void Position::movePiece(Move move) {
    int piece = _board[move.startRow][move.startCol];
    _board[move.startRow][move.startCol] = NA;

    // Update castling booleans *****NEED TO ALSO UPDATE WHEN ROOK IS CAPTURED BY ENEMY!*****
    if (piece == wK) {
        _whiteKingMoved = true;
    }
    else if (piece == bK) {
        _blackKingMoved = true;
    }
    else if (piece == wR) {
        if (move.startRow == 7 && move.startCol == 0) {
            _whiteQueensideRookMoved = true;
        }
        else if (move.startRow == 7 && move.startCol == 7) {
            _whiteKingsideRookMoved = true;
        }
    }
    else if (piece == bR) {
        if (move.startRow == 0 && move.startCol == 0) {
            _blackQueensideRookMoved = true;
        }
        else if (move.startRow == 0 && move.startCol == 7) {
            _blackKingsideRookMoved = true;
        }
    }

    // Pawn promotion
    if ((piece == wP && move.endRow == 0) || (piece == bP && move.endRow == 7)) {
        // If promotedPiece is not specified, default to queen
        if (move.promotion == NA) {
            move.promotion = (piece == wP) ? wQ : bQ;
        }
        _board[move.endRow][move.endCol] = move.promotion;
    }
    else {
        _board[move.endRow][move.endCol] = piece;
    }

    // Castling
    // If the king moves two squares, the move is a castling move
    if ((piece == wK || piece == bK) && abs(move.startCol - move.endCol) == 2) {
        if (move.endCol == 6) {
            _board[move.startRow][7] = NA;
            _board[move.startRow][5] = (piece == wK) ? wR : bR;
        }
        else if (move.endCol == 2) {
            _board[move.startRow][0] = NA;
            _board[move.startRow][3] = (piece == wK) ? wR : bR;
        }
    }
}

void Position::getDirectionalMoves(int row, int column, const vector<pair<int, int>>& directions, vector<Move>& moves) const {
    for (const auto& dir : directions) {
        int r = row + dir.first;
        int c = column + dir.second;

        while (r >= 0 && r < 8 && c >= 0 && c < 8) {
            int piece = pieceColor(r, c);

            if (piece != -1) {  // There's a piece at this position, not empty square
                if (piece != _moveturn) {
                    moves.push_back(Move(row, column, r, c));  // Capture enemy piece
                }
                break;  // Stop moving in this direction
            }

            moves.push_back(Move(row, column, r, c));
            r += dir.first;
            c += dir.second;
        }
    }
}

void Position::getRookMoves(int row, int column, vector<Move>& moves) const {
    getDirectionalMoves(row, column, _rookDirections, moves);
}

void Position::getBishopMoves(int row, int column, vector<Move>& moves) const {
    getDirectionalMoves(row, column, _bishopDirections, moves);
}

void Position::getQueenMoves(int row, int column, vector<Move>& moves) const {
    getDirectionalMoves(row, column, _queenDirections, moves);
}

void Position::getKingMoves(int row, int column, vector<Move>& moves) const {
    for (const auto& move : _kingMoves) {
        int r = row + move.first;
        int c = column + move.second;

        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            int piece = pieceColor(r, c);
            if (piece == -1 || piece != _moveturn) {
                moves.push_back(Move(row, column, r, c));
            }
        }
    }

    // Castling Moves
    if (_moveturn == WHITE && !_whiteKingMoved) {
        // White kingside castling: King from e1 (7,4) to g1 (7,6)
        if (_board[7][5] == NA && _board[7][6] == NA && !_whiteKingsideRookMoved) {
            if (!isSquareUnderAttack(7, 4, BLACK) &&
                !isSquareUnderAttack(7, 5, BLACK) &&
                !isSquareUnderAttack(7, 6, BLACK)) {
                moves.push_back(Move(7, 4, 7, 6));
            }
        }
        // White queenside castling: King from e1 (7,4) to c1 (7,2)
        if (_board[7][3] == NA && _board[7][2] == NA && _board[7][1] == NA && !_whiteQueensideRookMoved) {
            if (!isSquareUnderAttack(7, 4, BLACK) &&
                !isSquareUnderAttack(7, 3, BLACK) &&
                !isSquareUnderAttack(7, 2, BLACK)) {
                moves.push_back(Move(7, 4, 7, 2));
            }
        }
    }
    else if (_moveturn == BLACK && !_blackKingMoved) {
        // Black kingside castling: King from e8 (0,4) to g8 (0,6)
        if (_board[0][5] == NA && _board[0][6] == NA && !_blackKingsideRookMoved) {
            if (!isSquareUnderAttack(0, 4, WHITE) &&
                !isSquareUnderAttack(0, 5, WHITE) &&
                !isSquareUnderAttack(0, 6, WHITE)) {
                moves.push_back(Move(0, 4, 0, 6));
            }
        }
        // Black queenside castling: King from e8 (0,4) to c8 (0,2)
        if (_board[0][3] == NA && _board[0][2] == NA && _board[0][1] == NA && !_blackQueensideRookMoved) {
            if (!isSquareUnderAttack(0, 4, WHITE) &&
                !isSquareUnderAttack(0, 3, WHITE) &&
                !isSquareUnderAttack(0, 2, WHITE)) {
                moves.push_back(Move(0, 4, 0, 2));
            }
        }
    }
}

void Position::getKnightMoves(int row, int column, vector<Move>& moves) const {
    for (const auto& move : _knightOffsets) {
        int r = row + move.first;
        int c = column + move.second;

        if (r >= 0 && r < 8 && c >= 0 && c < 8) {
            int piece = pieceColor(r, c);
            if (piece == -1 || piece != _moveturn) {
                moves.push_back(Move(row, column, r, c));
            }
        }
    }
}

void Position::getPawnMoves(int row, int column, int piece, vector<Move>& moves, int player) const {
    int direction = (piece == wP) ? -1 : 1; // White pawns move up (-1), black pawns move down (+1)
    int startRow = (piece == wP) ? 6 : 1;
    int promotionRow = (piece == wP) ? 0 : 7; 

    // Single forward move
    int forwardRow = row + direction;
    if (forwardRow >= 0 && forwardRow < 8 && _board[forwardRow][column] == NA) {
        // Promotion handling
        if (forwardRow == promotionRow) {
            if (player == WHITE) {
                for (const auto& promotion : whitePromotions) {
                    moves.push_back(Move(row, column, forwardRow, column, promotion));
                }
            }
            else if (player == BLACK) {
                for (const auto& promotion : blackPromotions) {
                    moves.push_back(Move(row, column, forwardRow, column, promotion));
                }
            }
        }
        // No promotion available
        else {
            moves.push_back(Move(row, column, forwardRow, column));

            // Double forward move from the starting position
            int doubleForwardRow = row + 2 * direction;
            if (row == startRow && _board[doubleForwardRow][column] == NA) {
                moves.push_back(Move(row, column, doubleForwardRow, column));
            }
        }
    }

    // Capturing diagonally
    for (int dc : {-1, 1}) {  // Diagonal directions
        int captureRow = row + direction;
        int captureCol = column + dc;
        if (captureRow >= 0 && captureRow < 8 && captureCol >= 0 && captureCol < 8) {
            int targetPieceColor = pieceColor(captureRow, captureCol);
            if (targetPieceColor != -1 && targetPieceColor != _moveturn) {
                moves.push_back(Move(row, column, captureRow, captureCol));
            }
        }
    }

    // En passant
    
}

void Position::getAllMoves(int player, vector<Move>& moves) const {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            int piece = _board[r][c];
            if (pieceColor(r, c) == player) {
                switch (piece) {
                case wR: case bR:
                    getRookMoves(r, c, moves);
                    break;
                case wB: case bB:
                    getBishopMoves(r, c, moves);
                    break;
                case wQ: case bQ:
                    getQueenMoves(r, c, moves);
                    break;
                case wK: case bK:
                    getKingMoves(r, c, moves);
                    break;
                case wN: case bN:
                    getKnightMoves(r, c, moves);
                    break;
                case wP: case bP:
                    getPawnMoves(r, c, piece, moves, player);
                    break;
                default:
                    break;  // Empty square
                }
            }
        }
    }
}

void Position::getLegalMoves(vector<Move> allMoves, vector<Move>& legalMoves) const {
    int kingRow, kingCol;
    int player = _moveturn; // Store the moving player's color
    int opponent = (player == WHITE ? BLACK : WHITE);

    for (Move& move : allMoves) {
        Position posCopy = *this;

        posCopy.movePiece(move);
        posCopy.findKing(player == WHITE ? wK : bK, kingRow, kingCol);
        if (!posCopy.isSquareUnderAttack(kingRow, kingCol, opponent)) {
            legalMoves.push_back(move); // The move is legal
        }
    }
}

bool Position::isSquareUnderAttack(int row, int col, int opponent) const {
    // Loop through every square
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            int piece = _board[r][c];
            if (piece == NA) { // Skip empty squares
                continue;  
            }
            if (pieceColor(r, c) != opponent) {
                continue;
            }
            switch (piece) {
                // Pawn
            case wP:
            case bP: {
                if (opponent == WHITE) {
                    if ((r - 1 == row && c - 1 == col) ||
                        (r - 1 == row && c + 1 == col)) {
                        return true;
                    }
                }
                else {  // opponent == BLACK
                    if ((r + 1 == row && c - 1 == col) ||
                        (r + 1 == row && c + 1 == col)) {
                        return true;
                    }
                }
                break;
            }
                   // Knight
            case wN:
            case bN: {
                for (const auto& offset : _knightOffsets) {
                    if (r + offset.first == row && c + offset.second == col) {
                        return true;
                    }
                }
                break;
            }
                   // King
            case wK:
            case bK: {
                for (const auto& move : _kingMoves) {
                    if (r + move.first == row && c + move.second == col) {
                        return true;
                    }
                }
                break;
            }
                   // Rook
            case wR:
            case bR: {
                for (const auto& dir : _rookDirections) {
                    int rr = r + dir.first;
                    int cc = c + dir.second;
                    while (rr >= 0 && rr < 8 && cc >= 0 && cc < 8) {
                        if (rr == row && cc == col) {
                            return true;
                        }
                        if (_board[rr][cc] != NA) {
                            break;
                        }
                        rr += dir.first;
                        cc += dir.second;
                    }
                }
                break;
            }
                   // Bishop
            case wB:
            case bB: {
                for (const auto& dir : _bishopDirections) {
                    int rr = r + dir.first;
                    int cc = c + dir.second;
                    while (rr >= 0 && rr < 8 && cc >= 0 && cc < 8) {
                        if (rr == row && cc == col) {
                            return true;
                        }
                        if (_board[rr][cc] != NA) {
                            break;
                        }
                        rr += dir.first;
                        cc += dir.second;
                    }
                }
                break;
            }
                   // Queen
            case wQ:
            case bQ: {
                for (const auto& dir : _queenDirections) {
                    int rr = r + dir.first;
                    int cc = c + dir.second;
                    while (rr >= 0 && rr < 8 && cc >= 0 && cc < 8) {
                        if (rr == row && cc == col) {
                            return true;
                        }
                        if (_board[rr][cc] != NA) {
                            break;
                        }
                        rr += dir.first;
                        cc += dir.second;
                    }
                }
                break;
            }
            default:
                break;
            }
        }
    }
    // If no opponent piece attacks the square, return false.
    return false;
}

void Position::findKing(int piece, int& row, int& column) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (_board[r][c] == piece) {
                row = r;
                column = c;
                return;
            }
        }
    }
}

float Position::material() {
    return 0;
}

float Position::endResultScore() const {
    return 0;
}

float Position::evaluate() const {
    // return 1.0f * material() + 0.1f * mobiliteetti();

    // TODO
    // t‰ydent‰k‰‰ halutessanne uusilla pisteytett‰vill‰ aseman piirteill‰.
    // Mobiliteetin sijasta kannattanee pisteytt‰‰ materiaali nappuloiden
    // keskeisen sijainin perusteella, ks. esim. 
    // https://github.com/bytefire/Shutranj/blob/master/Shutranj.Engine/Evaluation2.cs
    return 0;
}

// Palauttaa aseman minimax-arvon. Syvyys m‰‰ritt‰‰,
// kuinka monta asekelta syvemm‰lle pelipuuta k‰yd‰‰n l‰pi.
//
// Testaaminen esim. p‰‰ohjelmasta:
//
// Asema asema;
// MinimaxArvo arvo = asema.minimax(4);
// 
// Nyt tietokoneen siirto saadaan pelattua n‰in:
// asema.tee_siirto(arvo._siirto);
MinimaxValue Position::minimax(int depth, vector<Move> moves) {
    if (moves.size() == 0)
    {
        // Rekursion kantatapaus 1:
        // peli on p‰‰ttynyt (ei yht‰‰n laillista siirtoa).
        return MinimaxValue(endResultScore(), Move());
    }

    if (depth == 0)
    {
        // Rekursion kantatapaus 2:
        // ollaan katkaisusyvyydess‰.
        return MinimaxValue(evaluate(), Move());
    }

    // Siirtoja on j‰ljell‰ ja ei olla katkaisusyvyydess‰,
    // joten kokeillaan yksitellen mahdollisia siirtoja,
    // ja kutsutaan minimax:a kullekin seuraaja-asemalle.
    // Otetaan paras minimax-arvo talteen (alustetaan
    // paras_arvo mahdollisimman huonoksi siirtovuoroisen
    // pelaajan kannalta).
    float bestValue = _moveturn == WHITE ? numeric_limits<float>::min() : numeric_limits<float>::max();
    Move bestMove;
    for (Move& move : moves)
    {
        Position posCopy = *this;
        posCopy.movePiece(move);

        // Rekursioasekel: kutsutaan minimax:ia seuraaja-asemalle.
        MinimaxValue value = posCopy.minimax(depth - 1, moves);

        // Jos saatiin paras arvo, otetaan se talteen.
        if (_moveturn == WHITE && value._value > bestValue)
        {
            bestValue = value._value;
            bestMove = move;
        }
        else if (_moveturn == BLACK && value._value < bestValue)
        {
            bestValue = value._value;
            bestMove = move;
        }
    }

    // Palautetaan paras arvo.
    return MinimaxValue(bestValue, bestMove);
}

// --- BOARD VISUALISATION ---
void Position::emptyBoard() {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            _board[r][c] = NA;
        }
    }
}

void Position::insertTestPiece(int r, int c, int piece) {
    _board[r][c] = piece;
}

void Position::printBoard() const {
    const int boardSize = 8;
    cout << "    ";
    for (char file = 'a'; file <= 'h'; ++file) {
        cout << file << "   ";  // Spacing between file labels
    }
    cout << endl;

    cout << "  +";
    for (int i = 0; i < boardSize; i++) {
        cout << "----";
    }
    cout << "+" << endl;

    for (int r = 0; r < boardSize; r++) {
        cout << 8 - r << " | ";

        for (int c = 0; c < boardSize; c++) {
            cout << setw(2) << pieceIndicator(_board[r][c]) << " |";
        }

        cout << endl;

        if (r < boardSize - 1) {
            cout << "  +";
            for (int i = 0; i < boardSize; i++) {
                cout << "----";
            }
            cout << "+" << endl;
        }
    }

    cout << "  +";
    for (int i = 0; i < boardSize; i++) {
        cout << "----";
    }
    cout << "+" << endl;
}
