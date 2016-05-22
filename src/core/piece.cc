#include "piece.h"

namespace chess {

Player& Piece::owner() {
	return _owner;
}

Position Piece::loc() {
	return _loc;
}

bool Piece::has_moved() {
	return _has_moved;
}

bool Piece::is_alive() {
	return _is_alive;
}

bool Piece::move(const Position& pos) {
	// If the position is invalid, do not move
	if (!isValid(pos))
		return false;

	// Move the piece to the specified position
	_loc = pos;
	
	// Capture any opposing pieces
	Piece* piece = owner().opponent()->piece(pos);
	if (piece != nullptr) piece->_is_alive = false;
	
	return true;
}

bool Piece::isValid(const Position& pos) {
	// Verify that the position is within bounds and that
	// the owner of this piece does not contain a piece of the
	// same color at the specified position.
	if (!(owner().piece(pos) == nullptr &&
		pos.x >= 0 && pos.x < 8 &&
		pos.y >= 0 && pos.y < 8))
		return false;

	// Verify that movement to the specified position does not
	// place the king in check. Save the current positions of
	// impacted pieces, move the piece, determine if in check,
	// and then reset the original pieces.
	Piece* piece = owner().opponent()->piece(pos);
	if (piece != nullptr) piece->_is_alive = false;
	Position old = _loc;
	_loc = pos;
	
	bool in_check = owner().in_check();
	if (piece != nullptr) piece->_is_alive = true;
	_loc = old;
	return !in_check;
}

bool Pawn::isValid(const Position& pos) {
	if (!Piece::isValid(pos)) return false;

	// White pawns move up the board, and black pawns move down.	
	int dir = owner().is_white() ? -1 : 1;

	// Pawns may always move forward unless there is an opposing
	// piece already occupying that position.
	if (pos - this->loc() == Position(dir, 0) &&
		owner().opponent()->piece(pos) == nullptr) 
		return true;

	// Pawns may move forward twice unless moving twice would
	// prevent a capture (en passant). Otherwise, the pawn may
	// only move once.
	if (pos - this->loc() == Position(2*dir, 0) && !has_moved() &&
		owner().opponent()->piece(pos) == nullptr &&
		owner().opponent()->piece(pos - Position(dir, 0)) == nullptr &&
		owner().opponent()->piece(this->loc() + Position(2*dir, +1)) == nullptr &&
		owner().opponent()->piece(this->loc() + Position(2*dir, -1)) == nullptr)
		return true;

	// Pawns may move diagonally to capture
	if (owner().opponent()->piece(pos) != nullptr &&
		abs(pos.y - this->loc().y) == 1 && 
		pos.x - this->loc().x == dir)
		return true;	

	return false;
}

bool Knight::isValid(const Position& pos) {
	// Knights may move to a position as long as it is within 3
	// tiles (manhattan distance), but not within a straight line.
	return Piece::isValid(pos) && this->loc().dist(pos) <= 3 &&
		this->loc().x != pos.x && this->loc().y != pos.y;
}

bool Bishop::isValid(const Position& pos) {
	// The bishop may only move diagonally
	int dx = pos.x - this->loc().x;
	int dy = pos.y - this->loc().y;
	if (!Piece::isValid(pos) || std::abs(dx) != std::abs(dy)) return false;

	// The bishop may not pass through pieces of either color.
	int sx = ((dx>0)-(dx<0)), sy = ((dy>0)-(dy<0));
	for(int i = 1; i < std::abs(dx); i++) {
		Position nxt = Position(pos.x + i*sx, pos.y + i*sy);
		if (owner().piece(nxt) != nullptr ||
			owner().opponent()->piece(nxt) != nullptr)
			return false;
	}
	
	return true;
}

bool Rook::isValid(const Position& pos) {
	// The rook may only move horizontally xor vertically
	int dx = pos.x - this->loc().x;
	int dy = pos.y - this->loc().y;
	if (!Piece::isValid(pos) || (dx != 0 && dy != 0)) return false;

	// The rook may not pass through pieces of either color. The values
	// sx and sy store the signs of the differences. We make use of
	// clever arithmetic and the fact that one of dx and dy is zero
	// in order to test all possible paths to pos with 1 conditional.
	int sx = ((dx>0)-(dx<0)), sy = ((dy>0)-(dy<0));
	for(int i = 1; i < std::abs(dx + dy); i++) {
		Position nxt = Position(pos.x + i*sx, pos.y + i*sy);
		if (owner().piece(nxt) != nullptr || 
			owner().opponent()->piece(nxt) != nullptr)
			return false;
	}
	
	return true;
}

bool Queen::isValid(const Position& pos) {
	return Rook::isValid(pos) || Bishop::isValid(pos);
}

bool King::move(const Position& pos) {
	return Piece::move(pos);
}
  
bool King::isValid(const Position& pos) {
	// The king may castle if it and the rock have no moved,
	// and it may not castle out of, into, or through check.
	return Piece::isValid(pos) && pos.dist(this->loc()) == 1;
}

}
