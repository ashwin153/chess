#include "shader.h"
#include <ifstream>

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
	glShaderSource(id, 1, &source.c_str(), nullptr);
	glCompileShader(id);
}

ShaderProgram::ShaderProgram(std::vector<Shader> shaders, 
		std::vector<std::string> attrib_names,
		std::vector<std::string> data_names,
		std::vector<std::string> uniform_names) : shaders(shaders) {

	id = glCreateProgram();

	// Attach all shaders
	for (Shader shader : shaders)
		glAttachShader(id, shader.id);
	
	// Bind vertex attributes
	for (int i = 0; i < attrib_names.size(); i++)
		inputs.insert({attrib_names[i], 
			glBindAttribLocation(id, i, attrib_names[i])});
	
	// Bind fragment data locations
	for (int i = 0; i < data_names.size(); i++)
		outputs.insert({data_names[i],
			glBindFragDataLocation(id, i, data_names[i])});

	glLinkProgram(id);

	// Get uniform locations
	for (std::string uniform : uniform_names)
		uniforms.insert({uniform, glGetUniformLocation(id, uniform)});
}

ShaderProgram::uniform(std::string uniform_name) {
	return uniforms.find(uniform_name);
}

} // namespace chess
