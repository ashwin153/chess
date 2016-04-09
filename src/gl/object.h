#ifndef GL_OBJECT_H
#define GL_OBJECT_H

#include <vector>
#include <string>
#include <glm/glm.hpp>

namespace chess {

/*! Non-deformable object
 * Represents a non-deformable object. Objects are represented using
 * trianglular faces and vertices. Because the models cannot be modified
 * after creation, vertex and face normals can be pre-computed. This should
 * make rendering significantly less expensive.
 */
const struct Object {
	std::vector<glm::vec4> vertices;
	std::vector<glm::uvec3> faces;
	std::vector<glm::vec3> vertex_normals;
	std::vector<glm::vec3> face_normals;

	Object(std::string object_path);
};

} // namespace chess

#endif // GL_OBJECT_H
