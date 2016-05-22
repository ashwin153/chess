#ifndef AI_ENGINE_H
#define AI_ENGINE_H

#include "../core/game.h"

namespace chess {

/*! Represents a chess engine.
 * The goal is to make this chess engine is modular as possible
 * so that various statistical and machine learning techniques
 * can be easily applied. The goal of the engine is, given a
 * chess board, it should be able to decide the best next move.
 */
struct Engine {	

	/*! Move selection
	 * The goal of any chess engine is to decide what the best possible move
	 * is given the current state of the board. While the manner in which
	 * chess engines determine the optimal move may be different, the ultimate
	 * goal is the same.
	 * @param[in] game Chess game
	 * @return Optimal move
	 */
	Move select(const Game& game) = 0;

};

} // namespace chess

#endif // AI_ENGINE_H
