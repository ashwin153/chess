#include "box.h"
#include <algorithm>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace chess {

double Box::area() const {
	double dx = _max[0] - _min[0];
	double dy = _max[1] - _min[1];
	double dz = _max[2] - _min[2];
	return 2 * (dx * dy + dy *dz + dz * dx);
}

double Box::volume() const {
	double dx = _max[0] - _min[0];
	double dy = _max[1] - _min[1];
	double dz = _max[2] - _min[2];
	return dx * dy * dz;
}

void Box::merge(const Box& box) {
	// If either box is empty, make the box the other box
	if (volume() == 0) { *this = box; return; }
	else if (box.volume() == 0) return;

	// Find the minimum and maximum of both boxes along each axis
	for (int axis = 0; axis < 3; axis++) {
		_min[axis] = std::min(_min[axis], box._min[axis]);
		_max[axis] = std::max(_max[axis], box._max[axis]);
	}	
}

glm::vec3 Box::midpoint() const {
	return (_max + _min) / 2.0f;
}

double Box::intersect(const Ray& ray) const {
	// Uses the Kay/Kajiya Algorithm to detect ray-box intersections.
	// Set tmin and tmax to be negative/positive infinity.
	double tmin = -1.0e308;
	double tmax = +1.0e308;

	for (int axis = 0; axis < 3; axis++) {
		// If the ray is parallel to the plane, then skip
		if (ray.dir()[axis] == 0) continue;

		// Perform two slab intersections
		double v1 = _min[axis] - ray.at(0)[axis];
		double v2 = _max[axis] - ray.at(0)[axis];
		double t1 = v1 / ray.dir()[axis];
		double t2 = v2 / ray.dir()[axis];

		tmin = (std::min(t1, t2) > tmin) ? std::min(t1, t2) : tmin;
		tmax = (std::max(t1, t2) < tmax) ? std::max(t1, t2) : tmax;

		// If the box is missed or is behind the ray, return false
		if (tmin > tmax || tmax < EPSILON)
			return -1.0f;
	}

	return tmin;
}

} // namespace chess
