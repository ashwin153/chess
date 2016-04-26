#include "box.h"
#include <algorithm>

namespace chess {

double Box::area() const {
	double dx = max[0] - min[0];
	double dy = max[1] - min[1];
	double dz = max[2] - min[2];
	return 2 * (dx * dy + dy *dz + dz * dx);
}

double Box::volume() const {
	double dx = max[0] - min[0];
	double dy = max[1] - min[1];
	double dz = max[2] - min[2];
	return dx * dy * dz;
}

void Box::merge(const Box& box) {
	// If either box is empty, make the box the other box
	if (volume() == 0) { *this = box; return; }
	else if (box.volume() == 0) return;

	// Find the minimum and maximum of both boxes along each axis
	for (int axis = 0; axis < 3; axis++) {
		min[axis] = std::min(min[axis], box.min[axis]);
		max[axis] = std::max(max[axis], box.max[axis]);
	}	
}

bool Box::intersect(const Ray& r) const {
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

		tmin = (std::min(t1, t2) > tmin) ? std::min(t1, t2) : tmin;
		tmax = (std::max(t1, t2) < tmax) ? std::max(t1, t2) : tmax;

		// If the box is missed or is behind the ray, return false
		if (tmin > tmax || tmax < EPSILON)
			return false;
	}

	return true;
}

} // namespace chess
