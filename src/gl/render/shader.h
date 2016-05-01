#ifndef GL_SHADER_H
#define GL_SHADER_H

#include "scene/mesh.h"
#include "scene/camera.h"
#include "scene/light.h"
#include "scene/frustrum.h"

#include "render/texture.h"

#include <map>
#include <string>
#include <vector>
#include <GL/glew.h>

namespace chess {

/*! Shader Uniforms
 * This class represents the uniforms accepted by the GL Shader Program.
 * This removes the dependency between C++ code that uses uniforms and the
 * GLSL code that defines them.
 */
enum class Uniform {
	kLightCoord, 		// Light Coordinates
	kLightColor, 		// Light Color
	kLightAtten, 		// Light Attenuation
	kProjectionMatrix, 	// Projection Matrix
	kModelMatrix, 		// Model Matrix
	kViewMatrix, 		// View Matrix
	kNormalMatrix, 		// Normal Matrix
	kCameraEye, 		// Camera Eye
	kCenterMass, 		// Center of Mass
	kMinBounds, 		// Minimum Bounds
	kMaxBounds, 		// Maximum Bounds
	kSampler, 			// Texture Sampler
};

/*! GLSL Shader
 * This class abstracts away the compilation and management of GLSL Shaders. 
 * Shaders cannot be modified after they are created. Shaders should save
 * information about their state so that they can be queried later.
 */
class Shader {
private:
	GLuint _id;

public:
	/*! Create Shader
	 * Creates a shader of the specified type, from the source file specified
	 * on the path. Compiles the shaders and runs a quick check to verify that
	 * compilation was successful.
	 * @param[in] type Type of shader (GL_VERTEX_SHADER, GL_FRAGMENT_SHADER)
	 * @param[in] source_path Location of file
	 */
	Shader(const GLenum type, const std::string source_path);	

	/*! Remove Shader
	 * Deletes the shader from OpenGL. This method may be called immediately	
	 * after linking the program, because, once linked, this shader object
	 * has no value.
	 */
	inline virtual ~Shader() {
		glDeleteShader(_id);
	}

	/*! Shader ID
	 * Returns the unique ID number of the Shader used by the ShaderProgram
	 * during linking to attach the Shaders.
	 * @return Unique ID number
	 */
	inline GLuint id() const {
		return _id;
	}
};

/*! GLSL Shader Program
 * This class abstracts away the compilation, linking, and management of GLSL 
 * Shader programs. Shader programs cannot be modified after creation.
 */
class ShaderProgram {
private:
	GLuint _id;
	std::map<Uniform, GLuint> _uniforms;
	Texture* _texture;

public:
	/* Default Shader Program
	 * Constructs a shader program from the specified shaders and binds all
	 * uniforms enumerated in the Uniform enum class above. The Shader Program
	 * is the primitive unit of rendering.
	 */
	ShaderProgram(const std::vector<Shader>& shaders, Texture* texture = nullptr);
	
	virtual ~ShaderProgram();

	/*! Program ID
	 * Returns the unique shader program ID number associate with this program.
	 * It is used to attach the program at render time.
	 * @return Unique program ID
	 */
	inline GLuint id() const {
		return _id;
	}

	/*! Find Uniform
	 * Finds and returns the location of the specified uniform.
	 * @param[in] uniform Uniform to lookup
	 * @return Location of uniform
	 */
	inline GLuint find(Uniform uniform) const {
		return _uniforms.find(uniform)->second;
	}

	/*! Render Mesh
	 * Renders the mesh given the specified frustrum, camera, and light (scene)
	 * parameters. Binds the scene parameters to the shader program and then
	 * calls draw on the mesh.
	 */
	void render(
			const Mesh& mesh, 
			const Frustrum& frustrum,
			const Camera& camera,
			const Light& light) const;
};

} // namespace chess

#endif // GL_SHADER_H
