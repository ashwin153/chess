#ifndef AI_SAMPLE_H
#define AI_SAMPLE_H

#include "core/move.h"
#include <vector>
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
	std::vector<Move> moves; 	
	int result; 				
	int white_elo; 			
	int black_elo; 			
};

} // namespace chess

namespace boost {
namespace serialization {

/*!
 * Writes this object to the specified archive using boost serialization.
 * This allows Samples to be easier saved and loaded from file and enables
 * the creation of samples to be independent of their use.
 * @param[in, out] ar Boost archive.
 * @param[in] samp Serialized/Deserialized sample.
 * @param[in] ver Archive version.
 */
template <typename Archive>
inline void serialize(Archive& ar, chess::Sample& samp, const unsigned int ver) {
	ar & samp.moves & samp.result & samp.white_elo & samp.black_elo;
}

} // namespace serialization
} // namespace boost	

#endif // AI_SAMPLE_H
