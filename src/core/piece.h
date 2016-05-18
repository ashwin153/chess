#ifndef CORE_PIECE_H
#define CORE_PIECE_H

#include "move.h"
#include "position.h"
#include "player.h"
#include <set>
#include <string>

namespace chess {

/*!
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
	/*!
	 * Constructs a piece at the specified location and links it to the
	 * specified player. Note: this method only sets up the Piece-Player
	 * relationship and not the Player-Piece relationship. Therefore, it really
	 * only should be called by Player objects.
	 * @param[in] owner Owner of piece
	 * @param[in] loc Location of piece
	 */
	Piece(Player& owner, const Position& loc) 
		: _owner(owner), _loc(loc), _org(loc) {}

	/*!
	 * According to Item 7 in Scott Meyers' Effective C++, every polymorphic base
	 * class should have a virtual destructor.
	 */
	virtual ~Piece() {}
	
	/*!
	 * Returns true if the specified position does not contain an allied piece
	 * and is within the chess board.
	 * @param[in] pos Candidate position.
	 * @return True if valid position, false otherwise.
	 */
	bool valid(const Position& pos) const;
    
	/*
	 * Returns a collection of all the possible moves that the piece may make. 
	 * Note: that possible moves are not the same as playable moves. For example, 
	 * it might be possible for a piece to move to a position, but doing so might
	 * place its king in check. Therefore, the move is possible but not playable.
	 * Therefore, additional checks must be made to filter out unplayable moves.
	 * @return Possible moves
	 */
	virtual std::set<Move> moves() = 0;
	
	/*!
	 * Returns a string representation of this piece. Used by the textual chess
	 * game to display the contents of the board.
	 * @return String representation of piece.
	 */
	virtual std::string to_string() const = 0;
	
	/*!
	 * Returns whether or not the piece has moved from its original position.
	 * Used to determine if pawns are allowed to move forward or not.
	 * @return True if it has moved, false otherwise.
	 */
	virtual bool has_moved() const {
		return _loc != _org;
	}

	/*!
	 * Returns the player who owns this piece. Used by subclasses to acces the
	 * private owner field. This allows pieces to be aware of the other pieces
	 * on the board when making move calculations.
	 * @return Owner of piece.
	 */
	inline Player& owner() const {
		return _owner;
	}

	/*!
	 * Returns the current position of this piece on the chess board.
	 * @return Current piece location.
	 */
	inline Position loc() const {
		return _loc;
	}

	/*!
	 * Set the position of this piece to the specified position.
	 * @param[in] New piece location.
	 */
	inline void loc(const Position& pos) {
		_loc = pos;
	}
	
};

/*!
 * This class represents a pawn piece. Pawns may move (1) forward, (2) diagonal
 * to capture (enpassant and regular), (3) forward twice (if first move) and
 * may be promoted to a Queen, Rook, Knight, or Bishop if it reaches the end of
 * the board.
 */
class Pawn : public Piece {
public:
	Pawn(Player& owner, Position loc) : Piece(owner, loc) {}
	std::set<Move> moves() override;
	inline std::string to_string() const override {
		return (owner().is_white()) ? "♙" : "♟";
	}
};

/*!
 * This class represents the knight piece. Knights may move in L's. More
 * formally, Knights may move a distance of 3 tiles as long it moves at least
 * 1 tile in both the x and y directions.
 */
class Knight : public Piece {
public:
	Knight(Player& owner, Position loc) : Piece(owner, loc) {}
	std::set<Move> moves() override;	
	inline std::string to_string() const override {
		return (owner().is_white()) ? "♘" : "♞";
	}
};

/*!
 * This class represents the bishop piece. Bishops may move diagonally, but
 * they may not move through allied or enemy pieces. Therefore, a bishop that
 * begins on a particular color tile may never move to a different color tile. 
 */
class Bishop : public virtual Piece {
public:
	Bishop(Player& owner, Position loc) : Piece(owner, loc) {}
	std::set<Move> moves() override;
	inline std::string to_string() const override {
		return (owner().is_white()) ? "♗" : "♝";
	}
};

/*!
 * This class represents the rook piece. Rooks may move both vertically and
 * horizontally, but may not move through allied or enemy pieces. 
 */
class Rook : public virtual Piece {
public:
	Rook(Player& owner, Position loc) : Piece(owner, loc) {}
	std::set<Move> moves() override;
	inline std::string to_string() const override {
		return (owner().is_white()) ? "♖" : "♜";
	}
};

/*!
 * This class represents the queen piece and makes use of multiple inheritance 
 * to save a few lines of code. A queen is permitted to move anywhere that a 
 * rook or a bishop is allowed. Note: in order to make this work, we must make 
 * the Bishop and Rook inherit virtually from Piece.
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

/*!
 * This class represents the king piece. A king may move one space in any
 * direction and may move two spaces if castling. The king may not castle after
 * it or the corresponding rook has moved previously and it may not castle
 * out of, into, or through check.
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
