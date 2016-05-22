#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <GL/glew.h>

namespace chess {

/*! GLSL Shader
 * This class abstracts away the compilation and management of
 * GLSL Shaders. Shaders cannot be modified after they are created.
 */
struct Shader {
	int id;
	std::string source;	
	GLenum type;

	Shader(const GLenum type, const std::string source_path);
	virtual ~Shader() {}
};

/*! GLSL Shader Program
 * This class abstracts away the compilation, linking, and management
 * of GLSL Shader programs. Shader programs cannot be modified after
 * they have been created.
 */
struct ShaderProgram {
	int id;
	std::vector<Shader> shaders;
	std::unordered_map<std::string, int> uniforms;
	
	ShaderProgram(const std::vector<Shader> shaders,
				  const std::vector<std::string> uniform_names);

	/*! Returns the location of the specified uniform or -1. */
	int uniform(std::string name) const;
};

} // namespace chess

#endif // GL_SHADER_H
