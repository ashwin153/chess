#ifndef GL_LIGHT_H
#define GL_LIGHT_H

#include <vector>
#include <glm/glm.hpp>

namespace chess {

/*! Light
 * This class represents an OpenGL Light. Lights have position (4D), diffuse
 * color (3D), and attenuation (3D). Lights can be moved around in space and
 * the values of their parameters can be freely adjusted.
 */
class Light {
private:
	std::vector<glm::vec4> _coord; 	// Light Position
	std::vector<glm::vec3> _color; 	// Diffuse Color
	std::vector<glm::vec3> _atten; 	// Attenuation

public:
	/*! Construct Lights
	 * By defaults lights are perfectly attenuated white lights at the origin.
	 * Therefore, the only way for a light to actually have an effect is to
	 * change its corresponding values in the coord, color, and atten arrays.
	 */
	Light() {
		for (int i = 0; i < 8; i++) {
			_coord.push_back(glm::vec4(0, 0, 0, 1));
			_color.push_back(glm::vec3(1, 1, 1));
			_atten.push_back(glm::vec3(0, 0, 0));
		}
	}

	/*! Coordinates
	 * Sets the position of the specified light in world coordinates.
	 * @param[in] coord Position
	 */
	inline void coord(int i, glm::vec4 coord) {
		if (i >= 0 && i < 8)
			_coord[i] = coord;
	}

	inline std::vector<glm::vec4> coord() const {
		return _coord;
	}

	/*! Color
	 * Sets the color of the specified light in normalized RGB. By default the
	 * color of all lights in white (1, 1, 1).
	 * @param[in] color Color
	 */
	inline void color(int i, glm::vec3 color) {
		if (i >= 0 && i < 8)
			_color[i] = color;
	}

	inline std::vector<glm::vec3> color() const {
		return _color;
	}

	/*! Attenuation
	 * Sets the attenuation o the specified light. By default the attenuation
	 * of all lights is set to (1, 1, 1); this implies that the light is
	 * perfectly unattenuated.
	 * @param[in] atten Attenuation
	 */
	inline void atten(int i, glm::vec3 atten) {
		if (i >= 0 && i < 8)
			_atten[i] = atten;
	}

	inline std::vector<glm::vec3> atten() const {
		return _atten;
	}

};

} // namespace chess

#endif // GL_LIGHT_H
