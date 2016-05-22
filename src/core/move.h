#ifndef CORE_MOVE_H
#define CORE_MOVE_H

#include "position.h"
#include <tuple>
#include <functional>
#include <type_traits>

namespace chess {

/*! Move Type
 * Specifies the type of move. Different moves are handled differently, and the
 * move type enumeration provides information to the game code as to how to
 * execute a particular move.
 */
enum class MoveType : int {
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

} // namespace chess

namespace std {

/*! Hash Move
 * Defines how moves are hashed. Allows moves to be placed into unordered
 * containers like unordered_map and unordered_set and allow for O(1) lookups.
 */
template <>
struct hash<chess::Move> {
private:
	/*! Combine Hashes
	 * Rather than having to link with Boost just to use the useful hash_combine
	 * function, I just copy and pasted their implementation of this method.
	 * Fucking sue me.
	 * @param[in, out] seed Hash seed
	 * @param[in] v Combine value
	 */
	template <class T>
	inline void hash_combine(std::size_t& seed, const T& v) const {
    	seed ^= hash<T>()(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
	}

public:
	/*! Hash
	 * C++ probably has the most garbage way of implementing hashes for user
	 * defined objects. Not only do you have to create this random struct in
	 * namespace std (gross), but you also have to overload some random operator.
	 * @param[in] move Move to hash
	 * @return Hashed value
	 */
	size_t operator()(const chess::Move& move) const {
		size_t seed = 0;
		hash_combine(seed, hash<int>()(static_cast<int>(move.type)));
		hash_combine(seed, hash<int>()(move.cur.x));
		hash_combine(seed, hash<int>()(move.cur.y));
		hash_combine(seed, hash<int>()(move.nxt.x));
		hash_combine(seed, hash<int>()(move.nxt.y));
		return seed;
	}
};

} // namespace std

#endif // CORE_MOVE_H

