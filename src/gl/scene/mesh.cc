#include "mesh.h"
#include <iostream>
#include <fstream>

namespace chess {

Mesh::Mesh(const std::string& path) {	
	// Loads the vertices and faces from the .obj file
	std::ifstream in(path);
	glm::vec4 vertex = glm::vec4(0, 0, 0, 1);
	glm::uvec3 face = glm::uvec3(0, 0, 0);

	while (in.good()) {
		switch (in.get()) {
			case 'v':
				in >> vertex[0] >> vertex[1] >> vertex[2];
				_vertices.push_back(vertex);
				break;
			case 'f':
				in >> face[0] >> face[1] >> face[2];
				_faces.push_back(face -= 1);
				break;
			default:
				break;
		}
	}

	in.close();

	// Calculate the center of mass
	for (int i = 0; i < _vertices.size(); i++)
		_center += glm::vec3(_vertices[i]);
	_center /= _vertices.size();

	// Computes the normals of each face and the normal at each vertex
	// by averaging the normals of all incident _faces for each vertex.
	for (int i = 0; i < _vertices.size(); i++)
		_vertex_normals.push_back(glm::vec3(0, 0, 0));

	for (int i = 0; i < _faces.size(); i++) {
		glm::vec3 p1 = glm::vec3(_vertices[_faces[i].x]);
		glm::vec3 p2 = glm::vec3(_vertices[_faces[i].y]);
		glm::vec3 p3 = glm::vec3(_vertices[_faces[i].z]);
		glm::vec3 v1 = glm::normalize(p2 - p1);
		glm::vec3 v2 = glm::normalize(p3 - p1);
		_face_normals.push_back(glm::normalize(glm::cross(v1, v2)));

		for (int j = 0; j < 3; j++)
			_vertex_normals[_faces[i][j]] += _face_normals[i];
	}

	for (int i = 0; i < _vertices.size(); i++) 
		_vertex_normals[i] = glm::normalize(_vertex_normals[i]);

	// Setup the Bounded Volume Hierarchy (BVH)
	std::vector<Triangle*> triangles;
	for (auto face : _faces) {
		triangles.push_back(new Triangle(
					glm::vec3(_vertices[face[0]]),
					glm::vec3(_vertices[face[1]]),
					glm::vec3(_vertices[face[2]])));
		_bounds.merge(triangles[triangles.size()-1]->bounds());
	}
	
	_bvh = new Bvh(triangles, 25, 5);

	// Setup the vertex array and vertex buffer objects
	glGenVertexArrays(1, _vao);
	glBindVertexArray(_vao[0]);
	glGenBuffers(3, &_vbo[0]);

	// Bind vertices
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(float) * _vertices.size() * 4, 
		&_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	// Bind vertex normals
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(float) * _vertex_normals.size() * 3,
		&_vertex_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// Bind faces
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		sizeof(uint32_t) * _faces.size() * 3,
		&_faces[0], GL_STATIC_DRAW);
	
	// Unbind all buffers and arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	glBindVertexArray(0);
}

Mesh::~Mesh() {
	delete _bvh;
	glDeleteBuffers(3, &_vbo[0]);
	glDeleteVertexArrays(1, _vao);
}

double Mesh::intersect(const Ray& ray) const {
	// Convert to the ray to model coordinates
	glm::mat4 inv = glm::inverse(model_matrix());
	glm::mat3 nor = glm::transpose(glm::inverse(glm::mat3(model_matrix())));
	glm::vec3 pos(inv * glm::vec4(ray.at(0), 1));
	glm::vec3 dir = glm::normalize(nor * ray.dir());
	Ray tform = Ray(pos, dir);

	// Use the BVH to perform collision detection
	double tmin = _bvh->intersect(tform);
	
	// Recalculate distance in world coordinates
	return (tmin < 0) ? tmin : tmin / glm::length(dir); 
}

} // namespace chess
