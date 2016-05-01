#include "shader.h"
#include <fstream>
#include <iostream>

namespace chess {

Shader::Shader(GLenum type, std::string path) {
	// Load the shader program from file
	std::ifstream in;
	in.open(path);
	std::string source = std::string(
			std::istreambuf_iterator<char>(in), 
			std::istreambuf_iterator<char>());
	in.close();

	// Create and compile the shader
	_id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(_id, 1, &src, nullptr);
	glCompileShader(_id);

	// Check shader status
	GLint status = 0;
	GLint length = 0;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &status);
	glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &length);
	
	if (!status) {
		std::string log(length, 0);
		glGetShaderInfoLog(_id, length, nullptr, &log[0]);
		std::cerr << path << __LINE__ << " GLSL Shader Log\n" << &log[0] << "\n";
	}
}

ShaderProgram::ShaderProgram(const std::vector<Shader>& shaders, 
		Texture* texture) : _texture(texture) {
	
	// Create shader program and attach shaders
	_id = glCreateProgram();
	for (Shader shader : shaders)
		glAttachShader(_id, shader.id());
	
	// Bind vertex attributes and fragment data
	glBindAttribLocation(_id, 0, "vertex_position");
	glBindAttribLocation(_id, 1, "vertex_normals");
	glBindFragDataLocation(_id, 0, "fragment_color");

	// Link program
	glLinkProgram(_id);
	
	// Bind uniforms
	_uniforms.insert({
		{ Uniform::kLightCoord,       glGetUniformLocation(_id, "light_coord") },
		{ Uniform::kLightColor,       glGetUniformLocation(_id, "light_color") },
		{ Uniform::kLightAtten,       glGetUniformLocation(_id, "light_atten") },
		{ Uniform::kProjectionMatrix, glGetUniformLocation(_id, "projection")  },
		{ Uniform::kModelMatrix,  	  glGetUniformLocation(_id, "model") 	   },
		{ Uniform::kViewMatrix,   	  glGetUniformLocation(_id, "view") 	   },
		{ Uniform::kNormalMatrix, 	  glGetUniformLocation(_id, "normal") 	   },
		{ Uniform::kCameraEye,    	  glGetUniformLocation(_id, "eye") 	       },
		{ Uniform::kCenterMass,    	  glGetUniformLocation(_id, "center") 	   },
		{ Uniform::kMinBounds,    	  glGetUniformLocation(_id, "min") 	       },
		{ Uniform::kMaxBounds,    	  glGetUniformLocation(_id, "max") 	       },
		{ Uniform::kSampler,    	  glGetUniformLocation(_id, "sampler") 	   }});

	// Check program status
	GLint status = 0;
	GLint length = 0;
	glGetProgramiv(_id, GL_LINK_STATUS, &status);
	glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &length);
	
	if (!status) {
		std::string log(length, 0);
		glGetProgramInfoLog(_id, length, nullptr, &log[0]);
		std::cerr << __LINE__ << " GLSL Program Log\n" << &log[0] << "\n";
	}
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(_id);
	if (_texture != nullptr)
		delete _texture;
}

void ShaderProgram::render(
		const Mesh& mesh, 
		const Frustrum& frustrum,
		const Camera& camera, 
		const Light& light) const {

	glUseProgram(_id);

	// Attach texture parameters
	glUniform3fv(find(Uniform::kCenterMass), 1, &mesh.center()[0]);
	glUniform3fv(find(Uniform::kMinBounds), 1, &mesh.bounds().min()[0]);
	glUniform3fv(find(Uniform::kMaxBounds), 1, &mesh.bounds().max()[0]);
	glUniform1i(find(Uniform::kSampler), 0);

	if (_texture != nullptr) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture->texture());
		glBindSampler(0, _texture->sampler());
	}

	// Attach lights and cameras
	glUniform4fv(find(Uniform::kLightCoord), 8, &light.coord()[0][0]);
	glUniform3fv(find(Uniform::kLightColor), 8, &light.color()[0][0]);
	glUniform3fv(find(Uniform::kLightAtten), 8, &light.atten()[0][0]);
	glUniform3fv(find(Uniform::kCameraEye),  1, &camera.eye()[0]);

	// Attach projection, model, view, and normal matrices	
	glUniformMatrix4fv(find(Uniform::kProjectionMatrix), 1, 
			GL_FALSE, &frustrum.projection_matrix()[0][0]);
	glUniformMatrix4fv(find(Uniform::kViewMatrix), 1,
			GL_FALSE, &camera.view_matrix()[0][0]);
	glUniformMatrix4fv(find(Uniform::kModelMatrix), 1,
			GL_FALSE, &mesh.model_matrix()[0][0]);
	glUniformMatrix4fv(find(Uniform::kNormalMatrix), 1, 
			GL_FALSE, &glm::transpose(glm::inverse(
			camera.view_matrix() * mesh.model_matrix()))[0][0]);

	// Draw triangles
	glBindVertexArray(mesh.vao());
	glDrawElements(GL_TRIANGLES, mesh.faces() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

} // namespace chess
