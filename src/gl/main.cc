#include <iostream>
#include <string>
#include <vector>

// GLSL features implemented by GLM
#include <glm/glm.hpp>

// Matrix transformation functions
#include <glm/gtc/matrix_transform.hpp>

// OpenGL library functions
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Define window parameters
const struct Window {
	int width;
	int height;
	std::string title;
} window = { 800, 600, "Chess" };

void ErrorCallback(int error, const char* description) {
	std::cerr << "GLFW error: " << description << "\n";
}

void KeyCallback(GLFWwindow* glfw_window, int key, int sc, int action, int mods) {

}

void CursorPosCallback(GLFWwindow* glfw_window, double mx, double my) {

}

void MouseButtonCallback(GLFWwindow* glfw_window, int button, int action, int mods) {

}

int main(int argc, char** argv) {
	// Initialize and setup GLFW (window manager)
	if (!glfwInit()) exit(EXIT_FAILURE);
	glfwSetErrorCallback(ErrorCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* glfw_window = glfwCreateWindow(
			window.width, window.height, &window.title[0], nullptr, nullptr);
	
	glfwMakeContextCurrent(glfw_window);
	glfwSetKeyCallback(glfw_window, KeyCallback);
	glfwSetCursorPosCallback(glfw_window, CursorPosCallback);
	glfwSetMouseButtonCallback(glfw_window, MouseButtonCallback);
	glfwSwapInterval(1);
		
	// Initialize GLEW (cross-platform extension loader)
	glewExperimental = GL_TRUE;
	glGetError();

	// Initialize GL constants
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cout << "Renderer: " << renderer << "\n";
	std::cout << "OpenGL version: " << version << "\n";
	std::cout << "GLSL version: " << glsl_version << "\n";

	// Load geometries + setup VBO and VAO for each geometry
	// Setup vertex shader
	// Setup geometry shader
	// Setup fragment shader
	// Create program
	// Bind attributes
	// Get uniform locations

	// Rendering loop
	while (!glfwWindowShouldClose(glfw_window)) {
		// Clear window and setup basic components
		glfwGetFramebufferSize(glfw_window, &window.width, &window.height);
		glViewport(0, 0, window.width, window.height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);

		// Compute view and projection matricies
		// Render geometries

		glfwPollEvents();
		glfwSwapBuffers(glfw_window);
	}

	// Close the window and terminate
	glfwDestroyWindow(glfw_window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

