#ifndef GL_RAY_H
#define GL_RAY_H

#define EPSILON 0.001

#include <glm/glm.hpp>

namespace chess {

/*!
 * This struct parameterizes a ray in the following form: r = pos + t * dir. 
 * This allows us to easily specify the position of the ray at any timestep t.
 */
class Ray {
private:
	glm::vec3 _pos;
	glm::vec3 _dir;

public:
	/*!
	 * Constructs a ray at the specified position pointing in the specified
	 * direction. The direction vector should be normalized.
	 * @param[in] pos Position vector
	 * @param[in] dir Normalized direction vector
	 */
	Ray(glm::vec3 pos, glm::vec3 dir) : _pos(pos), _dir(dir) {}

	/*!	
	 * Returns the ray location at the specified time.
	 * @param[in] t Time
	 * @return Position of ray
	 */
	inline glm::vec3 at(const float& t) const {
		return _pos + t * _dir;
	}

	/*!
	 * Returns the direction the ray is pointing in.
	 * @return Direction of ray
	 */
	inline glm::vec3 dir() const {
		return _dir;
	}

	inline Ray& operator=(const Ray& rhs) {
		_pos = rhs.at(0);
		_dir = rhs.dir();
		return *this;
	}	
};

} // namespace chess

#endif // GL_RAY_H
