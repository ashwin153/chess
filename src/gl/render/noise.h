#ifndef GL_NOISE_H
#define GL_NOISE_H

#include <vector>
#include <glm/glm.hpp>

namespace chess {

/*! Perlin Noise
 * This class is used to calculate Perlin Noise. It is almost identical to
 * Ken Perlin's reference implementation (http://cs.nyu.edu/~perlin/noise/),
 * except that I ported his Java code over to C++.
 */
class PerlinNoise {
private:
	std::vector<int> _perm;

	float fade(float t) const;
	float lerp(float t, float a, float b) const;
	float grad(int hash, float x, float y, float z) const;

public:
	/*! Construct default Perlin Noise
	 * Constructs a default perlin noise object that uses the permutation
	 * provided by Ken Perlin in his reference implementation of Perlin Noise.
	 */
	PerlinNoise();
	
	/*! Construct randomized Perlin Noise
	 * Constructs a perlin noise object that uses a randomized permutation
	 * instead of the permutation provided by Ken Perlin.
	 */
	PerlinNoise(unsigned int seed);

	/*! Noise
	 * Calculates the noise at the specified coordinates.
	 * @return Perlin Noise
	 */
	float noise(glm::vec3 coord) const;
};

}

#endif // GL_NOISE_H
