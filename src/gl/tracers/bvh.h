#ifndef GL_BVH_H
#define GL_BVH_H

#include "ray.h"
#include <vector>

namespace chess {

class BvhNode {
private:
	BoundingBox bbox;
	std::vector<BvhNode*> children;
	std::vector<Model*> models;

public:
	BvhNode(std::vector<Model*> models, int depth, int targetLeafSize);
	bool intersect(const Ray& ray, Isect& isect);
};

class BvhTree {
private:
	BvhNode* root;

public:
	BvhTree(std::vector<Model*> models, int maxDepth=100, int targetLeafSize=1);
	bool intersect(const Ray& ray, Isect& isect);
};

} // namespace chess

#endif // GL_BVH_H
