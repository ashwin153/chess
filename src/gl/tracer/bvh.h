#ifndef GL_BVH_H
#define GL_BVH_H

#include "ray.h"
#include "box.h"
#include "triangle.h"

#include <vector>
#include <glm/glm.hpp>

namespace chess {

class Bvh {
private:
	std::vector<Triangle*> _triangles;
	std::vector<Bvh*> _children;
	Box _box;

public:
	/*! Constructs a BVH
	 * Recursively constructs sub-nodes until depth = 0 or targetLeafSize
	 * is reached.
	 * @param[in] objects Constituent objects
	 * @param[in] depth Current depth
	 * @param[in] targetLeafSize Target size for leaf nodes
	 */
	Bvh(std::vector<Triangle*> triangles, int depth, int targetLeafSize);

	virtual ~Bvh();

	/* Finds the closest point of intersection
	 * @param[in] ray Traced ray
	 * @param[in,out] isect Intersection details
	 * @return True if intersection, false otherwise
	 */
	double intersect(const Ray& ray) const; 
};

} // namespace chess

#endif // GL_BVH_H
