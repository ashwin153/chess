#ifndef GL_UI_H
#define GL_UI_H

#include "game.h"
#include "position.h"
#include "object.h"

#include "render/window.h"
#include "render/shader.h"

#include "scene/mesh.h"
#include "scene/camera.h"
#include "scene/light.h"
#include "scene/frustrum.h"

#include <string>

namespace chess {

/*! Chess Window
 * This class contains the logic for rendering and interacting with the chess
 * board. It subclasses the familiar GLFW Window object, which allows it to
 * forget about the nasty implementation details that are inevitable when working
 * with an API as shitty as GLFW.
 */
class ChessWindow : public Window {
private:
	Frustrum* _frustrum; 					// Frustrum (Projection)
	Camera* _camera; 						// Camera (View)
	Light* _light; 							// Lighting
	std::vector<Object*> _objects; 			// Renderable Objects
	ShaderProgram* _white; 					// Shade White Objects
	ShaderProgram* _black; 					// Shade Black Objects
	ShaderProgram* _marble; 				// Shade Marble Objects
	Game* _game; 							// Core Game Logic
	
	double _mousex; 						// Mouse X Coordinate
	double _mousey;  						// Mouse Y Coordinate
	Object* _clicked; 						// Last Clicked Object
	
protected:
	void on_cursor(double mx, double my);
	void on_click(int button, int action, int mods);
	void on_key(int key, int sc, int action, int mods);
	void render();

public:
	/*! Constructs a new GUI with the specified width and height */
	ChessWindow(int width, int height);
	
	/*! Destroys the current interface and all objects */
	virtual ~ChessWindow();
};

} // namespace chess

#endif // GL_UI_H
