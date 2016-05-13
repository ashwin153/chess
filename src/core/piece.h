#ifndef CORE_PIECE_H
#define CORE_PIECE_H

#include "move.h"
#include "position.h"
#include "player.h"
#include <set>
#include <string>

namespace chess {

/*! Piece
 * Abstract base class for all chess pieces. Defines the basic logic
 * for piece movement and move validity. Pieces are associated with
 * an owner who provides all the requisite when making movement decisions.
 */
class Piece {
private:
	Player& _owner;
	Position _loc;
	Position _org;

public:
	/*! Construct Piece
	 * Constructs a piece at the specified location and links it to the
	 * specified player. Note: this method only sets up the Piece-Player
	 * relationship and not the Player-Piece relationship. Therefore, it really
	 * only should be called by Player objects.
	 */
	Piece(Player& owner, const Position& loc) 
		: _owner(owner), _loc(loc), _org(loc) {}

	/*! Destruct Piece
	 * According to Item 7 in Scott Meyers' Effective C++, every polymorphic base
	 * class should have a virtual destructor.
	 */
	virtual ~Piece() {}
	
	/*! Position Validity
	 * Returns true if the specified position does not contain an allied piece
	 * and is within the chess board.
	 * @return True if valid position, False otherwise
	 */
	bool valid(const Position& pos) const;
    
	/* Possible Moves
	 * Returns a vector of all the possible moves that the piece may make. Note
	 * that possible moves are not the same as playable moves. For example, it
	 * might be possible for a piece to move to a position, but doing so might
	 * place its king in check. Therefore, the move is possible but not possible.
	 * Therefore, additional checks must be made to filter out unplayable moves.
	 * @param[in] pos Position
	 * @return Possible moves
	 */
	virtual std::set<Move> moves() = 0;
	
	/*! String Representation
	 * Returns a string representation of this piece. Used by the textual chess
	 * game to display the contents of the board.
	 * @return String representation of piece
	 */
	virtual std::string to_string() const = 0;
	
	/*! Has Moved
	 * Returns whether or not the piece has moved from its original position.
	 * Used to determine if pawns are allowed to move forward or not.
	 * @return True if it has moved, False otherwise
	 */
	virtual bool has_moved() const {
		return _loc != _org;
	}

	/*! Piece Owner
	 * Returns the player who owns this piece. Used by subclasses to acces the
	 * private owner field. This allows pieces to be aware of the other pieces
	 * on the board when making move calculations.
	 * @return Owner of piece
	 */
	inline Player& owner() const {
		return _owner;
	}

	/*! Get Location
	 * Returns the position of this piece on the chess board.
	 * @return Piece location
	 */
	inline Position loc() const {
		return _loc;
	}

	/*! Set Location
	 * Sets the position of this piece to the specified position.
	 * @return New piece location
	 */
	inline void loc(const Position& pos) {
		_loc = pos;
	}
	
};

/*! Pawn Piece
 * This class represents a pawn piece. Pawns may only move forward
 * in a particular direction, may move twice in certain circumstances,
 * and may move diagonally to capture.
 */
class Pawn : public Piece {
public:
	Pawn(Player& owner, Position loc) : Piece(owner, loc) {}
	std::set<Move> moves() override;

	inline std::string to_string() const override {
		return (owner().is_white()) ? "♙" : "♟";
	}
};

/*! Knight Piece
 * This class represents the knight piece. Knights move in L's.
 */
class Knight : public Piece {
public:
	Knight(Player& owner, Position loc) : Piece(owner, loc) {}
	std::set<Move> moves() override;
	
	inline std::string to_string() const override {
		return (owner().is_white()) ? "♘" : "♞";
	}
};

/*! Bishop Piece
 * This class represents the bishop piece. Bishops move diagonally.
 */
class Bishop : virtual public Piece {
public:
	Bishop(Player& owner, Position loc) : Piece(owner, loc) {}
	std::set<Move> moves() override;

	inline std::string to_string() const override {
		return (owner().is_white()) ? "♗" : "♝";
	}
};

/*! Rook Piece
 * This class represents the rook piece. Rooks may only move 
 * vertically and horizontally.
 */
class Rook : virtual public Piece {
public:
	Rook(Player& owner, Position loc) : Piece(owner, loc) {}
	std::set<Move> moves() override;

	inline std::string to_string() const override {
		return (owner().is_white()) ? "♖" : "♜";
	}
};

/*! Queen Piece
 * This class makes use of multiple inheritance to save a couple
 * lines of code. Note that a queen is allowed to move as if it
 * were either a rook or a bishop!
 */
class Queen : public Rook, public Bishop {
public:
	Queen(Player& owner, Position loc) 
		: Piece(owner, loc), Rook(owner, loc), Bishop(owner, loc) {}
	std::set<Move> moves() override;
	
	inline std::string to_string() const override {
		return (owner().is_white()) ? "♕" : "♛";
	}
};

/*! King Piece
 * This class implements the logic to move the king and perform
 * castling. The king may only move one space in any direction unless
 * the king is castling. The king may not castle after it or the
 * corresponding rook has moved and it may not castle out of, through, or
 * into check.
 */
class King : public Piece {
public:
	King(Player& owner, Position loc) : Piece(owner, loc) {}
	std::set<Move> moves() override;
	
	inline std::string to_string() const override {
		return (owner().is_white()) ? "♔" : "♚";
	}
};
 
} // namespace chess

#endif // CORE_PIECE_H
