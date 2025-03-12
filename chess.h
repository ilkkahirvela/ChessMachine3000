#pragma once
#include <string>
#include <vector>
#include "move.h"

using namespace std;

/**
 * @file chess.h
 * @brief Contains utility functions and constants for chess game logic.
 *
 * This file defines enumerations for players and pieces, as well as helper functions to
 * convert between representations and validate moves.
 */

 /**
  * @brief Player identifiers.
  *
  * Enum representing the two players in a chess game.
  */
enum Player : int { WHITE, BLACK };

/**
 * @brief Chess piece constants.
 *
 * Enum values for different chess pieces. NA represents an empty square.
 * White pieces are typically represented by uppercase symbols and black pieces by lowercase.
 */
enum Piece : int { wR, wN, wB, wQ, wK, wP, bR, bN, bB, bQ, bK, bP, NA };

/**
 * @brief Promotion pieces for white.
 *
 * A vector of piece constants that a white pawn can be promoted to.
 */
static const vector<int> whitePromotions{ wR, wN, wB, wQ };

/**
 * @brief Promotion pieces for black.
 *
 * A vector of piece constants that a black pawn can be promoted to.
 */
static const vector<int> blackPromotions{ bR, bN, bB, bQ };

/**
 * @brief Returns the printable representation of a chess piece.
 *
 * Converts the given piece constant to a string. White pieces are represented in uppercase,
 * black pieces in lowercase, and NA is represented as a space (" ").
 *
 * @param piece The chess piece constant.
 * @return A string representing the piece.
 */
string pieceIndicator(int piece);

/**
 * @brief Converts a UCI-formatted move string to a Move object.
 *
 * Parses a move provided in UCI notation (e.g., "e2e4") and returns a corresponding Move object.
 *
 * @param moveStr The move string in UCI format.
 * @return A Move object corresponding to the move string.
 */
Move uciToMove(const string& moveStr);

/**
 * @brief Validates if a move is legal.
 *
 * Checks whether the provided move exists in the list of all generated legal moves.
 *
 * @param allMoves A vector containing all legal moves.
 * @param playerMove The move to validate.
 * @return True if the move is found in the list of legal moves, false otherwise.
 */
bool validMove(const vector<Move>& allMoves, Move playerMove);

/**
 * @brief Returns the opponent's color.
 *
 * Given the current player's color, returns the opposing player's color.
 *
 * @param player The current player's color (WHITE or BLACK).
 * @return The opponent's color.
 */
int opponent(int player);
