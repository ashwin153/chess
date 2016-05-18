#ifndef CORE_PLAYER_H
#define CORE_PLAYER_H

#include "move.h"
#include "position.h"

#include <vector>
#include <stack>

namespace chess {

/* Forward declaration resolves circular dependencies */
class Piece;
class King;

/*!
 * This class represents a chess player. Players have sets of live and dead 
 * pieces as well as an opponent that they place against. Players are
 * responsible for making and undoing moves to pieces.
 */
class Player {
private:
	std::vector<Piece*> _live;
	std::vector<Piece*> _dead;
	std::stack<Move> _moves;

	Player* _opponent;
	King* _king;
	bool _is_white;
	
	/*!
	 * Replaces the piece at the specified position with a new piece. This method
	 * does not place the replaced piece onto the dead vector. Therefore, pieces
	 * may not be "unreplaced".
	 * @param[in] pos Position of piece to replace.
	 * @param[in] piece Replacement piece.
	 */	
	void replace(const Position& pos, Piece* piece);
	
	/*!
	 * Captures any piece at the specified position. Captured pieces may be
	 * "uncaptured" by calling uncapture on the returned piece. Not that if two
	 * pieces are captured on the same position, the piece that was captured
	 * last will be the one that is uncaptured.
	 * @param[in] position Position to capture.
	 * @return Pointer to captured piece.
	 */ 
	Piece* capture(const Position& pos);

	/*!
	 * Uncaptures any piece at the specified position. Uncaptured pieces move
	 * from the dead vector to the live vector. Returns the last dead piece to
	 * be captured at a specified position.
	 * @return Uncaptured piece.
	 */	
	Piece* uncapture(const Position& piece);
	
protected:
	/*!
	 * Used solely to create mocked player objects. Typing the protected
	 * label hurts my soul - I'm a firm believer that it has no place in well 
	 * designed object oriented code. However, in this case it allows for less 
	 * interdependent unit tests and higher testability is something I can 
	 * always get behind.
	 */
	Player() : _opponent(nullptr), _king(nullptr), _is_white(false) {}

public:
	/*!
	 * Constructs a default player with all the pieces in the standard chess
	 * formation of the specified color (true->white, false->black).
	 * @param[in] is_white Type of player to construct.
	 */
	Player(bool is_white);
	
	/*!
	 * Constructs a default opposing player to the specified player. Sets up the
	 * player-opponent relationships for both this player and its opponent. The
	 * created player is of the opposite color as the opponent.
	 * @param[in] opponent Opposing player.
	 */
	Player(Player* opponent);
	
	/*!
	 * Note: Add smart pointers so that we don't have to manually manage memory.
	 * This is fucking 2016. Reference counting exists.
	 */
	virtual ~Player();

	/*!
	 * Instructs the player to make the specified move. This method assumes that
	 * the specified move is valid and makes no effort to verify that this is
	 * in fact the case. By avoiding validity checks here, we can greatly speed
	 * up the game logic when we are certain we are making valid moves (as is
	 * the case when loading pre-existing games).
	 * @param[in] move Move to make.
	 */
	void make(const Move& move);

	/*!
	 * Undo the last move made by this player. Calling this method multiple times
	 * results in undefined behavior (player undo must be alternated). Do not
	 * directly call this method unless you know what you are doing.
	 */
	void undo();

	/*!
	 * Returns true if the player is in check after the specified move, and 
	 * false otherwise. Recall that pieces are not allowed to move if doing
	 * so would put the player in check.
	 * @param[in] move Move to test for check.
	 * @return True if in check, false otherwise.
	 */
	virtual bool in_check(const Move& move);

	/*!
	 * Returns true if the player is currently in check and false otherwise.
	 * @return True if in check, false otherwise.
	 */
	virtual bool in_check();
	
	/*!
	 * Returns the live piece at the specified position.
	 * @param[in] pos Position to search for.
	 * @return Pointer to piece or nullptr if no such piece exists.
	 */
	virtual Piece* piece(const Position& pos) const;

	/*!
	 * Returns the number of moves that the player has made.
	 * @return Number of moves made.
	 */
	virtual int turns() const {
		return _moves.size();
	}

	/*!
	 * Returns the last move made by the player.
	 * @return Last made move.
	 */
	virtual Move last() const {
		return _moves.top();
	}

	/*!
	 * Returns true if the player is white and false otherwise.
	 * @return True if white, false if black.
	 */
	virtual bool is_white() const {
		return _is_white;
	}

	/*!
	 * Returns a pointer to the player's opponent. Used by some pieces to access
	 * information about the opponent's pieces when determining validity.
	 * @return Opposing player.
	 */
	virtual Player* opponent() const {
		return _opponent;
	}

	/*!
	 * Returns a list of all the player's live pieces.
	 * @return Live pieces.
	 */
	std::vector<Piece*> live() const {
		return _live;
	}

	/*!
	 * Returns a list of all the player's dead pieces.
	 * @return Dead pieces.
	 */
	std::vector<Piece*> dead() const {
		return _dead;
	}

};

} // namespace chess

#endif // CORE_PLAYER_H
