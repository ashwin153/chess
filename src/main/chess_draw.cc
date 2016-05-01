#include "ui.h"
#include <iostream>
#include <stdlib.h>

int main(int argc, char** argv) {
	chess::ChessWindow ui(800, 600);
	std::cout << "Renderer: " << ui.renderer() << "\n";
	std::cout << "OpenGL Version: " << ui.version() << "\n";
	std::cout << "GLSL Version: " << ui.glsl() << "\n";
	std::cout << "Width: " << ui.width() << "\n";
	std::cout << "Height: " << ui.height() << "\n";

	while (ui.active())
		ui.refresh();

	return EXIT_SUCCESS;
}

