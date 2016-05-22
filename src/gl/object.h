#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace chess {

/*! Non-deformable object
 * Represents a non-deformable object. Objects are represented using
 * trianglular faces and vertices. Because the models cannot be modified
 * after creation, vertex and face normals can be pre-computed. This should
 * make rendering significantly less expensive.
 */
struct Object {
	std::vector<glm::vec4> vertices;
	std::vector<glm::uvec3> faces;
	std::vector<glm::vec3> vertex_normals;
	std::vector<glm::vec3> face_normals;
	
	GLuint vao;
	GLuint vbo[4];

	/*! Construst an object from an .obj file
	 * Creates an object, calculates normals, and setups the vertex array
	 * and vertex buffer objects to prepare for rendering.
	 */
	Object(std::string object_path);
	virtual ~Object();

	/*! Renders the object
	 * Binds the vertex array and draws triangles.
	 */
	void render();
};

} // namespace chess

#endif // OBJECT_H
