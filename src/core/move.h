#ifndef CORE_MOVE_H
#define CORE_MOVE_H

#include "position.h"

#include <tuple>
#include <functional>
#include <type_traits>

#include <boost/functional/hash.hpp>
#include <boost/serialization/access.hpp>

namespace chess {

/*!
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

/*!
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

	/*!
	 * Builds a move of the specified type that represents a chess move of a
	 * piece located at cur to a piece located at nxt.
	 * @param[in] type Type of move.
	 * @param[in] cur Current position.
	 * @param[in] nxt Next position.
	 */
	Move(MoveType type, Position cur, Position nxt) 
		: type(type), cur(cur), nxt(nxt) {}

	/* Overload equals operator. */
	inline bool operator==(const Move& move) const {
		return type == move.type && cur == move.cur && nxt == move.nxt;
	}

	/* Overload less than operator. */
	inline bool operator<(const Move& move) const {
		return std::tie(type, cur, nxt) < std::tie(move.type, move.cur, move.nxt);
	}

	/* Overload not equals operator. */
	inline bool operator!=(const Move& move) const {
		return !(*this == move);
	}	
};

} // namespace chess

namespace std {

/*!
 * Defines how moves are hashed. Allows moves to be placed into unordered
 * containers like unordered_map and unordered_set and allow for O(1) lookups.
 */
template <>
struct hash<chess::Move> {
	/*!
	 * C++ probably has the most garbage way of implementing hashes for user
	 * defined objects. Not only do you have to create this random struct in
	 * namespace std (gross), but you also have to overload some random operator.
	 * @param[in] move Move to hash.
	 * @return Hashed value.
	 */
	size_t operator()(const chess::Move& move) const {
		size_t seed = 0;
		boost::hash_combine(seed, hash<int>()(static_cast<int>(move.type)));
		boost::hash_combine(seed, hash<int>()(move.cur.x));
		boost::hash_combine(seed, hash<int>()(move.cur.y));
		boost::hash_combine(seed, hash<int>()(move.nxt.x));
		boost::hash_combine(seed, hash<int>()(move.nxt.y));
		return seed;
	}
};

} // namespace std

namespace boost {
namespace serialization {

/*!
 * Writes this object to the specified archive using boost serialization.
 * Because the Move class does not expose a default constructor, implementation
 * of this method is delegated to the save_construct_data and load_construct_data
 * methods.
 * @param[in, out] ar Boost archive.
 * @param[in] move Serialized/Deserialized move.
 * @param[in] ver Archive version.
 */
template <typename Archive>
inline void serialize(Archive& ar, chess::Move& move, const unsigned int ver) {}

/*!
 * Saves the move to the specified archive. Note: the order in which members are
 * saved matters; variables must be loaded in the same order when loading.
 * @param[in, out] ar Boost archive.
 * @param[in] move Move to serialize.
 * @param[in] ver Archive version.
 */
template <typename Archive>
inline void save_construct_data(Archive& ar, 
		const chess::Move* move, const unsigned int ver) {

	ar << move->type << move->cur.x << move->cur.y << move->nxt.x << move->nxt.y;
}

/*!
 * Loads the move from the serialized archive. Because Move does not expose a 
 * default constructor, we must manually construct moves from the archive file.
 * @param[in] ar Boost archive.
 * @param[in, out] move Deserialized move.
 * @param[in] ver Archive version.
 */
template <typename Archive>
inline void load_construct_data(Archive& ar, 
		chess::Move* move, const unsigned int ver) {
	
	chess::MoveType type;
	int cx, cy, nx, ny;
	ar >> type >> cx >> cy >> nx >> ny;
	
	chess::Position cur(nx, ny);
	chess::Position nxt(nx, ny);
	::new(move) chess::Move(type, cur, nxt);
}

} // namespace serialization
} // boost

#endif // CORE_MOVE_H

