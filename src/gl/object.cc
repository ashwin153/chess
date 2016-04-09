#include "object.h"
#include <ifstream>

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
}

} // namespace chess
