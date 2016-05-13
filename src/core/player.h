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

/*! Chess Player
 * Players have sets of live and dead pieces as well as an opponent that they 
 * place against. Note: later revisions of this class should include timers to 
 * support speed chess.
 */
class Player {
private:
	std::vector<Piece*> _live;
	std::vector<Piece*> _dead;
	std::stack<Move> _moves;

	Player* _opponent;
	King* _king;
	bool _is_white;
	
	/*! Replace Piece
	 * Replaces the piece at the specified position with a new piece. This method
	 * does not place the replaced piece onto the dead vector. Therefore, pieces
	 * may not be "unreplaced"
	 * @param[in] pos Position of piece to replace
	 * @param[in] piece Replacement piece
	 */	
	void replace(const Position& pos, Piece* piece);
	
	/*! Capture Piece
	 * Captures any piece at the specified position. Captured pieces may be
	 * "uncaptured" by calling uncapture on the returned piece. Not that if two
	 * pieces are captured on the same position, the piece that was captured
	 * last will be the one that is uncaptured.
	 * @param[in] position Position to capture
	 * @return Pointer to captured piece
	 */ 
	Piece* capture(const Position& pos);

	/*! Uncapture Piece
	 * Uncaptures any piece at the specified position. Uncaptured pieces move
	 * from the dead vector to the live vector. Returns the last dead piece to
	 * be captured at a specified position.
	 * @return Uncaptured piece
	 */	
	Piece* uncapture(const Position& piece);
	
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
	/*! Default Player
	 * Constructs a default player with all the pieces in the standard chess
	 * formation of the specified color (true->white, false->black).
	 */
	Player(bool is_white);
	
	/*! Opposing Player
	 * Constructs a default opposing player to the specified player. Sets up the
	 * player-opponent relationships for both this player and its opponent. The
	 * created player is of the opposite color as the opponent.
	 */
	Player(Player* opponent);
	
	/*! Destruct Player
	 * Note: Add smart pointers so that we don't have to manually manage memory.
	 * This is fucking 2016. Reference counting exists.
	 */
	virtual ~Player();

	/*! Make move
	 * Instructs the player to make the specified move. This method assumes that
	 * the specified move is valid and makes no effort to verify that this is
	 * in fact the case. By avoiding validity checks here, we can greatly speed
	 * up the game logic when we are certain we are making valid moves (as is
	 * the case when loading pre-existing games).
	 */
	void make(const Move& move);

	/*! Undo last move
	 * Undo the last move made by this player. Calling this method multiple times
	 * results in undefined behavior (player undo must be alternated). Do not
	 * directly call this method unless you know what you are doing.
	 */
	void undo();

	/*! In Check After Move
	 * Returns true if the player is in check after the specified move, and 
	 * false otherwise. Recall that pieces are not allowed to move if doing
	 * so would put the player in check.
	 * @return True if in check, False otherwise
	 */
	virtual bool in_check(const Move& move);

	/*! In Check Currently
	 * Returns true if the player is currently in check and false otherwise.
	 * @return True if in check, False otherwise
	 */
	virtual bool in_check();
	
	/*! Piece at Location 
	 * Returns the live piece at the specified position.
	 * @param[in] pos Position
	 * @return Pointer to piece or nullptr if no such piece exists.
	 */
	virtual Piece* piece(const Position& pos) const;

	/*! Number of Moves
	 * Returns the number of moves that the player has made.
	 * @return Number of moves made
	 */
	virtual int turns() const {
		return _moves.size();
	}

	/*! Last Move
	 * Returns the last move made by the player.
	 * @return Last made move
	 */
	virtual Move last() const {
		return _moves.top();
	}

	/*! Player Color
	 * Returns true if the player is white and false otherwise.
	 * @return True if white, False if black
	 */
	virtual bool is_white() const {
		return _is_white;
	}

	/*! Player Opponent
	 * Returns a pointer to the player's opponent. Used by some pieces to access
	 * information about the opponent's pieces when determining validity.
	 * @return Opposing player
	 */
	virtual Player* opponent() const {
		return _opponent;
	}

	/*! Live Pieces
	 * Returns a list of all the player's live pieces.
	 * @return Live pieces
	 */
	std::vector<Piece*> live() const {
		return _live;
	}

	/*! Dead Pieces
	 * Returns a list of all the player's dead pieces.
	 * @return Dead pieces
	 */
	std::vector<Piece*> dead() const {
		return _dead;
	}

};

} // namespace chess

#endif // CORE_PLAYER_H
