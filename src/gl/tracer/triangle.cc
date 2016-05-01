#include "triangle.h"

namespace chess {

Triangle::Triangle(const glm::vec3& v1, const glm::vec3& v2, 
		const glm::vec3& v3) : _v1(v1), _v2(v2), _v3(v3) {	
	
	// Calculate a bounding box around the triangle
	glm::vec3 min = glm::min(v1, glm::min(v2, v3));
	glm::vec3 max = glm::max(v1, glm::max(v2, v3));
	_box.merge(Box(min, max));
}

double Triangle::intersect(const Ray& ray) const {
	// Moller-Trumbore Intersection Algorithm
	glm::vec3 E1 = _v2 - _v1;
	glm::vec3 E2 = _v3 - _v1;
	glm::vec3 P  = glm::cross(ray.dir(), E2);
	glm::vec3 T  = ray.at(0) - _v1;
	glm::vec3 Q = glm::cross(T, E1);
	double d = glm::dot(E1, P);

	// If the determinant is near zero, the ray lies in the plane
	if (d > -EPSILON && d < EPSILON)
		return -1.0;

	// Calculate the 'u' coordinate
	double u = glm::dot(T, P) / d;
	if (u < 0.0 || u > 1.0)
		return -1.0;

	// Calculate the 'v' coordinate
	double v = glm::dot(ray.dir(), Q) / d;
	if (v < 0.0 || u + v > 1.0)
		return -1.0;

	// Calculate the 't' parameter
	double t = glm::dot(E2, Q) / d;
   	return (t < EPSILON) ? -1.0 : t;
}

} // namespace chess
