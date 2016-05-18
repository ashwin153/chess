#ifndef AI_SAMPLE_H
#define AI_SAMPLE_H

#include "core/move.h"

#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

namespace chess {

/*! Training Sample
 * This class represents a serializable training sample. It allows chess games to
 * be saved and recovered from disk, while preserving all game information.
 */
class Sample {
private:
	std::vector<Move> _moves; 	
	int _white_elo; 			
	int _black_elo; 			
	int _result; 				

	/*! Boost Serialization
	 * Writes this object to the specified archive using boost serialization.
	 * This allows Samples to be easier saved and loaded from file and enables
	 * the creation of samples to be independent of their use.
	 */
	friend class boost::serialization::access;
	template <typename Archive>
	inline void serialize(Archive& ar, const unsigned int version) {
		ar & _moves & _white_elo & _black_elo & _result;
	}

public:
	/*! Construct Default Sample
	 * Boost serialization expects there to be a default constructor, so that
	 * it can create a temporary object when deserializing.
	 */
	Sample() = default;

	/*! Construct Sample
	 * Constructs a sample from the specified parameters. Note: samples are
	 * immutable by design; therefore, samples may not be modified after
	 * construction.
	 */
	Sample(std::vector<Move> moves, int welo, int belo, int res)
		: _moves(moves), _white_elo(welo), _black_elo(belo), _result(res) {}

	/*! Move Sequence
	 * Returns the sequence of moves in this chess game. Can be replayed to
	 * regenerate a playable chess game.
	 * @return Sequence of moves
	 */
	inline std::vector<Move> moves() const {
		return _moves;
	}

	/*! White Rating
	 * Returns the ELO of the white player. White moves will be weighted by
	 * the average ELO of white players who made the move.
	 * @return White ELO
	 */
	inline int white_elo() const {
		return _white_elo;
	}

	/*! Black Rating
	 * Returns the ELO of the black player. Black moves will be weighted by
	 * the average ELO of black players who made the move.
	 * @return Black ELO
	 */
	inline int black_elo() const {
		return _black_elo;
	}

	/*! Game Result
	 * The result of the particular game; 1 indicates a white win, 0 indicates
	 * a stalemate, and -1 indicates a black win.
	 * @return Result of game (-1, 0, or 1)
	 */
	inline int result() const {
		return _result;
	}	
};

}

#endif // AI_SAMPLE_H
