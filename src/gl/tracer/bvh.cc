#include "bvh.h"

namespace chess {

Bvh::Bvh(std::vector<Triangle*> triangles, 
		int depth, int leaf) : _triangles(triangles) {

	// Calculate a bounding box around all triangles
	for (auto triangle : triangles)
		_box.merge(triangle->bounds());

	// Generate child nodes if not leaf node
	if (depth > 0 && triangles.size() > leaf) {
		// Calculate average point
		glm::vec3 mean(0, 0, 0);
		for (auto triangle : triangles)
			mean += triangle->bounds().midpoint();
		mean /= triangles.size();

		// Place triangles into quadrants depending on relative position
		std::vector<Triangle*> quads[8];
		for (auto triangle : triangles) {
			glm::vec3 mdpt = triangle->bounds().midpoint();
			int i = (mdpt[0]>mean[0])*4 + (mdpt[1]>mean[1])*2 + (mdpt[2]>mean[2]);	
			quads[i].push_back(triangle);
		}

		// Recursively create child nodes
		for (int i = 0; i < 8; i++)
			if (!quads[i].empty())
				_children.push_back(new Bvh(quads[i], depth - 1, leaf));
	}
}

Bvh::~Bvh() {
	if (_children.empty()) {
		for (auto triangle : _triangles)
			delete triangle;
	} else {
		for (auto child : _children)
			delete child;
	}
	
	_triangles.clear();
	_children.clear();
}

double Bvh::intersect(const Ray& ray) const {
	// If the ray does not intersect the bounding box, it is impossible for it
	// to intersect any of the triangles within the box.
	if (_box.intersect(ray) < 0)
		return -1.0;

	// Calculate the minimum point of intersection with leaf nodes or with all
	// the triangles in the node if it is a leaf node.
	double tmin = -1.0; 
	if (_children.empty()) {
		for (auto triangle : _triangles) {
			double t = triangle->intersect(ray);
			tmin = (t > 0 && (tmin < 0 || t < tmin)) ? t : tmin;
		}
	} else {
		for (auto child : _children) {
			double t = child->intersect(ray);
			tmin = (t > 0 && (tmin < 0 || t < tmin)) ? t : tmin;
		}	
	}

	// Return the minimum point of intersection or -1
	return tmin;
}

} // namespace chess
