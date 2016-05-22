#ifndef RANDOM_ENGINE_H
#define RANDOM_ENGINE_H

#include "engine.h"
#include "core/move.h"

#include <set>
#include <random>
#include <algorithm>

namespace chess {

/*! Random Engine
 * This class is a randomized chess engine. It selects moves uniformly at random
 * from a set of candidate moves. This is a good benchmark for measuring the
 * performance of more complex chess engines, because it allows us to answer the
 * question "how much better than random guessing is this engine?"
 */
class RandomEngine : public Engine {
private:
	std::mt19937 _prng;

public:
	/*! Construct Random Engine
	 * Constructs a random engine. Seeds the pseudo-random number generator
	 * that will be used by the move selector to choose moves.
	 */
	RandomEngine() : _prng(std::random_device()()) {}

	/*! Select Random Move
	 * Selects a move from the set of moves uniformly at random
	 * @return Randomly selected move
	 */
	inline Move select(std::set<Move> moves) const override {
		std::uniform_int_distribution<> dis(0, moves.size() - 1);
		std::set<int>::const_iterator it(moves.begin());
  		std::advance(it, dis(_prng));	
		return *it;
	}
};

}

#endif // RANDOM_ENGINE_H
