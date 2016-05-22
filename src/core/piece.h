#ifndef CORE_PIECE_H
#define CORE_PIECE_H

#include "position.h"
#include "player.h"
#include <vector>
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
	/*! Constructs a piece at the specified position on the board. */
	Piece(Player& owner, Position loc) 
		: _owner(owner), _loc(loc), _org(loc) {}

	/*! All polymorphic types need a virtual destructor. */	
	virtual ~Piece() {}

	/*! Returns the player who controls this piece. */
	inline Player& owner() const {
		return _owner;
	}

	/*! Returns the current location of this piece. */
	inline Position loc() const {
		return _loc;
	}

	/*! Returns whether or not the piece has been moved. */
	inline bool has_moved() const {
		return _loc != _org;
	}
	
	/*! Returns a string representation of this piece. */
	inline virtual std::string to_string() const {
		return " ";
	}

	/*! Move the piece to the specified position.
	 * @param[in] pos Position
	 */
	virtual void move(const Position& pos);

	/*! Undos a move to the specified position.
	 * @param[in] pos Position/ 
	 */
	virtual void undo(const Position& pos);

	/*! Verifies that the specified position is a valid move.
     * @param[in] pos Position
	 * @return True if valid move, false otherwise
	 */
	virtual bool valid(const Position& pos);

};

/*! Pawn Piece
 * This class represents a pawn piece. Pawns may only move forward
 * in a particular direction, may move twice in certain circumstances,
 * and may move diagonally to capture.
 */
class Pawn : public Piece {
public:
	Pawn(Player& owner, Position loc) : Piece(owner, loc) {}
	void move(const Position& pos);
	bool valid(const Position& pos);
	inline std::string to_string() const {
		return (owner().is_white()) ? "♙" : "♟";
	}
};

/*! Knight Piece
 * This class represents the knight piece. Knights move in L's.
 */
class Knight : public Piece {
public:
	Knight(Player& owner, Position loc) : Piece(owner, loc) {}
	bool valid(const Position& pos);
	inline std::string to_string() const {
		return (owner().is_white()) ? "♘" : "♞";
	}
};

/*! Bishop Piece
 * This class represents the bishop piece. Bishops move diagonally.
 */
class Bishop : virtual public Piece {
public:
	Bishop(Player& owner, Position loc) : Piece(owner, loc) {}
	bool valid(const Position& pos);
	inline std::string to_string() const {
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
	bool valid(const Position& pos);	
	inline std::string to_string() const {
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
	bool valid(const Position& pos);
	
	inline std::string to_string() const {
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
	void move(const Position& pos);
	void undo(const Position& pos);
	bool valid(const Position& pos);
	inline std::string to_string() const {
		return (owner().is_white()) ? "♔" : "♚";
	}
};
 
} // namespace chess

#endif // CORE_PIECE_H
