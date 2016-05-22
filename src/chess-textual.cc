#include "core/game.h"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
	chess::Game game;
	std::string cmd;
	std::cout << "\nWelcome to TextChess by Ashwin Madavan!\n\n";
	std::cout << game.to_string() << "\n";
	
	while (true) {
		cmd.clear();
		std::cout << "Enter move (ex. e4 e5): ";
		std::getline(std::cin, cmd);

		if (cmd == "q" || cmd == "quit")
		   break;
		else if (cmd.length() < 5)
			std::cout << "Too few characters.\n";
		else if (!game.make(chess::Move(
						chess::Position(cmd.substr(0, 2)), 
						chess::Position(cmd.substr(3, 2)))))
			std::cout << "Invalid move.\n";
		else
			std::cout << "\n" << game.to_string() << "\n";	
	}

	return 0;
}

