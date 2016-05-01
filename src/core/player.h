#ifndef CORE_PLAYER_H
#define CORE_PLAYER_H

#include "position.h"
#include <vector>

namespace chess {

/* Forward declaration resolves circular dependencies */
class Piece;
class King;
struct Move;

/*! Represents a chess game player.
 * Players have sets of live and dead pieces as well as an
 * opponent that they place against. Note: later revisions of this
 * class should include timers to support speed chess.
 */
class Player {
private:
	std::vector<Piece*> _live;
	std::vector<Piece*> _dead;
	Player* _opponent;
	King* _king;
	bool _is_white;

protected:
	/*! Protected default constructor.
	 * Used solely to create mocked player objects. Typing the protected
	 * label hurts my soul - I'm a firm believer that it has no place in
	 * well designed object oriented code. However, in this case it allows
	 * for less interdependent unit tests and higher testability is something
	 * I can always get behind.
	 */
	Player() : _opponent(nullptr), _king(nullptr), _is_white(false) {}

public:
	Player(bool is_white);
	Player(Player* opponent);
	virtual ~Player();

	/*! Player Color
	 * Returns true if the player is white and false otherwise.
	 * @return True if white, False if black
	 */
	inline virtual bool is_white() const {
		return _is_white;
	}

	/*! Opponent
	 * Returns the a pointer to the player's opponent.
	 * @return Opposing player
	 */
	inline virtual Player* opponent() const {
		return _opponent;
	}

	/*! Spawn Piece
	 * Spawns the specified piece. Adds it to the live list.
	 * @param[in] piece Spawned piece
	 */
	inline virtual void spawn(Piece* piece) {
		_live.push_back(piece);
	}

	/*! In Check
	 * Returns true if the player is in check and false otherwise.
	 * @return True if in check, False otherwise
	 */
	virtual bool in_check(const Move& move);

	/*! Captures any live piece at the specified location. 
	 * @param[in] pos Position
	 * @return Pointer to captured piece or nullptr */
	virtual Piece* capture(const Position& pos);

	/*! Uncaptures any dead piece at the specified location. 
	 * @param[in] pos Position
	 * @return Pointer to uncaptured piece or nullptr. */
	virtual Piece* uncapture(const Position& pos);
	
	/*! Returns the live piece at the specified position.
	 * @param[in] pos Position
	 * @return Pointer to piece or nullptr if no such piece exists.
	 */
	virtual Piece* piece(const Position& pos) const;

	/*! Pieces of Type
	 * Returns all pieces of the specified piece type. Uses dyanmic_cast to
	 * attempt to cast piece pointers to the specified type.
	 * @return All live pieces of the specified type.
	 */
	template <typename T>
	inline std::vector<Piece*> piece() const {
		std::vector<Piece*> pieces;
		for (int i = 0; i < _live.size(); i++)
			if (dynamic_cast<T*>(_live[i]))
				pieces.push_back(_live[i]);
		return pieces;
	}
};

} // namespace chess

#endif // CORE_PLAYER_H
