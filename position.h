/**
 * @file position.h
 * @brief Defines the Position class and related helper types for a chess game.
 *
 * This file contains the Position class responsible for maintaining the chessboard state,
 * generating moves, evaluating board positions, and performing a minimax search with alpha-beta pruning.
 */

#pragma once
#include <vector>
#include "move.h"
#include "chess.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <limits>
#include <chrono>
#include <exception>

using namespace std;
using namespace std::chrono;

/**
 * @brief Class representing the result of a minimax search.
 *
 * This class stores the evaluated score and the best move found during the search.
 */
class MinimaxValue {
public:
    /**
     * @brief Default constructor.
     *
     * Initializes the score to 0.0 and the move to a default Move.
     */
    MinimaxValue() : _value(0.0f), _move(Move()) {}

    /**
     * @brief Parameterized constructor.
     * @param value The evaluated score of the position.
     * @param move The best move found.
     */
    MinimaxValue(float value, Move move) : _value(value), _move(move) {}

    float _value;  ///< Evaluated score of the board position.
    Move _move;    ///< The best move associated with this score.
};

/**
 * @brief Structure to store information required to undo a move.
 *
 * This structure holds details about the state of the board before a move is applied,
 * including the moved piece, captured piece, castling rights, en passant data, and castling move details.
 */
struct UndoInfo {
    Move move;   ///< The move that was executed.
    int movedPiece;  ///< The piece that was moved.
    int capturedPiece;  ///< The piece that was captured (if any).
    pair<int, int> capturedPieceSquare;  ///< The board coordinates of the captured piece.
    bool enPassantCapture;  ///< True if the move was an en passant capture.

    // Castling rights
    bool whiteKingMoved;              ///< True if the white king has moved.
    bool whiteKingsideRookMoved;      ///< True if the white kingside rook has moved.
    bool whiteQueensideRookMoved;     ///< True if the white queenside rook has moved.
    bool blackKingMoved;              ///< True if the black king has moved.
    bool blackKingsideRookMoved;      ///< True if the black kingside rook has moved.
    bool blackQueensideRookMoved;     ///< True if the black queenside rook has moved.

    pair<int, int> enPassantSquare;  ///< The square eligible for en passant capture.

    // Castling move details
    bool castlingMove;  ///< True if the move was a castling move.
    int rookFromRow;    ///< The starting row of the rook during castling.
    int rookFromCol;    ///< The starting column of the rook during castling.
    int rookToRow;      ///< The destination row of the rook during castling.
    int rookToCol;      ///< The destination column of the rook during castling.
};

/**
 * @brief Class representing the state of the chessboard and move generation.
 *
 * The Position class manages the board state, move generation, legal move filtering,
 * board evaluation, and minimax search functionality.
 */
class Position {
private:
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

    static const vector<pair<int, int>> _knightOffsets;   ///< Offsets for knight moves.
    static const vector<pair<int, int>> _rookDirections;    ///< Directions for rook moves.
    static const vector<pair<int, int>> _bishopDirections;  ///< Directions for bishop moves.
    static const vector<pair<int, int>> _queenDirections;   ///< Directions for queen moves.
    static const vector<pair<int, int>> _kingMoves;         ///< Moves available to the king.

    bool _whiteKingMoved = false;         ///< True if the white king has moved.
    bool _whiteKingsideRookMoved = false; ///< True if the white kingside rook has moved.
    bool _whiteQueensideRookMoved = false;///< True if the white queenside rook has moved.
    bool _blackKingMoved = false;         ///< True if the black king has moved.
    bool _blackKingsideRookMoved = false; ///< True if the black kingside rook has moved.
    bool _blackQueensideRookMoved = false;///< True if the black queenside rook has moved.

    pair<int, int> _enPassantSquare = { -1, -1 }; ///< Square available for an en passant capture.

public:
    int _moveturn = WHITE;  ///< Indicates whose turn it is: WHITE or BLACK.

    /**
     * @brief Switches the turn to the other player.
     */
    void changeTurn();

    /**
     * @brief Determines the color of the piece at the given board coordinates.
     * @param row The row index.
     * @param column The column index.
     * @return WHITE, BLACK, or -1 if the square is empty or invalid.
     */
    int pieceColor(int row, int column) const;

    /**
     * @brief Executes a move on the board.
     * @param move The move to be executed.
     * @return An UndoInfo structure containing the state before the move was applied.
     */
    UndoInfo movePiece(Move move);

    /**
     * @brief Undoes a previously executed move.
     * @param move The move to be undone.
     * @param undo The UndoInfo structure with the board state before the move.
     */
    void undoMove(Move move, const UndoInfo& undo);

