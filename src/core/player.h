#ifndef CORE_PLAYER_H
#define CORE_PLAYER_H

#include "position.h"
#include <vector>

namespace chess {

/* Forward declaration resolves circular dependencies */
class Piece;
class King;

/*! Represents a chess game player.
 * Players have sets of live and dead pieces as well as an
 * opponent that they place against. Note: later revisions of this
 * class should include timers to support speed chess.
 */
class Player {
private:
	std::vector<Piece*> _pieces;
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
	virtual bool is_white();

	/*! Returns the player's opponent. */
	virtual Player* opponent();

	/*! Returns whether or not the player is currently in check. */
	virtual bool in_check();

	/*! Returns the live piece at the specified position.
	 * @param[in] pos Position
	 * @return Point to piece or nullptr if no such piece exists.
	 */
	virtual Piece* piece(const Position& pos);
};

} // namespace chess

#endif // CORE_PLAYER_H
