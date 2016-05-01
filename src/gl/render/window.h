#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace chess {

/*! GLFW Window Manager
 * This class wraps a GLFW window to provide important size information used
 * when refreshing the framebuffer at render time. The class serves literally
 * no purpose except to flex my object-oriented muscles. This goal of this class
 * is to separate window management and window rendering. In a perfect world,
 * these two tasks should be completely independent.
 */
class Window {
private:
	GLFWwindow* _glfw;
	int _width;
	int _height;
	int _buffer_width;
	int _buffer_height;

protected:
	virtual void render() = 0;
	virtual void on_cursor(double mx, double my) = 0;
	virtual void on_click(int button, int action, int mods) = 0;
	virtual void on_key(int key, int sc, int action, int mods) = 0;

public:
	/*! Constructs GLFW context and window */
	Window(int width, int height, std::string title);
	
	/*! Destroys window and GLFW context */
	virtual ~Window();
	
	/*! Displays the window */
	void refresh();
	
	/*! Makes window inactive */
	inline void close() {
		glfwSetWindowShouldClose(_glfw, GL_TRUE);
	}

	/*! Returns whether or not the window is active */
	inline bool active() const {
		return !glfwWindowShouldClose(_glfw);
	}
	/*! Returns the window width */
	inline int width() const {
		return _width;
	}

	/*! Returns the window height */
	inline int height() const {
		return _height;
	}

	/*! Returns the width of the framebuffer */
	inline int buffer_width() const {
		return _buffer_width;
	}

	/*! Returns the height of the framebuffer */
	inline int buffer_height() const {
		return _buffer_height;
	}

	/*! Returns the window aspect ratio */
	inline float aspect_ratio() const {
		return (float) _width / _height;
	}
	
	/*! Returns the OpenGL renderer */
	inline const GLubyte* renderer() const {
		return glGetString(GL_RENDERER);
	}

	/*! Returns the OpenGL version */
	inline const GLubyte* version() const {
		return glGetString(GL_VERSION);
	}

	/*! Returns the GLSL version */
	inline const GLubyte* glsl() const {
		return glGetString(GL_SHADING_LANGUAGE_VERSION);
	}

	/* GLFW is a fucking nightmare. It will not accept class member functions
	 * as valid callback functions. Callback functions must be declared as
	 * static for GLFW. However, the callback functions need to be called on
	 * a class instance to be useful. Therefore, we redirect the static GLFW
	 * callback functions using the glfwWindowUserPointer which we setup in the
	 * Window constructor. It's a hack. But so is all of fucking GLFW.
	 */
	static void glfw_error(int error, const char* desc) {
		std::cerr << "GLFW Error (" << error << "): " << desc << "\n";
	}	

	static void glfw_cursor(GLFWwindow* glfw, double mx, double my) {
		Window* window = (Window*) glfwGetWindowUserPointer(glfw);
		window->on_cursor(mx, my);
	}

	static void glfw_click(GLFWwindow* glfw, int button, int action, int mods) {
		Window* window = (Window*) glfwGetWindowUserPointer(glfw);
		window->on_click(button, action, mods);
	}
	
	static void glfw_key(GLFWwindow* glfw, int key, int sc, int action, int mods) {
		Window* window = (Window*) glfwGetWindowUserPointer(glfw);
		window->on_key(key, sc, action, mods);
	}
};

} // namespace chess

#endif // GL_WINDOW_H
