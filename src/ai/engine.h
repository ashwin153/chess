#ifndef AI_ENGINE_H
#define AI_ENGINE_H

#include "core/move.h"

#include <set>

namespace chess {

/*! 
 * This class represents a chess engine. The goal is to make chess engines as 
 * modular as possible so that various statistical and machine learning 
 * techniques can be implemented as tested with minimal code changes. The goal 
 * of any engine is to select the optimal move given a set of candidates.
 */
struct Engine {	
	/*!
	 * The goal of any chess engine is to decide what the best possible move
	 * given a set of candidate moves and a current board positions. While
	 * different chess engines select this optimal move differently, all chess
	 * engines share the same ultimate goal.
	 * @param[in] moves Candidate moves.
	 * @return Optimal move.
	 */
	virtual Move select(std::set<Move> moves) = 0;

};

} // namespace chess

#endif // AI_ENGINE_H
