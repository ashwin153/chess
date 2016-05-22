#ifndef GL_RAY_H
#define GL_RAY_H

#define EPSILON 0.000001

#include "object.h"
#include <glm/glm.hpp>

namespace chess {

/*! Intersection
 * This struct is used to provide descriptions about a ray-model intersection.
 * It's purpose is to act as a data transfer object between various methods.
 */
struct Isect {
	double t;
	Object* obj;
};

/*! Ray
 * This struct parameterizes a ray in the following form: r = pos + t * dir. 
 * This allows us to easily specify the position of the ray at any timestep t.
 */
struct Ray {
	glm::vec3 pos;
	glm::vec3 dir;

	Ray(glm::vec3 pos, glm::vec3 dir) : pos(pos), dir(dir) {}
	glm::vec3 at(const double& t) const;
	Ray& operator=(const Ray& rhs);
};

} // namespace chess

#endif // GL_RAY_H