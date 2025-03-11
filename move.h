#pragma once
#include <string>

using namespace std;

/**
 * @brief Represents a chess move.
 *
 * This class encapsulates a chess move, including the starting and ending coordinates,
 * as well as an optional promotion piece.
 */
class Move {
public:
    int startRow;    ///< The starting row index of the move.
    int startCol;    ///< The starting column index of the move.
    int endRow;      ///< The ending row index of the move.
    int endCol;      ///< The ending column index of the move.
    int promotion;   ///< The promotion piece (if applicable), otherwise a default value.

    /**
     * @brief Default constructor.
     *
     * Initializes the move with default values.
     */
    Move();

    /**
     * @brief Constructs a move with specified start and end coordinates.
     * @param startRow The starting row index.
     * @param startCol The starting column index.
     * @param endRow The ending row index.
     * @param endCol The ending column index.
     */
    Move(int startRow, int startCol, int endRow, int endCol);

    /**
     * @brief Constructs a move with specified coordinates and promotion.
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
     * This utility method converts the move to a human-readable string format.
     *
     * @return A string that represents the move.
     */
    string toString() const;
};
