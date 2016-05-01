#include "texture.h"
#include "noise.h"
#include <math.h>
#include <iostream>

namespace chess {

Texture::Texture(int width, int height, std::vector<uint8_t> bytes) 
		: _width(width), _height(height), _bytes(bytes) {

	glGenTextures(1, &_texture);
	glGenSamplers(1, &_sampler);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, width, height);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, 
			GL_RGB, GL_UNSIGNED_BYTE, (const GLvoid*) &bytes[0]);
	glSamplerParameteri(_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

float TextureGenerator::turb(glm::vec3 coord, float size) const {
	// http://lodev.org/cgtutor/randomnoise.html
	float value = 0.0;
	while (size >= 1) {
		value += _perlin->noise(coord / size) * size;
		size /= 2.0;	
	}
	return value;	
}

Texture* TextureGenerator::marble(float freq, float amp, float size) const {
	std::vector<uint8_t> bytes; 
	for (int y = 0; y < _height; y++) {
		for (int x = 0; x < _width; x++) {
			// http://lodev.org/cgtutor/randomnoise.html
			float normx = (float) x / _width;
			float normy = (float) y / _height;
			glm::vec3 coord = glm::vec3(normx, normy, 0);

			float xy = normx * freq + normy * 2 * freq + amp * turb(coord, size);
    		float sine = 256 * fabs(sin(xy * M_PI));
	
			// Set the red, green, and blue components equal
			for (int i = 0; i < 3; i++)
				bytes.push_back((uint8_t) sine);
		}
	}

	// Return the generated marble texture
	return new Texture(_width, _height, bytes);
}

} // namespace chess
