#ifndef CORE_PLAYER_H
#define CORE_PLAYER_H

#include "position.h"
#include <vector>

namespace chess {

// Forward declaration of Piece to resolve circular dependencies
class Piece;

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
	bool _is_white;

public:
	/*! Creates a default player. */
	Player() : _opponent(nullptr), _is_white(false) {}
	
	/*! Creates a default player of the specified color. */
	Player(bool is_white);

	/*! Creates a default player with the specified opponent. */
	Player(Player* opponent);
	
	virtual ~Player();

	inline bool is_white() { return _is_white; }
	inline Player* opponent() { return _opponent; }

	/*! Returns the live piece at the specified position.
	 * @param[in] pos Position
	 * @return Point to piece or nullptr if no such piece exists.
	 */
	virtual Piece* piece(const Position& pos);

	/*! Captures the specified piece.
	 * Moves the piece to the dead set if it exists in the live set.
	 * @param[in] piece Captured piece
	 * @return True if capture was successful, false otherwise.
	 */
	virtual bool capture(Piece* piece);

};

}

#endif // CORE_PLAYER_H
