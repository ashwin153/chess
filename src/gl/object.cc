#include "object.h"
#include <fstream>

namespace chess {

Object::Object(std::string object_path) {
	// Loads the vertices and faces from the .obj file
	std::ifstream in(object_path);
	glm::vec4 vertex = glm::vec4(0, 0, 0, 1);
	glm::uvec3 face = glm::uvec3(0, 0, 0);

	while (in.good()) {
		switch (in.get()) {
			case 'v':
				in >> vertex[0] >> vertex[1] >> vertex[2];
				vertices.push_back(vertex);
				break;
			case 'f':
				in >> face[0] >> face[1] >> face[2];
				faces.push_back(face);
				break;
			default:
				break;
		}
	}

	in.close();

	// Computes the normals of each face and the normal at each vertex
	// by averaging the normals of all incident faces for each vertex.
	std::vector<float> vertex_counts;
	for (int i = 0; i < vertices.size(); i++) {
		vertex_normals.push_back(glm::vec3(0, 0, 0));
		vertex_counts.push_back(0);
	}

	for (int i = 0; i < faces.size(); i++) {
		glm::vec3 p1 = glm::vec3(vertices[faces[i].x]);
		glm::vec3 p2 = glm::vec3(vertices[faces[i].y]);
		glm::vec3 p3 = glm::vec3(vertices[faces[i].z]);
		face_normals.push_back(glm::normalize(glm::cross(
			glm::normalize(p2 - p1), glm::normalize(p3 - p1))));

		vertex_normals[faces[i].x] += face_normals[i];
		vertex_normals[faces[i].y] += face_normals[i];
		vertex_normals[faces[i].z] += face_normals[i];
		vertex_counts[faces[i].x]++;
		vertex_counts[faces[i].y]++;
		vertex_counts[faces[i].z]++;
	}

	for (int i = 0; i < vertices.size(); i++)
		vertex_normals[i] /= vertex_counts[i];

	// Setup the vertex array and vertex buffer objects
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(3, &vbo[0]);

	// Bind vertices
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(float) * vertices.size() * 4, 
		&vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Bind faces
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		sizeof(uint32_t) * faces.size() * 3,
		&faces[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_UNSIGNED_INT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Bind vertex normals
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(float) * vertex_normals.size() * 3,
		&vertex_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);	

	// Bind face normals
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(float) * face_normals.size() * 3,
		&face_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Object::~Object() {
	glDeleteBuffers(4, &vbo[0]);
	glDeleteVertexArrays(1, &vao);
}

void Object::render() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

} // namespace chess
