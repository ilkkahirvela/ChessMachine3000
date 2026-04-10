/**
 * @file position.h
 * @brief Defines the Position class and related helper types for a chess game.
 *
 * This file contains the Position class responsible for maintaining the chessboard state,
 * generating moves, evaluating board positions, and performing a minimax search with alpha-beta pruning.
 */

#pragma once
#include <string>
#include <vector>
#include <chrono>
#include <exception>
#include "move.h"
#include "chess.h"

/**
 * @brief Exception class for handling time limit exceedance during search.
 */
class TimeLimitExceeded : public std::exception {
public:
    /**
     * @brief Returns the error message.
     * @return const char* C-string describing the exception.
     */
    const char* what() const noexcept override {
        return "Time limit exceeded";
    }
};

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
    std::pair<int, int> capturedPieceSquare;  ///< The board coordinates of the captured piece.
    bool enPassantCapture;  ///< True if the move was an en passant capture.

    // Castling rights
    bool whiteKingMoved;              ///< True if the white king has moved.
    bool whiteKingsideRookMoved;      ///< True if the white kingside rook has moved.
    bool whiteQueensideRookMoved;     ///< True if the white queenside rook has moved.
    bool blackKingMoved;              ///< True if the black king has moved.
    bool blackKingsideRookMoved;      ///< True if the black kingside rook has moved.
    bool blackQueensideRookMoved;     ///< True if the black queenside rook has moved.

    std::pair<int, int> enPassantSquare;  ///< The square eligible for en passant capture.
    int halfMoveClock;  ///< Half-move clock before this move (for 50-move rule).

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

    static const std::vector<std::pair<int, int>> _knightOffsets;   ///< Offsets for knight moves.
    static const std::vector<std::pair<int, int>> _rookDirections;    ///< Directions for rook moves.
    static const std::vector<std::pair<int, int>> _bishopDirections;  ///< Directions for bishop moves.
    static const std::vector<std::pair<int, int>> _queenDirections;   ///< Directions for queen moves.
    static const std::vector<std::pair<int, int>> _kingMoves;         ///< Moves available to the king.

    bool _whiteKingMoved = false;         ///< True if the white king has moved.
    bool _whiteKingsideRookMoved = false; ///< True if the white kingside rook has moved.
    bool _whiteQueensideRookMoved = false;///< True if the white queenside rook has moved.
    bool _blackKingMoved = false;         ///< True if the black king has moved.
    bool _blackKingsideRookMoved = false; ///< True if the black kingside rook has moved.
    bool _blackQueensideRookMoved = false;///< True if the black queenside rook has moved.

    std::pair<int, int> _enPassantSquare = { -1, -1 }; ///< Square available for an en passant capture.
    int _halfMoveClock = 0; ///< Increments each move, resets on pawn move or capture (50-move rule).

    int _moveturn = WHITE;  ///< Indicates whose turn it is: WHITE or BLACK.

