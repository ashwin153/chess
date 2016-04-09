#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <string>

namespace chess {

/*! GLSL Shader
 * This class abstracts away the compilation and management of
 * GLSL Shaders. Shaders cannot be modified after they are created.
 */
const struct Shader {
	int id;
	std::string source;	
	GLenum type;

	Shader(std::string source_path);
	virtual ~Shader();
};

/*! GLSL Shader Program
 * This class abstracts away the compilation, linking, and management
 * of GLSL Shader programs. Shader programs cannot be modified after
 * they have been created.
 */
const struct ShaderProgram {
	int id;
	std::vector<Shader> shaders;
	std::unordered_map<std::string, int> uniforms;
	
	ShaderProgram(std::vector<Shader> shaders,
				  std::vector<std::string> uniform_names);

	/*! Returns the location of the specified uniform. */
	int uniform(std::string name);
}

} // namespace chess

#endif // GL_SHADER_H
