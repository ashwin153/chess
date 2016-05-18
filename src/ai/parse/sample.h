#ifndef AI_SAMPLE_H
#define AI_SAMPLE_H

#include "core/move.h"

#include <vector>

#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

namespace chess {

/*!
 * This class represents a serializable training sample. It allows chess games to
 * be saved and recovered from disk, while preserving all game information.
 * Samples are immutable; this prevents malicious code from tampering with the
 * training samples. Once a sample has been created, it can never be modified.
 * Samples can be saved to and from disk using boost serialization.
 */
struct Sample {
	const std::vector<Move> moves; 	
	const int result; 				
	const int white_elo; 			
	const int black_elo; 			

	/*!
	 * Construct a sample with the specified move sequence and result with
	 * default player ratings. The default player rating is the median USCF
	 * rating taken from the September 2013 Golden Database. 
	 * 
	 * Source: http://chess.stackexchange.com/a/2750
	 * @param[in] moves Move sequence.
	 * @param[in] res Game result (1 white win, 0 stalemate, -1 black win).
	 */
	Sample(std::vector<Move> moves, int res) : Sample(moves, res, 1390, 1390) {}

	/*!
	 * Construct a sample with the specified move sequence, result, and player
	 * ratings. These player ratings are used to weight training samples. Moves
	 * made by skilled players will be weighted more than moves made by lower
	 * rated players.
	 * @param[in] moves Move sequence.
	 * @param[in] res Game result (1 white win, 0 stalemate, -1 black win).
	 * @param[in] welo White rating (ELO).
	 * @param[in] belo Black rating (ELO).
	 */
	Sample(std::vector<Move> moves, int res, int welo, int belo) :
	   moves(moves), result(res), white_elo(welo), black_elo(belo) {}	
};

} // namespace chess

namespace boost {
namespace serialization {

/*!
 * Writes this object to the specified archive using boost serialization.
 * This allows Samples to be easier saved and loaded from file and enables
 * the creation of samples to be independent of their use. Because the Sample
 * class does not expose a default constructor, implementation of this method is
 * delegated to the save_construct_data and load_construct_data methods.
 * @param[in, out] ar Boost archive.
 * @param[in] samp Serialized/Deserialized sample.
 * @param[in] ver Archive version.
 */
template <typename Archive>
inline void serialize(Archive& ar, chess::Sample& samp, const unsigned int ver) {}

/*!
 * Saves the sample to the specified archive. Note: the order in which members
 * are saved matters; variables must be loaded in the same order when loaded.
 * @param[in, out] ar Boost archive.
 * @param[in] samp Sample to serialize.
 * @param[in] ver Archive version.
 */
template <typename Archive>
inline void save_construct_data(Archive& ar,
		const chess::Sample* samp, const unsigned int ver) {
	ar << samp->moves << sampe->result << samp->white_elo << samp->black_elo;
}

/*!
 * Loads the sample from the serialized archive. Because Sample does not expose a
 * default constructor, we must manually construct samples from the archive file.
 * @param[in] ar Boost archive.
 * @param[in, out] samp Deserialized sample.
 * @param[in] ver Archive version.
 */
template <typename Archive>
inline void load_construct_data(Archive& ar,
		chess::Sample* samp, const unsigned int ver) {
	std::vector<Move> moves;
	int result, white_elo, black_elo;
	ar >> moves >> result >> white_elo >> black_elo;
	::new(samp) chess::Sample(moves, result, white_elo, black_elo);
}

} // namespace serialization
} // namespace boost	

#endif // AI_SAMPLE_H
