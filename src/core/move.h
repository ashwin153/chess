#ifndef CORE_MOVE_H
#define CORE_MOVE_H

#include "position.h"
#include <tuple>

namespace chess {

/*! Move Type
 * Specifies the type of move. Different moves are handled differently, and the
 * move type enumeration provides information to the game code as to how to
 * execute a particular move.
 */
enum class MoveType {
	kDefault,
	kCastleKingside,
	kCastleQueenside,
	kEnpassant,
	kPromoteQueen,
	kPromoteKnight,
	kPromoteBishop,
	kPromoteRook
};

/*! Chess Move
 * This class represents a move of a piece at a specified current location to a
 * specified next location. Moves may be of different types; these types help
 * the game code understand how to execute the move (castle kingside additionally
 * requires the kingside rook to be moved; captures require captured pieces to
 * be marked dead, ect.).
 */
struct Move {
	MoveType type;
	Position cur;
	Position nxt;

	/*! Construct Move
	 * Builds a move of the specified type that represents a chess move of a
	 * piece located at cur to a piece located at nxt.
	 */
	Move(MoveType type, Position cur, Position nxt) 
		: type(type), cur(cur), nxt(nxt) {}

	/*! Overload Equals Operator */
	inline bool operator==(const Move& move) const {
		return type == move.type && cur == move.cur && nxt == move.nxt;
	}

	/*! Overload Less Than Operator */
	inline bool operator<(const Move& move) const {
		return std::tie(type, cur, nxt) < std::tie(move.type, move.cur, move.nxt);
	}

	/*! Overload Not Equals Operator */
	inline bool operator!=(const Move& move) const {
		return !(*this == move);
	}	
};

}

#endif // CORE_MOVE_H

