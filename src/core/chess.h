#ifndef GAME_H
#define GAME_H

#include "player.h"
#include <vector>

namespace chess {

/*! Represents a chess move. 
 * Used to store the sequence of moves in a game. Games can be
 * replayed by simply repeating the same sequence of moves to
 * a default board. 
 */
struct Move {
	Position cur;
	Position nxt;
};

/*! Represents the state of a given chess game. */
class Chess {
private:
	Player* _white;
	Player* _black;
	std::vector<Move> _history;
	
public:
	/*! Creates a default chess game with default pieces. */
	Chess();

	/*! Creates a chess game with pieces in the same places. */
	Chess(std::vector<Move> history);
 		
	/*! Copies a chess game from an existing chess game. */
	Chess(const Chess& chess) : Chess(chess._history) {}

	virtual ~Chess();	
};

} // namespace chess

#endif // CHESS_H
