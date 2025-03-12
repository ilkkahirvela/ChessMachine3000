/**
 * @file move.h
 * @brief Defines the Move class for representing chess moves.
 *
 * This file declares the Move class, which encapsulates the starting and ending coordinates of a chess move,
 * along with an optional promotion piece for pawn promotion.
 */

#pragma once
#include <string>

using namespace std;

/**
 * @brief Represents a chess move.
 *
 * The Move class encapsulates a chess move, including the starting and ending coordinates,
 * as well as an optional promotion piece. It provides constructors to create moves with or without
 * promotion and a utility method to convert the move into a human-readable string.
 */
class Move {
public:
    int startRow;    ///< The starting row index of the move.
    int startCol;    ///< The starting column index of the move.
    int endRow;      ///< The ending row index of the move.
    int endCol;      ///< The ending column index of the move.
    int promotion;   ///< The promotion piece (if applicable); otherwise, a default value (-1).

    /**
     * @brief Default constructor.
     *
     * Initializes the move with default values (-1 for all coordinates and promotion).
     */
    Move();

    /**
     * @brief Constructs a move with specified start and end coordinates.
     *
     * Initializes a move from the starting position (startRow, startCol) to the ending position (endRow, endCol)
     * with no promotion.
     *
     * @param startRow The starting row index.
     * @param startCol The starting column index.
     * @param endRow The ending row index.
     * @param endCol The ending column index.
     */
    Move(int startRow, int startCol, int endRow, int endCol);

    /**
     * @brief Constructs a move with specified coordinates and promotion.
     *
     * Initializes a move from the starting position to the ending position and sets the promotion piece.
     *
     * @param startRow The starting row index.
     * @param startCol The starting column index.
     * @param endRow The ending row index.
     * @param endCol The ending column index.
     * @param promotion The piece type to which a pawn is promoted.
     */
    Move(int startRow, int startCol, int endRow, int endCol, int promotion);

    /**
     * @brief Returns a string representation of the move.
     *
     * Converts the move to a human-readable string using standard chess notation.
     * For example, a move from e2 to e4 would be represented as "e2e4". If the move involves
     * pawn promotion, the promotion piece is appended.
     *
     * @return string A string that represents the move.
     */
    string toString() const;
};
