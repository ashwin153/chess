#ifndef CORE_GAME_H
#define CORE_GAME_H

#include "move.h"
#include "player.h"
#include "piece.h"

#include <vector>
#include <set>
#include <string>

namespace chess {

/*!
 * Represents a chess game. Games can be replayed using the step-back 
 * functionality. Games are not immutable and are not thread-safe.
 */
class Game {
private:
	Player* _white;
	Player* _black;

	std::vector<Move> _history;
	std::set<Move> _valid;
	int _turn;

	/*!
	 * Returns the player whose turn it is to play next. The next player is white
	 * on even valued turns and black on odd value turns.
	 * @return Next player to move.
	 */
	inline Player* next() const {
		return (_turn % 2 == 0) ? _white : _black;
	}

public:
	/*!
	 * Constructs a chess game with the pieces in the standard chess formation.
	 * Begins the game with white's turn and with an empty move history.
	 */
	Game();

	/*!
	 * Constructs a game by replaying the specified move history. This allows us
	 * to recreate entire games from just a sequence of moves. We only have to
	 * serialize the move history and not the entire game when saving.
	 * @param[in] history Move history.
	 */
	Game(std::vector<Move> history);

	/*!
	 * Copies and replays the move history of the specified game. This will,
	 * in effect, create a deep copy of the game.
	 * @param[in] game Template game.
	 */
	Game(const Game& game) : Game(game._history) {}

	/*!
	 * Destroys the game; deletes its move history and removes its players.
	 * TODO: Consider replacing raw pointers with smart pointers so that
	 * memory is automatically managed.
	 */
 	virtual ~Game();	

	/*!
	 * Changes the state of the game to be the state "times" turns forward.
	 * @param[in] times Number of steps.
	 */
	void step(int times);

	/*!
	 * Changes the state of the game to be the state "times" turns ago.
	 * @param[in] times Number of steps.
	 */
	void back(int times);

  inline Player* black() const {
		return _black;
	}
  
  inline Player* white() const {
		return _white;
	}

	/*!
	 * Attempts to make the specified move. If the move is valid, this method
	 * makes the move and returns true; if the move is invalid, this method
	 * maintains the current state of the game and returns false.
	 * @return True if successful move, false otherwise.
	 */
	bool make(const Move& move);

	/*!
	 * Attempts to convert the PGN string into a valid move. Returns true and
	 * makes the specified move if possible and returns false if the conversion
	 * failed or no such move was possible.
	 * @return True if successful, false otherwise.
	 */
	bool make(const std::string& pgn);

	/*!
	 * Returns a string representation of the game. This is used to display the
	 * current state of the game, but should not be used to infer information
	 * about the game state itself.
	 * @return String representation of game.
	 */
	std::string to_string() const;

	/*!
	 * Returns the history of all played moves for this game.
	 * @return Move history.
	 */
	inline std::vector<Move> history() {
		return _history;
	}

	/*!
	 * Returns all playable moves. This method filters the possible moves for
	 * the next player to the ones that actually produce valid board 
	 * combinations. It aggregates possible moves and filters out playable ones.
	 * @return All playable moves.
	 */
	inline std::set<Move> moves() {
		return _valid;
	}

	/*!
	 * Returns all playable moves that can be made by pieces of the specified
	 * type. This is used by the PGN move translator to find candidate moves
	 * for particular types of pieces.
	 * @return Playable moves for pieces of specified type.
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
