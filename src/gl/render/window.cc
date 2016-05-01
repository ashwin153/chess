#include "window.h"

namespace chess {

Window::Window(int w, int h, std::string t) : _width(w), _height(h) {
	// Initialize GLFW
	if (!glfwInit()) exit(EXIT_FAILURE);
	glfwSetErrorCallback(Window::glfw_error);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);	
	
	// Create window 
	_glfw = glfwCreateWindow(w, h, &t[0], nullptr, nullptr);
	glfwMakeContextCurrent(_glfw);
	glfwSwapInterval(1); 

	// Initialize GLEW (cross-platform extension loader)
	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

	// Initialize callbacks
	glfwSetWindowUserPointer(_glfw, this);
	glfwSetKeyCallback(_glfw, glfw_key);
	glfwSetCursorPosCallback(_glfw, glfw_cursor);
	glfwSetMouseButtonCallback(_glfw, glfw_click);
	glfwGetFramebufferSize(_glfw, &_buffer_width, &_buffer_height);
}

Window::~Window() {
	glfwDestroyWindow(_glfw);
	glfwTerminate();
}

void Window::refresh() {
	// Clear the viewport
	glfwGetFramebufferSize(_glfw, &_buffer_width, &_buffer_height);
	glViewport(0, 0, _buffer_width, _buffer_height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Render window contents
	render();

	// Update window
	glfwSwapBuffers(_glfw);
	glfwPollEvents();
}

} // namespace chess
