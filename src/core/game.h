#ifndef CORE_GAME_H
#define CORE_GAME_H

#include "move.h"
#include "player.h"
#include "piece.h"
#include <vector>
#include <set>
#include <string>

namespace chess {

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
	std::set<Move> _valid;
	int _turn;

	/*! Active Player
	 * Returns the player whose turn it is to play next. The next player is white
	 * on even valued turns and black on odd value turns.
	 * @return Next player to move
	 */
	inline Player* next() const {
		return (_turn % 2 == 0) ? _white : _black;
	}

public:
	/*! Default Game
	 * Constructs a chess game with the pieces in the standard chess formation.
	 * Begins the game with white's turn and with an empty move history.
	 */
	Game();

	/*! Construct Game from Move History
	 * Constructs a game by replaying the specified move history. This allows us
	 * to recreate entire games from just a sequence of moves. We only have to
	 * serialize the move history and not the entire game when saving.
	 */
	Game(std::vector<Move> history);

	/*! Copy Game
	 * Copies and replays the move history of the specified game. This will,
	 * in effect, create a deep copy of the game.
	 */
	Game(const Game& game) : Game(game._history) {}

	/*! Destruct Game
	 * Destroys the game; deletes its move history and removes its players.
	 */
 	virtual ~Game();	

	/*! Increment Turn Counter
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

	/*! Make Move PGN
	 * Attempts to convert the PGN string into a valid move. Returns true and
	 * makes the specified move if possible and returns false if the conversion
	 * failed or no such move was possible.
	 * @return True if successful, False otherwise.
	 */
	bool make(const std::string& pgn);

	/*! Stringify
	 * Returns a string representation of the game. This is used to display the
	 * current state of the game, but should not be used to infer information
	 * about the game state itself.
	 * @return String representation
	 */
	std::string to_string() const;

	/*! Playable Moves
	 * Returns all playable moves. This method filters the possible moves for
	 * the next player to the ones that actually produce valid board 
	 * combinations. It aggregates possible moves and filters out playable ones.
	 * @return Playable moves
	 */
	inline std::set<Move> moves() {
		return _valid;
	}

	/*! Playable Moves of Type
	 * Returns all playable moves that can be made by pieces of the specified
	 * type. This is used by the PGN move translator to find candidate moves
	 * for particular types of pieces.
	 * @return Playable moves of type
	 */
	template <typename T>
	inline std::set<Move> moves() {
		std::set<Move> moves;
		for (auto piece : next()->live())
			if (dynamic_cast<T*>(piece))
				for (auto move : piece->moves())
					if (!next()->in_check(move))
						moves.insert(move);
		return moves;
	}
};

} // namespace chess

#endif // CORE_GAME_H
