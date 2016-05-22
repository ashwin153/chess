#ifndef GL_BOX_H
#define GL_BOX_H

namespace chess {

/*! Box
 * This class represents a rectangular bounding box. It is much cheaper to
 * perform bounding box intersections than triangle mesh intersections.
 */
struct Box {
	glm::vec3 min;
	glm::vec3 max;

	Box() : Box(glm::vec3(0.0), glm::vec3(0.0)) {}
	Box(glm::vec3 min, glm::vec3 max) : min(min), max(max) {}
	virtual ~Box() {}
	
	/*! Returns the surface area of the box. */
	double area() const;
	
	/*! Returns the volume of the box. */
	double volume() const;

	/*! Smallest box containing both boxes. */
	void merge(const Box& box) const;
};

} // namespace chess

#endif // GL_BOX_H
