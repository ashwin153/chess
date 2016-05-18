#ifndef GL_TRIANGLE_H
#define GL_TRIANGLE_H

#include "box.h"
#include <glm/glm.hpp>

namespace chess {

/*!
 * This class represents a single triangle face. A triangle may only exist as
 * part of a larger mesh. However, it provides a convinient abstraction for
 * performing collision detection.
 */
class Triangle {
private:
	const glm::vec3 _v1;
	const glm::vec3 _v2;
	const glm::vec3 _v3;
	Box _box;

public:
	Triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);	
	virtual ~Triangle() {}

	/*!
	 * Returns a bounding box around this triangle, because calculating ray-box
	 * intersections is much cheaper than calculating ray-triangle.
	 * @return Box containing triangle
	 */
	inline const Box& bounds() const {
		return _box;
	}

	/*!
	 * Calculates the time of intersection between the ray and this triangle.
	 * @return Time of intersection or -1 if they do not intersect.
	 */
	double intersect(const Ray& ray) const;
};

} // namespace chess

#endif // GL_TRIANGLE_H
