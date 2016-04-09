#ifndef AI_ENGINE_H
#define AI_ENGINE_H

#include "chess.h"

namespace chess {

/*! Represents a chess engine.
 * The goal is to make this chess engine is modular as possible
 * so that various statistical and machine learning techniques
 * can be easily applied. The goal of the engine is, given a
 * chess board, it should be able to decide the best next move.
 */
class Engine {
private:

public:
	Move select(const Chess& game);
};

} // namespace chess

#endif // AI_ENGINE_H