    /**
     * @brief Generates moves in specified directions starting from a given square.
     * @param row The starting row index.
     * @param column The starting column index.
     * @param directions A vector of directional offsets.
     * @param moves A vector to which the generated moves are added.
     */
    void getDirectionalMoves(int row, int column, const vector<pair<int, int>>& directions, vector<Move>& moves) const;

    /**
     * @brief Generates all legal rook moves from a given position.
     * @param row The row of the rook.
     * @param column The column of the rook.
     * @param moves A vector to which the generated moves are added.
     */
    void getRookMoves(int row, int column, vector<Move>& moves) const;

    /**
     * @brief Generates all legal bishop moves from a given position.
     * @param row The row of the bishop.
     * @param column The column of the bishop.
     * @param moves A vector to which the generated moves are added.
     */
    void getBishopMoves(int row, int column, vector<Move>& moves) const;

    /**
     * @brief Generates all legal queen moves from a given position.
     * @param row The row of the queen.
     * @param column The column of the queen.
     * @param moves A vector to which the generated moves are added.
     */
    void getQueenMoves(int row, int column, vector<Move>& moves) const;

    /**
     * @brief Generates all legal king moves from a given position.
     * @param row The row of the king.
     * @param column The column of the king.
     * @param moves A vector to which the generated moves are added.
     */
    void getKingMoves(int row, int column, vector<Move>& moves) const;

    /**
     * @brief Generates all legal knight moves from a given position.
     * @param row The row of the knight.
     * @param column The column of the knight.
     * @param moves A vector to which the generated moves are added.
     */
    void getKnightMoves(int row, int column, vector<Move>& moves) const;

    /**
     * @brief Generates all legal pawn moves from a given position.
     * @param row The row of the pawn.
     * @param column The column of the pawn.
     * @param piece The type of the pawn (wP or bP).
     * @param moves A vector to which the generated moves are added.
     * @param player The color of the player (WHITE or BLACK).
     */
    void getPawnMoves(int row, int column, int piece, vector<Move>& moves, int player) const;

    /**
     * @brief Generates all moves for the current player.
     * @param player The current player's color (WHITE or BLACK).
     * @param moves A vector to which the generated moves are added.
     */
    void getAllMoves(int player, vector<Move>& moves) const;

    /**
     * @brief Filters the generated moves to keep only the legal ones.
     * @param allMoves A vector containing all generated moves.
     * @param legalMoves A vector to which the legal moves are added.
     */
    void getLegalMoves(vector<Move> allMoves, vector<Move>& legalMoves) const;

    /**
     * @brief Checks if a given square is under attack by the opponent.
     * @param row The row index of the square.
     * @param col The column index of the square.
     * @param opponent The opponent's color (WHITE or BLACK).
     * @return True if the square is under attack, false otherwise.
     */
    bool isSquareUnderAttack(int row, int col, int opponent) const;

    /**
     * @brief Finds the king's position on the board.
     * @param piece The king piece (wK or bK).
     * @param row Reference to an integer where the king's row will be stored.
     * @param column Reference to an integer where the king's column will be stored.
     */
    void findKing(int piece, int& row, int& column) const;

    /**
     * @brief Evaluates the current board position.
     * @return A floating-point number representing the evaluation score of the board.
     */
    float evaluate() const;

    /**
     * @brief Returns the score for a terminal game state.
     * @param depth The search depth at which the game state was reached.
     * @return A large positive number for a win, a large negative number for a loss, or zero for a draw.
     */
    float endResultScore(int depth) const;

    /**
     * @brief Executes a minimax search with alpha-beta pruning.
     * @param depth The depth of the search.
     * @param alpha The alpha value for pruning.
     * @param beta The beta value for pruning.
     * @param startTime The starting time point of the search.
     * @param timeLimitMs The time limit in milliseconds.
     * @return A MinimaxValue containing the best evaluation score and associated move.
     */
    MinimaxValue minimax(int depth, float alpha, float beta, const std::chrono::steady_clock::time_point& startTime, int timeLimitMs);

    /**
     * @brief Performs iterative deepening using minimax search.
     * @param maxDepth The maximum search depth.
     * @param timeLimitMs The time limit in milliseconds.
     * @return A MinimaxValue containing the best evaluation score and associated move.
     */
    MinimaxValue iterativeDeepening(int maxDepth, int timeLimitMs);

    /**
     * @brief Clears the board by setting all squares to empty.
     */
    void emptyBoard();

    /**
     * @brief Inserts a test piece at the specified location.
     * @param r The row index.
     * @param c The column index.
     * @param piece The piece to insert.
     */
    void insertTestPiece(int r, int c, int piece);

    /**
     * @brief Prints the board to the console.
     */
    void printBoard() const;
};
