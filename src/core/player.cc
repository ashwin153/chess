#include "player.h"
#include "piece.h"
#include "game.h"

#include <vector>
#include <algorithm>
#include <iostream>

namespace chess {

Player::Player(bool is_white) {
	_is_white = is_white;
	_enemy = nullptr;

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

Player::Player(Player* enemy) : Player(!enemy->is_white()) {
	// Setup opponent relationships
	_enemy = enemy;
	enemy->_enemy = this;
}

Player::~Player() {
	for (auto piece : _live)
		delete piece;
	for (auto piece : _dead)
		delete piece;

	delete _king;
	_live.clear();
	_dead.clear();
}

void Player::make(const Move& move) {
	at(move.cur)->loc(move.nxt);
	_enemy->capture(move.nxt);

	// Handle compound moves
	if (move.type == MoveType::kCastleKingside)
		at(move.nxt+Position(0, 1))->loc(move.nxt-Position(0, 1));
	else if (move.type == MoveType::kCastleQueenside)
		at(move.nxt-Position(0, 2))->loc(move.nxt+Position(0, 1));
	else if (move.type == MoveType::kEnpassant)
		_enemy->capture(Position(move.cur.x, move.nxt.y));
	else if (move.type == MoveType::kPromoteQueen)
		replace(move.nxt, new Queen(*this, move.nxt));
	else if (move.type == MoveType::kPromoteKnight)
		replace(move.nxt, new Knight(*this, move.nxt));
	else if (move.type == MoveType::kPromoteBishop)
		replace(move.nxt, new Bishop(*this, move.nxt));
	else if (move.type == MoveType::kPromoteRook)
		replace(move.nxt, new Rook(*this, move.nxt));

	_history.push(move);
}

void Player::undo() {
	Move move = _history.top();

	// Handle compound moves
	if (move.type == MoveType::kCastleKingside)	
		at(move.nxt-Position(0,1))->loc(move.nxt+Position(0,1));
	else if (move.type == MoveType::kCastleQueenside)
		at(move.nxt+Position(0,1))->loc(move.nxt-Position(0,2));
	else if (move.type == MoveType::kEnpassant)
		_enemy->uncapture(Position(move.cur.x, move.nxt.y));
	else if (move.type == MoveType::kPromoteKnight || 
			 move.type == MoveType::kPromoteQueen ||
			 move.type == MoveType::kPromoteBishop || 
			 move.type == MoveType::kPromoteRook)	
		replace(move.nxt, new Pawn(*this, move.nxt));

	// Undo specified move
	_enemy->uncapture(move.nxt);
	at(move.nxt)->loc(move.cur);
	_history.pop();
}

bool Player::valid(const Position& pos) const {
	return !at(pos) && pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
}

bool Player::in_check(const Move& move) {
	make(move);
	bool check = in_check();
	undo();
	return check;
}

bool Player::in_check() {
	// Check if opponents pieces can move to king
	for (auto enemy : _enemy->_live)
		for (auto move : enemy->moves())
			if (move.nxt == _king->loc())
				return true;
	return false;
}

void Player::replace(const Position& pos, Piece* replace) {
	delete at(pos);
	_live.erase(std::remove(_live.begin(), _live.end(), at(pos)), _live.end());
	_live.push_back(replace);
}

Piece* Player::at(const Position& pos) const {
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
	for (int i = _dead.size() - 1; i >= 0; i--) {
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
