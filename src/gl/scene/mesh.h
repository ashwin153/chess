#ifndef GL_MESH_H
#define GL_MESH_H

#include "tracer/ray.h"
#include "tracer/bvh.h"

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

namespace chess {

/*! Triangle Mesh
 * Represents a non-deformable triangle mesh. Because meshes may not be
 * modified after creation, vertex and face normals can be pre-computed to
 * make rendering significantly less expensive. Meshes are rendering primitives.
 * We make the assumption that all models have a single mesh. This simplifies
 * the abstractions greatly.
 */
class Mesh {
private:
	std::vector<glm::vec4> _vertices; 				// Vertices
	std::vector<glm::vec3> _vertex_normals; 		// Vertex Normals
	std::vector<glm::uvec3> _faces; 				// Face Indices
	std::vector<glm::vec3> _face_normals;			// Face Normals

	GLuint _vao[1]; 								// Vertex Array Object
	GLuint _vbo[3]; 								// Vertex Buffer Objects
	glm::mat4 _mesh2world; 							// Model Matrix

	Bvh* _bvh; 										// Bounded Volume Hierarchy
	Box _bounds; 									// Bounding Box
	glm::vec3 _center; 								// Center of Mass
	
	std::vector<Triangle*> _triangles;

public:	
	/*! Construst a mesh from an .obj file
	 * Creates a mesh, calculates normals, and setups the vertex array
	 * and vertex buffer objects to prepare for rendering.
	 */
	Mesh(const std::string& path);

	/*! Deletes VAO and all VBO's */
	virtual ~Mesh();

	/*! Vertex Array Object
	 * Returns the vertex array object associated with this mesh. This is used
	 * to bind the mesh to the shader/GPU at render time.
	 * @return Vertex array object
	 */
	inline GLuint vao() const {
		return _vao[0];
	}

	/*! Number of Vertices
	 * Returns the number of vertices in this mesh.
	 * @return Number of vertices in the mesh
	 */
	inline int vertices() const {
		return _vertices.size();
	}

	/*! Number of Faces
	 * Returns the number of triangles in this mesh.
	 * @return Number of triangles in the mesh
	 */
	inline int faces() const {
		return _faces.size();
	}

	/*! Bounding Box
	 * Returns a bounding box around all faces in this mesh.
	 * @return Bounding box around all faces
	 */
	inline Box bounds() const {
		return _bounds;
	}

	/*! Center of Mess
	 * Returns the center of mass for this mesh.
	 * @return Center of mass
	 */
	inline glm::vec3 center() const {
		return _center;
	}

	/*! Model Matrix
	 * Returns the model matrix required to transform coordinates in model space
	 * to coordinates in world space.
	 * @return Model matrix
	 */
	inline virtual glm::mat4 model_matrix() const {
		return glm::scale(_mesh2world, glm::vec3(0.05f, 0.05f, 0.05f));
	}

	/*! Apply Transformation
	 * Applies the specified transformation to the model matrix of this mesh.
	 * Used to rotate, scale, and translate objects.
	 * @param[in] tform Transformation
	 */
	inline void apply(glm::mat4 tform) {
		_mesh2world = tform * _mesh2world;
	}

	/*! Collision Detection
	 * Find the minimum point of collision between the specified ray and
	 * this mesh. Wraps a call to the internal BVH.
	 * @param[in] ray Intersection ray
	 * @return Minimum point of intersection or -1
	 */
	double intersect(const Ray& ray) const;
};

} // namespace chess

#endif // GL_MESH_H
