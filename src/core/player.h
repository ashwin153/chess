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

	/*! Returns the color of the player. */
	inline virtual bool is_white() const {
		return _is_white;
	}

	/*! Returns the player's opponent. */
	inline virtual Player* opponent() const {
		return _opponent;
	}

	/*! Spawns a new live piece (used for promotion). */
	inline virtual void spawn(Piece* piece) {
		_live.push_back(piece);
	}

	/*! Returns whether or not the player is in check after move. */
	virtual bool in_check(const Move& move);

	/*! Returns the live piece at the specified position.
	 * @param[in] pos Position
	 * @return Pointer to piece or nullptr if no such piece exists.
	 */
	virtual Piece* piece(const Position& pos) const;

	/*! Captures any live piece at the specified location. 
	 * @param[in] pos Position
	 * @return Pointer to captured piece or nullptr */
	virtual Piece* capture(const Position& pos);

	/*! Uncaptures any dead piece at the specified location. 
	 * @param[in] pos Position
	 * @return Pointer to uncaptured piece or nullptr. */
	virtual Piece* uncapture(const Position& pos);
};

} // namespace chess

#endif // CORE_PLAYER_H
