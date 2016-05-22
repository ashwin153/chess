#include "player.h"
#include "piece.h"
#include <vector>

namespace chess {

Player::Player(bool is_white) {
	_is_white = is_white;
	_opponent = nullptr;

	// Setup default positions depending on choice of white or black
	_pieces.push_back(new Rook  (*this, Position(is_white*7, 0)));
	_pieces.push_back(new Knight(*this, Position(is_white*7, 1)));
	_pieces.push_back(new Bishop(*this, Position(is_white*7, 2)));
	_pieces.push_back(new Queen (*this, Position(is_white*7, 3)));
	_pieces.push_back(new Bishop(*this, Position(is_white*7, 5)));
	_pieces.push_back(new Knight(*this, Position(is_white*7, 6)));
	_pieces.push_back(new Rook  (*this, Position(is_white*7, 7)));

	_king = new King(*this, Position(is_white*7, 4));
	for (int i = 0; i < 8; i++)
		_pieces.push_back(new Pawn(*this, Position(is_white*5+1, i)));
}

Player::Player(Player* opponent) : Player(!opponent->is_white()) {
	// Setup opponent relationships
	_opponent = opponent;
	opponent->_opponent = this;
}

Player::~Player() {
	// Delete all pieces
	for (auto piece : _pieces) 
		delete piece;
	_pieces.clear();

	// Delete the king
	delete _king;
	_king = nullptr;
}

bool Player::is_white() {
	return _is_white;
}

Player* Player::opponent() {
	return _opponent;
}

bool Player::in_check() {
	// Check if any of the opponent's live pieces or king  can 
	// move to the king's position.
	for (auto piece : _opponent->_pieces)
		if (piece->is_alive() && piece->isValid(_king->loc()))
			return true;
	return _opponent->_king->isValid(_king->loc());
}

Piece* Player::piece(const Position& pos) {
	// Search the live pieces for the position
	for (auto piece : _pieces)
		if (piece->is_alive() && piece->loc() == pos)
			return piece;

	// Check the position of the king
	return (_king->loc() == pos) ? _king : nullptr;
}

} // namespace chess
