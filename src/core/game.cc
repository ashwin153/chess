#include "game.h"
#include <regex>
#include <iostream>
#include <stdexcept>
#include <algorithm>

namespace chess {

Game::Game() : _turn(0) {
	_white = new Player(true);
	_black = new Player(_white);	
}

Game::Game(const std::string& pgn) {
	std::regex reg_numb("\\s*?\\d+[.]\\s+");
	std::regex reg_move("^([PNBRQK]?)([a-h]?)([1-8]?)[x]?([a-h][1-8]).*$");
	
	std::sregex_token_iterator iter(pgn.begin(), pgn.end(), reg_numb, -1);
	std::sregex_token_iterator end;
	for ( ; iter != end; ++iter) {
		// Deal with each move
		std::cout << *iter << '\n';

		// Special Cases: Pawn Promotion, Castling, and En Passant
		// Throw exception for unhandled cases (Pawn Promotion to non-queen)
		 
	}
}

Game::Game(std::vector<Move> moves) : Game() {
   	_moves = moves;
	step(moves.size());	
}

Game::~Game() {
	delete _white;
	delete _black;
	_white = nullptr;
	_black = nullptr;
}

void Game::step(int times) {
	for (int i = 0; i < times && _turn < _moves.size(); i++) {	
		next()->make(_moves[_turn]);
		_turn++;
	}
}

void Game::back(int times) {
	for (int i = 0; i < times && _turn >= 0; i++) {
		next()->undo();
		_turn--;
	}
}

void Game::make(const Move& move) {
	_moves.push_back(move);
	_moves.erase(_moves.begin() + _turn, _moves.end());
	_moves.push_back(move);	
	step(1);
}

std::string Game::to_string() const {
	// Build up a textual version of the game
	std::string text;
	for (int x = 0; x < 8; x++) {
		text += std::to_string(8 - x);
		for (int y = 0; y < 8; y++) {
			Piece* wpiece = _white->piece(Position(x, y));
			Piece* bpiece = _black->piece(Position(x, y));
			if (!wpiece && !bpiece) text += " ―";
			else if (wpiece) text += " " + wpiece->to_string();
			else if (bpiece) text += " " + bpiece->to_string();
		}	
		text += "\n";
	}
	
	text += " ";
	for (char i = 'a'; i <= 'h'; i++) {
		text += ' ';
		text += i;
	}

	// Return the textual representation
	return text + "\n";
}

}
