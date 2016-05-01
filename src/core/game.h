#ifndef CORE_GAME_H
#define CORE_GAME_H

#include "player.h"
#include "piece.h"
#include <vector>
#include <string>

namespace chess {

/*! Chess Move 
 * Used to store the sequence of moves in a game. Games can be replayed by 
 * simply repeating the same sequence of moves to a default board. 
 */
struct Move {
	Position cur;
	Position nxt;

	Move(Position cur, Position nxt) : cur(cur), nxt(nxt) {}
	Move(const Move& move) : cur(move.cur), nxt(move.nxt) {}
};

/*! Chess Game
 * Represents a chess game. The game state is immutatble. Games can be replayed 
 * using the step-back functionality. Any changes to the game state itself 
 * produces a new game. This immutability will make it much easier to 
 * parallelize algorithms in the future.
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

	/*! Construct Chess Game from PGN
	 * Constructs  a Chess game from the specified PGN string. This method should
	 * be relatively resiliant to valid PGN input.
	 */
	Game(const std::string& pgn);

	/*! Copies a chess game from an existing chess game. */
	Game(const Game& game) : Game(game._history) {}

	virtual ~Game();	

	/*! White Player
	 * @return Player 1
	 */
	inline Player* white() const {
		return _white;
	}

	/*! Black Player
	 * @return Player 2
	 */
	inline Player* black() const {
		return _black;
	}

	/*! Active Player
	 * Returns the player whose turn it is to play next. The next player is white
	 * on even valued turns and black on odd value turns.
	 * @return Next player to move
	 */
	inline Player* next() const {
		return (_turn % 2 == 0) ? _white : _black;
	}
	
	/*! Stringify
	 * Returns a string representation of the game. This is used to display the
	 * current state of the game, but should not be used to infer information
	 * about the game state itself.
	 * @return String representation
	 */
	std::string to_string() const;
	
	/* Increment Turn Counter
	 * Changes the state of the game to be the state "times" turns forward.
	 * @param[in] times Number of steps
	 */
	void step(int times);

	/*! Decrement Turn Counter
	 * Changes the state of the game to be the state "times" turns ago.
	 * @param[in] times Number of steps
	 */
	void back(int times);

	/* Make Move
	 * Attempts to make the specified move. If the move is valid, this method
	 * makes the move and returns true; if the move is invalid, this method
	 * maintains the current state of the game and returns false.
	 * @return True if successful move, False otherwise
	 */
	bool make(const Move& move);

};

} // namespace chess

#endif // CORE_GAME_H
