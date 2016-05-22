#include "shader.h"
#include <fstream>

namespace chess {

Shader::Shader(GLenum type, std::string source_path) : type(type) {
	// Load the shader program from file
	std::ifstream in;
	in.open(source_path);
	source = std::string(std::istreambuf_iterator<char>(in),
                		 std::istreambuf_iterator<char>());
	in.close();

	// Create and compile the shader
	id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
}

ShaderProgram::ShaderProgram(std::vector<Shader> shaders, 
		std::vector<std::string> uniform_names) : shaders(shaders) {

	// Create the shader program
	id = glCreateProgram();

	// Attach all shaders
	for (Shader shader : shaders)
		glAttachShader(id, shader.id);
	
	// Bind vertex attributes and fragment data
	glBindAttribLocation(id, 0, "vertex_position");
	glBindAttribLocation(id, 1, "vertex_normals");
	glBindAttribLocation(id, 2, "face_normals");
	glBindFragDataLocation(id, 0, "fragment_color");

	// Link Program
	glLinkProgram(id);

	// Get uniform locations
	for (std::string uniform : uniform_names)
		uniforms.insert({uniform, glGetUniformLocation(id, uniform.c_str())});
}

int ShaderProgram::uniform(std::string uniform_name) const {
	auto iter =  uniforms.find(uniform_name);
	if (iter != uniforms.end())
		return iter->second;
	return -1;
}

} // namespace chess
