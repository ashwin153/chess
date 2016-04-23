#include "game.h"
#include <algorithm>

namespace chess {

Game::Game() : _turn(0) {
	_white = new Player(true);
	_black = new Player(_white);	
}

Game::Game(std::vector<Move> history) : Game() {
   	_history = history;
	step(history.size());	
}

Game::~Game() {
	delete _white;
	delete _black;
	_white = nullptr;
	_black = nullptr;
}

Player* Game::next() const {
	return (_turn % 2 == 0) ? _white : _black;
}

void Game::step(int times) {
	for (int i = 0; i < times; i++) {
		Move move = _history[_turn++];
		next()->piece(move.cur)->move(move.nxt);
	}
}

void Game::back(int times) {
	for (int i = 0; i < times; i++) {
		Move move = _history[--_turn];
		next()->piece(move.nxt)->undo(move.cur);
	}
}

Game Game::make(const Move& move) const {
	std::vector<Move> copy;
	for (int i = 0; i < _turn; i++)
		copy.push_back(Move(_history[i]));
	copy.push_back(move);
	return Game(copy);
}

}
