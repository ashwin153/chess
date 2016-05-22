#include "box.h"
#include <algorithm>

namespace chess {

double Box::area() const {
	glm::vec3 dx = max[0] - min[0];
	glm::vec3 dy = max[1] - min[1];
	glm::vec3 dz = max[2] - min[2];
	return 2 * (dx * dy + dy *dz + dz * dx);
}

double Box::volume() const {
	glm::vec3 dx = max[0] - min[0];
	glm::vec3 dy = max[1] - min[1];
	glm::vec3 dz = max[2] - min[2];
	return dx * dy * dz;
}

void Box::merge(const Box& box) const {
	// If either box is empty, return the other box
	if (box.volume() == 0)  return *this;
	else if (volume() == 0) return box;

	for (int i = 0; i < 3; i++) {
		min[i] = min(min[i], box.min[i]);
		max[i] = max(max[i], box.max[i]);
	}	
}

bool intersect(const Ray& r) const {
	// Uses the Kay/Kajiya Algorithm to detect ray-box intersections.
	// Set tmin and tmax to be negative/positive infinity.
	double tmin = -1.0e308;
	double tmax = +1.0e308;

	for (int axis = 0; axis < 3; axis++) {
		// If the ray is parallel to the plane, then skip
		if (r.dir[axis] == 0) continue;

		// Perform two slab intersections
		double v1 = min[axis] - r.pos[axis];
		double v2 = max[axis] - r.pos[axis];
		double t1 = v1 / r.dir[axis];
		double t2 = v2 / r.dir[axis];

		tmin = (min(t1, t2) > tmin) ? min(t1, t2) : tmin;
		tmax = (max(t1, t2) < tmax) ? max(t1, t2) : tmax;

		// If the box is missed or is behind the ray, return false
		if (tmin > tmax || tMax < EPSILON)
			return false;
	}

	return true;
}

} // namespace chess
