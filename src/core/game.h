#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "piece.h"
#include <vector>
#include <string>

namespace chess {

/*! Chess Move 
 * Used to store the sequence of moves in a game. Games can be
 * replayed by simply repeating the same sequence of moves to
 * a default board. 
 */
struct Move {
	Position cur;
	Position nxt;

	// PGN Movetext Regex: ^([PNBRQK]?)([a-h]?)([1-8]?)[x]?([a-h][1-8]).*$
	Move(Position cur, Position nxt) : cur(cur), nxt(nxt) {}
	Move(const Move& move) : cur(move.cur), nxt(move.nxt) {}
};

/*! Chess Game
 * Represents a chess game. The game state is immutatble. Games can be
 * replayed using the step-back functionality. Any changes to the game
 * state itself produces a new game. This immutability will make it
 * much easier to parallelize algorithms in the future.
 */
class Game {
private:
	Player* _white;
	Player* _black;
	std::vector<Move> _history;
	int _turn;

public:
	/*! Creates a default chess game with default pieces. */
	Game();

	/*! Creates a chess game with pieces in the same places. */
	Game(std::vector<Move> history);
 		
	/*! Copies a chess game from an existing chess game. */
	Game(const Game& game) : Game(game._history) {}

	virtual ~Game();	

	/*! Returns the player whose turn is next. */
	virtual Player* next() const;
	
	/*! Returns a string representation of the current game state. */
	virtual std::string to_string() const;
	
	/*! Steps the turn counter "times" times
	 * Changes the state of the game to be the state "times" turns forward.
	 * @param[in] times Number of steps
	 */
	virtual void step(int times);

	/*! Steps the turn counter back "times" times backward
	 * Changes the state of the game to be the state "times" turns ago.
	 * @param[in] times Number of steps
	 */
	virtual void back(int times);

   	/*! Returns a new chess game with the move made. */
	virtual bool make(const Move& move);

};

} // namespace chess

#endif // GAME_H
