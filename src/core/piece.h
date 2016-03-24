#ifndef CORE_PIECE_H
#define CORE_PIECE_H

#include "position.h"
#include "player.h"
#include <vector>

namespace chess {

/*! Abstract base class for all chess pieces. */
class Piece {
private:
	Player& _owner;
	Position _loc;
	bool _has_moved;

public:
	/*! Constructs a piece at the specified position on the board. */
	Piece(Player& owner, Position loc) 
		: _owner(owner), _loc(loc), _has_moved(false) {}

	/*! All polymorphic types need a virtual destructor. */	
	virtual ~Piece() {}

	/*! Returns the player who controls this piece. */
	inline Player& owner() { return _owner; }

	/*! Returns the current location of this piece. */
	inline Position loc() { return _loc; }

	/*! Returns whether or not the piece has been moved. */
	inline bool has_moved() { return _has_moved; }

	/*! Attempts to move the piece to the specified position.
	 * @param[in] pos Position
	 * @return True if move was successful, false otherwise
	 */
	virtual bool move(const Position& pos);
	
	/*! Verifies that the specified position is a valid move.
     * @param[in] pos Position
	 * @return True if valid move, false otherwise
	 */
	virtual bool isValid(const Position& pos);
};

class Pawn : public Piece {
public:
	Pawn(Player& owner, Position loc) : Piece(owner, loc) {}
	bool isValid(const Position& pos);
};

class Knight : public Piece {
public:
	Knight(Player& owner, Position loc) : Piece(owner, loc) {}
	bool isValid(const Position& pos);
};

class Bishop : virtual public Piece {
public:
	Bishop(Player& owner, Position loc) : Piece(owner, loc) {}
	bool isValid(const Position& pos);
};

class Rook : virtual public Piece {
public:
	Rook(Player& owner, Position loc) : Piece(owner, loc) {}
	bool isValid(const Position& pos);
};

/*! Represents the Queen piece.
 * This class makes use of multiple inheritance to save a couple
 * lines of code. Note that a queen is allowed to move as if it
 * were either a rook or a bishop!
 */
class Queen : public Rook, public Bishop {
public:
	Queen(Player& owner, Position loc) 
		: Piece(owner, loc), Rook(owner, loc), Bishop(owner, loc) {}
	bool isValid(const Position& pos);
};

class King : public Piece {
public:
	King(Player& owner, Position loc) : Piece(owner, loc) {}
	bool move(const Position& pos);
	bool isValid(const Position& pos);
};
 
} // namespace chess

#endif // CORE_PIECE_H
