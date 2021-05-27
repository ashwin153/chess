#include "ui.h"
#include "tracer/ray.h"

#include <iostream>
#include <stdlib.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace chess {

ChessWindow::ChessWindow(int w, int h) : Window(w, h, "3D Chess") {
	// Setup shader programs
	TextureGenerator gen = TextureGenerator(512, 512);
	_white = new ShaderProgram(std::vector<Shader> {
		Shader(GL_VERTEX_SHADER,   "./glsl/basic.vertex"),
		Shader(GL_FRAGMENT_SHADER, "./glsl/white.fragment")});
	_black = new ShaderProgram(std::vector<Shader> {
		Shader(GL_VERTEX_SHADER,   "./glsl/basic.vertex"),
		Shader(GL_FRAGMENT_SHADER, "./glsl/black.fragment")});
	_marble = new ShaderProgram(std::vector<Shader> {
		Shader(GL_VERTEX_SHADER, 	"./glsl/basic.vertex"),
		Shader(GL_FRAGMENT_SHADER, 	"./glsl/texture.fragment")},	
		gen.marble(5.0f, 5.0f, 16.0f));

	// Setup frustrum, camera, and lights
	_frustrum = new Frustrum(this, 45.0f, 0.0001f, 1000.0f);
	_camera   = new Camera(glm::vec3(0, 8, 12), 0.5f, 1.0f, 0.025f);
	_light    = new Light();

	// Setup default camera and light positions
	_light->coord(0, glm::vec4(0, 50, 2, 1));
	_camera->rotate(25, glm::vec3(1, 0, 0));

	// Setup game and load meshes from file
	_game = new Game();
	for (int i = 0; i <= 7; i++) {
		for (int j = 0; j <= 7; j++) {
			auto tile = new Object("./meshes/tile.obj", Position(i, j),
					(i + j) % 2 == 0 ? Type::kWhiteTile : Type::kBlackTile);
			
			// The tile meshes are two close to the piece meshes, which often
			// leads to z-fighting. In order to fix this, I simply shifted the
			// tiles down by a few points in y to avoid overlapping.
			tile->apply(glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.1, 0)));
			_objects.push_back(tile);
		}
	}
	
	for (int i = 0; i <= 1; i++) {
		auto owner = (i == 0) ? _game->black() : _game->white(); 	
		_objects.push_back(new PieceObject("./meshes/rook.obj", 
					owner->at(Position(i*7, 0))));
		_objects.push_back(new PieceObject("./meshes/knight.obj", 
					owner->at(Position(i*7, 1))));
		_objects.push_back(new PieceObject("./meshes/bishop.obj", 
					owner->at(Position(i*7, 2))));
		_objects.push_back(new PieceObject("./meshes/king.obj",
					owner->at(Position(i*7, 3))));
		_objects.push_back(new PieceObject("./meshes/queen.obj",
					owner->at(Position(i*7, 4))));
		_objects.push_back(new PieceObject("./meshes/bishop.obj", 
					owner->at(Position(i*7, 5))));
		_objects.push_back(new PieceObject("./meshes/knight.obj", 
					owner->at(Position(i*7, 6))));
		_objects.push_back(new PieceObject("./meshes/rook.obj",
					owner->at(Position(i*7, 7))));

		for (int j = 0; j <= 7; j++)
			_objects.push_back(new PieceObject("./meshes/pawn.obj",
					   owner->at(Position(i*5+1, j))));	
	}
}

ChessWindow::~ChessWindow() {
	// Delete Scene Elements
	delete _frustrum;
	delete _camera;
	delete _light;
	
	// Delete Game Elements
	for (auto object : _objects)
		delete object;
	_objects.clear();
	
	if (_clicked != nullptr)
		delete _clicked;
	
	delete _game;

	// Delete Shader Elements
	delete _black;
	delete _white;
	delete _marble;
}

void ChessWindow::render() {
	ShaderProgram* program;
	for (auto object : _objects) {
		// Decide on a shader program to use depending on the type of the
		// object in question. We assume that types do not change for any
		// given object (ex. a white piece will remain a white piece).
		switch(object->type()) {
			case Type::kWhitePiece: program = _marble; break;
			case Type::kBlackPiece: program = _black; break;
			case Type::kWhiteTile:  program = _white; break;
			case Type::kBlackTile:  program = _black; break;
		}

		// Render the object using the mandatated shader program
		// enforced by the switch statement above.
		program->render(*object, *_frustrum, *_camera, *_light);
	}
}

void ChessWindow::on_cursor(double mx, double my) {
	// Convert the mouse coordinates in screen coordinates to normalized window
	// coordinates on the interval [0, 1]. This will always us to feed the result
	// to the ray tracing algorithm to detect collisions on mouse clicks.
	_mousex = mx / width();
	_mousey = 1 - my / height();
}

void ChessWindow::on_click(int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		// Calculate the click ray from camera location and find the first
		// intersection of this ray and the board or any tiles.
		Ray ray = _camera->ray(_mousex, _mousey);
		Object* selected = nullptr;
		double tmin = -1.0f;

		// Detect collisions with objects on the board. Because of the use of
		// an optimized bounded volume hierarchy, these collision checks should
		// be relatively inexpensive.
		for (auto object : _objects) {
			double t = object->intersect(ray);
			if (t >= 0)
			std::cout << object->loc().x << "," << object->loc().y << "\n";
			if (t > 0 && (selected == nullptr || t < tmin)) {
				tmin = t;
				selected = object;
			}
		}


		// Depending on if valid chess mesh was selected and if a valid
		// chess mesh was selected previously, we either clear the previous
		// clicked mesh, set the current clicked mesh, or make a move.
		if (selected == nullptr) {
			_clicked = nullptr;
		} else if (_clicked == nullptr) { 
//			if (selected->type() == Type::kWhitePiece || 
//				selected->type() == Type::kBlackPiece)
				_clicked = selected;
		} //else if (_game->make(Move(_clicked->loc(), selected->loc()))) {
		//	_clicked = nullptr;

		//	if (selected->type() == Type::kWhitePiece ||
		//		selected->type() == Type::kBlackPiece)
		//		_objects.erase(std::remove(_objects.begin(), 
		//				_objects.end(), selected), _objects.end());
		//}
	}
}

void ChessWindow::on_key(int key, int sc, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		close();
	else if (key == GLFW_KEY_W && action != GLFW_RELEASE)
		_camera->zoom(1);
	else if (key == GLFW_KEY_S && action != GLFW_RELEASE)
		_camera->zoom(-1);
	else if (key == GLFW_KEY_D && action != GLFW_RELEASE)
		_camera->pan(1);
	else if (key == GLFW_KEY_A && action != GLFW_RELEASE)
		_camera->pan(-1);
	else if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE)
		_camera->roll(1);
	else if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE)
		_camera->roll(-1);
}
	
} // namespace chess
