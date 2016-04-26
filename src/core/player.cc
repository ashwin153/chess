#include "player.h"
#include "piece.h"
#include "game.h"
#include <vector>
#include <algorithm>
#include <iostream>

namespace chess {

Player::Player(bool is_white) {
	_is_white = is_white;
	_opponent = nullptr;

	// Setup default positions depending on choice of white or black
	_live.push_back(new Rook  (*this, Position(is_white*7, 0)));
	_live.push_back(new Knight(*this, Position(is_white*7, 1)));
	_live.push_back(new Bishop(*this, Position(is_white*7, 2)));
	_live.push_back(new Queen (*this, Position(is_white*7, 3)));
	_live.push_back(new Bishop(*this, Position(is_white*7, 5)));
	_live.push_back(new Knight(*this, Position(is_white*7, 6)));
	_live.push_back(new Rook  (*this, Position(is_white*7, 7)));

	for (int i = 0; i < 8; i++)
		_live.push_back(new Pawn(*this, Position(is_white*5+1, i)));
	
	_king = new King(*this, Position(is_white*7, 4));
	_live.push_back(_king);
}

Player::Player(Player* opponent) : Player(!opponent->is_white()) {
	// Setup opponent relationships
	_opponent = opponent;
	opponent->_opponent = this;
}

Player::~Player() {
	// Delete all pieces
	for (auto piece : _live) 
		delete piece;
	for (auto piece : _dead)
		delete piece;
	_live.clear();
	_dead.clear();
}

bool Player::in_check(const Move& move) {
	Piece* ally = piece(move.cur);
	ally->move(move.nxt);
	bool in_check = false;

	// Check if any of the opponent's live pieces can move to king 
	for (auto enemy : _opponent->_live)
		if (enemy->valid(_king->loc()))
			in_check = true;

	ally->undo(move.cur);
	return in_check;
}

Piece* Player::piece(const Position& pos) const {
	// Search the live pieces for the position
	for (auto piece : _live)
		if (piece->loc() == pos)
			return piece;
	return nullptr;
}

Piece* Player::capture(const Position& pos) {
	// Search the live pieces for the position
	for (int i = 0; i < _live.size(); i++) {
		if (_live[i]->loc() == pos) {
			_dead.push_back(_live[i]);
			_live.erase(_live.begin() + i);
			return _dead[_dead.size() - 1];
		}
	}
	
	// If no piece is found, return nullptr	
	return nullptr;
}

Piece* Player::uncapture(const Position& pos) {
	// Search the live pieces for the position
	for (int i = 0; i < _dead.size(); i++) {
		if (_dead[i]->loc() == pos) {
			_live.push_back(_dead[i]);
			_dead.erase(_dead.begin() + i);
			return _live[_live.size() - 1];
		}
	}

	// If no piece is found, return nullptr
	return nullptr;
}

} // namespace chess
