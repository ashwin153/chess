#ifndef GL_BVH_H
#define GL_BVH_H

#include "ray.h"
#include "box.h"
#include "object.h"
#include <vector>

namespace chess {

class Bvh {
private:
	std::vector<Bvh*> children;
	std::vector<Object*> objects;
	Box box;

public:
	/*! Constructs a BVH node
	 * Recursively constructs sub-nodes until depth = 0 or targetLeafSize
	 * is reached.
	 * @param[in] objects Constituent objects
	 * @param[in] depth Current depth
	 * @param[in] targetLeafSize Target size for leaf nodes
	 */
	BvhNode(std::vector<Object*> objects, int depth, int targetLeafSize);

	inline virtual ~BvhNode() {
		for (auto child : children)
			delete child;
		children.clear();
	}

	/* Finds the closest point of intersection
	 * @param[in] ray Traced ray
	 * @param[in,out] isect Intersection details
	 * @return True if intersection, false otherwise
	 */
	bool trace(const Ray& ray, Isect& isect) const; 
};

} // namespace chess

#endif // GL_BVH_H