public:
    /**
     * @brief Returns the current player's turn.
     *
     * @return int WHITE or BLACK.
     */
    int getTurn() const { return _moveturn; }

    /**
     * @brief Returns the current half-move clock value.
     *
     * The half-move clock increments each half-move and resets on a pawn move or capture.
     * It is used to enforce the 50-move draw rule (draw at 100 half-moves).
     *
     * @return int The current half-move clock count.
     */
    int getHalfMoveClock() const { return _halfMoveClock; }

    /**
     * @brief Returns the material balance from White's perspective.
     *
     * Calculates the difference in total piece values between White and Black
     * using standard piece values (P=1, N=3, B=3, R=5, Q=9).
     * A positive value means White has more material; negative means Black does.
     *
     * @return int The material balance (White minus Black).
     */
    int getMaterialBalance() const;

    /**
     * @brief Returns a string key uniquely identifying the current position.
     *
     * Encodes the board, turn, castling rights, and en passant square.
     * Used for threefold repetition detection.
     */
    std::string getPositionKey() const;

    /**
     * @brief Switches the turn to the other player.
     *
     * Changes _moveturn from WHITE to BLACK or vice versa.
     */
    void changeTurn();

    /**
     * @brief Determines the color of the piece at the given board coordinates.
     *
     * @param row The row index of the board.
     * @param column The column index of the board.
     * @return int Returns WHITE, BLACK, or -1 if the square is empty or invalid.
     */
    int pieceColor(int row, int column) const;

    /**
     * @brief Executes a move on the board and updates the game state.
     *
     * Moves a piece from its start position to its end position. Handles special cases such as
     * en passant captures, pawn promotions, and castling moves. Returns an UndoInfo structure
     * that captures the state prior to the move.
     *
     * @param move The move to execute.
     * @return UndoInfo The state information necessary to undo this move.
     */
    UndoInfo movePiece(Move move);

    /**
     * @brief Undoes a previously executed move.
     *
     * Reverts the board to its state before a move was executed, restoring any captured pieces,
     * castling rights, and en passant eligibility.
     *
     * @param move The move to undo.
     * @param undo The UndoInfo structure containing the previous state.
     */
    void undoMove(Move move, const UndoInfo& undo);

    /**
     * @brief Generates moves in specified directions from a given square.
     *
     * Follows the provided directional offsets from the start square until an edge or a blocking piece is encountered.
     *
     * @param row The starting row index.
     * @param column The starting column index.
     * @param directions A vector of (row, column) offsets to follow.
     * @param moves A vector to which the generated moves are added.
     */
    void getDirectionalMoves(int row, int column, const std::vector<std::pair<int, int>>& directions, std::vector<Move>& moves) const;

    /**
     * @brief Generates all legal rook moves from the specified position.
     *
     * @param row The row index of the rook.
     * @param column The column index of the rook.
     * @param moves A vector to which the rook moves are appended.
     */
    void getRookMoves(int row, int column, std::vector<Move>& moves) const;

    /**
     * @brief Generates all legal bishop moves from the specified position.
     *
     * @param row The row index of the bishop.
     * @param column The column index of the bishop.
     * @param moves A vector to which the bishop moves are appended.
     */
    void getBishopMoves(int row, int column, std::vector<Move>& moves) const;

    /**
     * @brief Generates all legal queen moves from the specified position.
     *
     * @param row The row index of the queen.
     * @param column The column index of the queen.
     * @param moves A vector to which the queen moves are appended.
     */
    void getQueenMoves(int row, int column, std::vector<Move>& moves) const;

    /**
     * @brief Generates all legal king moves from the specified position.
     *
     * This includes both the standard one-square moves in any direction and the special castling moves.
     *
     * @param row The row index of the king.
     * @param column The column index of the king.
     * @param moves A vector to which the king moves are appended.
     */
    void getKingMoves(int row, int column, std::vector<Move>& moves) const;

    /**
     * @brief Generates all legal knight moves from the specified position.
     *
     * @param row The row index of the knight.
     * @param column The column index of the knight.
     * @param moves A vector to which the knight moves are appended.
     */
    void getKnightMoves(int row, int column, std::vector<Move>& moves) const;

    /**
     * @brief Generates all legal pawn moves from the specified position.
     *
     * Computes forward moves, captures (including en passant), double moves from the starting rank,
     * and pawn promotions.
     *
     * @param row The row index of the pawn.
     * @param column The column index of the pawn.
     * @param piece The pawn identifier (wP or bP).
     * @param moves A vector to which the pawn moves are appended.
     * @param player The color of the player (WHITE or BLACK).
     */
    void getPawnMoves(int row, int column, int piece, std::vector<Move>& moves, int player) const;

    /**
     * @brief Generates all possible moves for the current player.
     *
     * Iterates over the board and collects moves for all pieces belonging to the current player.
     *
     * @param player The current player's color (WHITE or BLACK).
     * @param moves A vector to which all generated moves are added.
     */
    void getAllMoves(int player, std::vector<Move>& moves) const;

    /**
     * @brief Filters a set of moves to retain only those that are legal.
     *
     * Checks each move by simulating it and verifying that the player's king is not left in check.
     *
     * @param allMoves A vector of moves generated from the board.
     * @param legalMoves A vector to which the moves that do not leave the king in check are added.
     */
    void getLegalMoves(const std::vector<Move>& allMoves, std::vector<Move>& legalMoves);

    /**
     * @brief Determines whether a given square is under attack by the opponent.
     *
     * Considers attacks from all piece types (pawn, knight, bishop, rook, queen, and king).
     *
     * @param row The row index of the square.
     * @param col The column index of the square.
     * @param opponent The opponent's color (WHITE or BLACK).
     * @return bool True if the square is under attack; otherwise, false.
     */
    bool isSquareUnderAttack(int row, int col, int opponent) const;

    /**
     * @brief Locates the king on the board.
     *
     * Searches for the king piece (wK or bK) and outputs its coordinates via the provided references.
     *
     * @param piece The king identifier (wK or bK).
     * @param row Reference variable to store the king's row index.
     * @param column Reference variable to store the king's column index.
     */
    void findKing(int piece, int& row, int& column) const;

    /**
     * @brief Evaluates the current board position.
     *
     * Computes a score based on material balance and positional advantages using piece-square tables.
     *
     * @return float A floating-point score representing the board evaluation.
     */
    float evaluate() const;

    /**
     * @brief Provides a terminal game state score.
     *
     * Returns a large positive value if the current player is in a winning mate position,
     * a large negative value for a loss, or zero for a stalemate.
     *
     * @param depth The search depth at which the terminal state was reached (used for tie-breaking).
     * @return float The evaluation score for the terminal state.
     */
    float endResultScore(int depth) const;

    /**
     * @brief Scores a move for move ordering.
     *
     * Calculates a priority score for a move based on potential capture value, promotion, and other factors.
     *
     * @param move The move to be scored.
     * @return int An integer representing the move's priority.
     */
    int scoreMove(const Move& move) const;

    /**
     * @brief Orders a list of moves in descending order based on their score.
     *
     * Uses the scoreMove function to sort moves so that higher priority moves are evaluated first.
     *
     * @param moves The vector of moves to be ordered.
     */
    void orderMoves(std::vector<Move>& moves) const;

    /**
     * @brief Executes a minimax search with alpha-beta pruning.
     *
     * Recursively evaluates moves to a specified depth while pruning branches that cannot affect the final decision.
     * A time limit is enforced; if exceeded, a TimeLimitExceeded exception is thrown.
     *
     * @param depth The maximum search depth.
     * @param alpha The current alpha value for pruning.
     * @param beta The current beta value for pruning.
     * @param startTime The time point when the search started.
     * @param timeLimitMs The time limit in milliseconds for the search.
     * @return MinimaxValue The best evaluation score and associated move.
     * @throws TimeLimitExceeded if the search time exceeds the allowed limit.
     */
    MinimaxValue minimax(int depth, float alpha, float beta, const std::chrono::steady_clock::time_point& startTime, int timeLimitMs);

    /**
     * @brief Executes a parallel minimax search at the root level.
     *
     * Uses asynchronous tasks to evaluate each legal move concurrently, then selects the best move.
     *
     * @param depth The maximum search depth.
     * @param startTime The time point when the search started.
     * @param timeLimitMs The time limit in milliseconds for the search.
     * @return MinimaxValue The best move found along with its evaluation score.
     * @throws TimeLimitExceeded if the search exceeds the allowed time limit.
     */
    MinimaxValue parallelMinimax(int depth, const std::chrono::steady_clock::time_point& startTime, int timeLimitMs);


    /**
     * @brief Performs iterative deepening using the minimax search.
     *
     * Repeatedly increases the search depth (up to maxDepth) while using parallel search at the root.
     * This allows a move to be returned quickly if time is constrained.
     *
     * @param maxDepth The maximum depth to search.
     * @param timeLimitMs The time limit in milliseconds for the search.
     * @return MinimaxValue The best move found and its corresponding evaluation score.
     */
    MinimaxValue iterativeDeepening(int maxDepth, int timeLimitMs);

    /**
     * @brief Clears the board by setting all squares to empty.
     *
     * Resets the board state by marking every square as having no piece.
     */
    void emptyBoard();

    /**
     * @brief Inserts a test piece at a specified location on the board.
     *
     * Mainly used for testing or debugging, this function places a given piece at the indicated row and column.
     *
     * @param r The row index for the test piece.
     * @param c The column index for the test piece.
     * @param piece The piece identifier to insert.
     */
    void insertTestPiece(int r, int c, int piece);

    /**
     * @brief Prints the current board state to the console.
     *
     * Outputs a visual representation of the chessboard including file (a-h) and rank (1-8) labels.
     */
    void printBoard(Move lastMove = Move()) const;
};
