#ifndef GL_BOX_H
#define GL_BOX_H

#include "ray.h"

namespace chess {

/*!
 * This class represents a rectangular bounding box. It is much cheaper to
 * perform bounding box intersections than triangle mesh intersections.
 */
class Box {
private:
	glm::vec3 _min;
	glm::vec3 _max;

public:
	Box() : Box(glm::vec3(0.0), glm::vec3(0.0)) {}
	Box(glm::vec3 min, glm::vec3 max) : _min(min), _max(max) {}
	virtual ~Box() {}
	
	/*! Returns the surface area of the box. */
	double area() const;
	
	/*! Returns the volume of the box. */
	double volume() const;

	/*! Returns the midpoint of the box. */
	glm::vec3 midpoint() const;

	/*! Smallest box containing both boxes. */
	void merge(const Box& box);

	/*! Returns whether or not the box intersects the ray. */
	double intersect(const Ray& ray) const;
	
	/*! Returns the minimum bound on the box. */
	inline glm::vec3 min() const {
		return _min;
	}

	/*! Returns the maximum bound on the box. */
	inline glm::vec3 max() const {
		return _max;
	}
};

} // namespace chess

#endif // GL_BOX_H
