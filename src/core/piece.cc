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

void Piece::move(const Position& pos) {
	// Move the piece to the specified position
	_loc = pos;
	
	// Capture any opposing pieces
	Piece* enemy = owner().opponent()->piece(pos);
	if (enemy != nullptr) enemy->_is_alive = false;
	
	// Mark that the piece has moved
	_has_moved = true;
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
	bool has_moved = _has_moved;
	move(pos);
	bool in_check = owner().in_check();
	if (piece != nullptr) piece->_is_alive = true;
	_loc = old;
	_has_moved = has_moved;
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
	// tiles (manhattan distance), but not along a straight line.
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

void King::move(const Position& pos) {
	// Move the rook if castling
	if (pos.dist(this->loc()) == 2 && pos.x == this->loc().x) {
		int dir = (pos.y - this->loc().y) / pos.dist(this->loc());
		Piece* rook = owner().piece(Position(pos.x, pos.y + dir));	
		rook->move(Position(pos.x, pos.y+dir));
	}

	// Otherwise move normally
	Piece::move(pos);
}
  
bool King::isValid(const Position& pos) {
	// The king may always move one square in any direction
	if (Piece::isValid(pos) && pos.dist(this->loc()) == 1) return true;
	
	// The king may move twice to castle
	if (pos.dist(this->loc()) == 2 && pos.x == this->loc().x) {
		bool isKingSide = pos.y - this->loc().y > 0;		
		Piece* rook = owner().piece(Position(this->loc().x, isKingSide*7));
		Position adj = isKingSide ? Position(0, 1) : Position(0, -1);
		
		// The king and rook may not have moved and the king may not
		// move out of, into, or through check.
		if (has_moved() || rook == nullptr || rook->has_moved() ||
			owner().in_check() || !Piece::isValid(adj) || !Piece::isValid(pos)) 
			return false;

		// Check that all adjacent squares between the king and rook are empty
		for (int i = 1; i < rook->loc().dist(this->loc()); i++)
			if (owner().piece(this->loc() + i*adj) != nullptr ||
				owner().opponent()->piece(this->loc() + i*adj) != nullptr)
				return false;
		return true;
	}

	return false;
}

} // namespace chess
