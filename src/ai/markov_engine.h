#ifndef AI_MARKOV_ENGINE_H
#define AI_MARKOV_ENGINE_H

#include "engine.h"

namespace chess {

/*!
 * This engine uses a Markov Chain to perform optimal move selection. Transition
 * probabilities are based on how often a move choice leads to a favorable
 * outcome (1, 0, -1 for win, stalemate, or loss) and the strength of the players
 * who chose to make the move (moves taken by "good" players should be higher
 * weighted than moves taken by relatively worse players).
 */
class MarkovEngine : public Engine {
private:

public:

};

} // namespace chess

#endif // AI_MARKOV_ENGINE_H
