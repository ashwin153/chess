#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include "noise.h"
#include <GL/glew.h>

namespace chess {

class Texture {
private:
	int _width;
	int _height;
	std::vector<uint8_t> _bytes;
	
	GLuint _texture;
	GLuint _sampler;

public:
	/*! Construct Texture
	 * Creates a new texture of the specified width and height from the bytes
	 * bytes specified. Textures can be attached at render time to texture units.
	 */
	Texture(int width, int height, std::vector<uint8_t> bytes);

	/*! Texture Location
	 * Returns the location of the bound texture.
	 * @return Texture location
	 */
	inline GLuint texture() const {
		return _texture;
	}

	/*! Sampler Location
	 * Returns the location of the bound texture sampler.
	 * @return Sampler location
	 */
	inline GLuint sampler() const {
		return _sampler;
	}

	/*! Texture Width
	 * Returns the width of the texture in pixels
	 * @return Texture width
	 */
	inline int width() const {
		return _width;
	}

	/*! Texture Height
	 * Returns the height of the texutre in pixels
	 * @return Texture height
	 */
	inline int height() const {
		return _height;
	}

	/*! Texture Bytes
	 * Returns the content of this texture in bytes
	 * @return Texture content
	 */
	inline std::vector<uint8_t> bytes() const {
		return _bytes;
	}

};

/*! Texture Generator
 * This class is used to procedurally generate textures. It uses Perlin Noise
 * by default and exposes a bunch of predefined texture generating functions.
 */
class TextureGenerator {
private:
	PerlinNoise* _perlin;	
	int _width;
	int _height;
	
	/*! Turbulence
	 * Turbulence allows us to generate more natural frequencies because there
	 * are more frequencies present.
	 * @param[in] coord Coordinates
	 * @param[in] size Size of turbulence
	 * @return Generated turbulence
	 */
	float turb(glm::vec3 coord, float size) const;

public:
	/*! Construct Texture Generator
	 * Creates a texture generator that creates textures of the specified
	 * width and height using Perlin Noise as the source of randomness.
	 */
	TextureGenerator(int w, int h) : _width(w), _height(h) {
		_perlin = new PerlinNoise();
	}

	/*! Destruct Texture Generator
	 * Destroys the texture generator and its Perlin Noise calculator.
	 */
	virtual ~TextureGenerator() {
		delete _perlin;
	}

	/*! Marble Texture
	 * Procedurally generates a marble texture.
	 * @return Marble texture
	 */
	Texture* marble(float freq, float amp, float size) const;
};

} // namespace chess

#endif // GL_TEXTURE_H
