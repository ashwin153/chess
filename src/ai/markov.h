#ifndef AI_MARKOV_H
#define AI_MARKOV_H

#include <random>
#include <utility>
#include <unordered_map>
#include <vector>

namespace chess {

/*! Markov Chain
 * This class represents a Markov Chain. Markov Chains are stochastic models with
 * the "memory-less" property; the probability of a state transition depends only
 * on the previous state. Markov Chains are typically implemented with a matrix
 * representing state transition probabilities. In the case of chess; however,
 * this matrix is likely to be extremely sparse. A much more memory efficient
 * model is to use a map to associate states to possible next states.
 */
template <typename T>
class MarkovChain {
private:
	std::unordered_map<std::vector<T>, 
		std::vector<std::pair<T, double>> _chain;
	std::vector<T> _seed;

public:

	/*! Construct Random Seed Markov Chain
	 * Constructs a Markov Chain from the specified map and randomly selects a
	 * seed from the set of possible states. 
	 */
	MarkovChain(std::unordered_map<std::vector<T>, 
			std::vector<std::pair<T, double>> chain); 

	/*! Construct Markov Chain
	 * Constructs a Markov Chain from the specified map and uses the specified
	 * seed as a starting point. Assumes that the specified state is a valid
	 * possible state (state in chain).
	 */
	MarkovChain(std::unordered_map<std::vector<T>,
			 std::vector<std::pair<T, double>> chain, 
			 std::vector<T> seed);

 	/*! Next State
	 * Transitions the Markov Chain to the next state by randomly selecting one
	 * of the possible next states for the current seed. This random selection
	 * is weighted by the transition probability.
	 * @return Next state
	 */
	T next();
};

}

#endif // AI_MARKOV_H
