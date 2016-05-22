#include "piece.h"
#include "game.h"
#include <iostream>

namespace chess {

void Piece::move(const Position& pos) {
	// Move the piece to the specified position
	_loc = pos;
	
	// Capture any opposing pieces
	owner().opponent()->capture(_loc);
}

void Piece::undo(const Position& pos) {
	// Uncapture any opposing pieces
	owner().opponent()->uncapture(_loc);
	
	// Move the piece to the specified position
	_loc = pos;
}

bool Piece::valid(const Position& pos) {
	// Verify that the position is within bounds and that
	// the owner of this piece does not contain a piece of the
	// same color at the specified position.
	if (!(!owner().piece(pos) &&
		pos.x >= 0 && pos.x < 8 &&
		pos.y >= 0 && pos.y < 8))
		return false;
	return true;
}

void Pawn::move(const Position& pos) {
	Piece::move(pos);

	// Pawn Promotion
	if (loc().x == 0 || loc().x == 7) {
		owner().capture(loc());
		owner().spawn(new Queen(owner(), loc()));
	}
}

bool Pawn::valid(const Position& pos) {
	if (!Piece::valid(pos)) return false;

	// White pawns move up the board, and black pawns move down.	
	int dir = owner().is_white() ? -1 : 1;

	// Pawns may always move forward unless there is an opposing
	// piece already occupying that position.
	if (pos - loc() == Position(dir, 0) &&
		!owner().opponent()->piece(pos)) 
		return true;

	// Pawns may move forward twice unless moving twice would
	// prevent a capture (en passant). Otherwise, the pawn may
	// only move once.
	if (pos - loc() == Position(2*dir, 0) && !has_moved() &&
		!owner().opponent()->piece(pos) &&
		!owner().opponent()->piece(pos - Position(dir, 0)) &&
		!owner().opponent()->piece(loc() + Position(2*dir, +1)) &&
		!owner().opponent()->piece(loc() + Position(2*dir, -1)))
		return true;

	// Pawns may move diagonally to capture
	if (owner().opponent()->piece(pos) &&
		abs(pos.y - loc().y) == 1 && 
		pos.x - loc().x == dir)
		return true;	

	return false;
}

bool Knight::valid(const Position& pos) {
	// Knights may move to a position as long as it is within 3
	// tiles (manhattan distance), but not along a straight line.
	return Piece::valid(pos) && loc().dist(pos) == 3 &&
		loc().x != pos.x && loc().y != pos.y;
}

bool Bishop::valid(const Position& pos) {
	int dx = pos.x - loc().x;
	int dy = pos.y - loc().y;
	
	// The bishop may only move diagonally
	if (!Piece::valid(pos) || std::abs(dx) != std::abs(dy)) 
		return false;

	// The bishop may not pass through pieces of either color.
	int sx = ((dx>0)-(dx<0)), sy = ((dy>0)-(dy<0));
	for(int i = 1; i < std::abs(dx); i++) {
		Position nxt = loc() + Position(i*sx, i*sy);
		if (owner().piece(nxt) || owner().opponent()->piece(nxt))
			return false;
	}
	
	return true;
}

bool Rook::valid(const Position& pos) {
	int dx = pos.x - loc().x;
	int dy = pos.y - loc().y;
	
	// The rook may only move horizontally xor vertically
	if (!Piece::valid(pos) || (dx != 0 && dy != 0)) 
		return false;

	// The rook may not pass through pieces of either color. The values
	// sx and sy store the signs of the differences. We make use of
	// clever arithmetic and the fact that one of dx and dy is zero
	// in order to test all possible paths to pos with 1 conditional.
	int sx = ((dx>0)-(dx<0)), sy = ((dy>0)-(dy<0));
	for(int i = 1; i < std::abs(dx + dy); i++) {
		Position nxt = loc() + Position(i*sx, i*sy);
		if (owner().piece(nxt) || owner().opponent()->piece(nxt))
			return false;
	}
	
	return true;
}

bool Queen::valid(const Position& pos) {
	return Rook::valid(pos) || Bishop::valid(pos);
}

void King::move(const Position& pos) {
	// Move the rook if castling
	int dy = pos.y - loc().y;
	if (std::abs(dy) == 2) {
		int sy = ((dy>0)-(dy<0));
		Piece* rook = owner().piece(Position(pos.x, (sy > 0) * 7));	
		if (rook) rook->move(pos - Position(0, sy));
	}

	// Otherwise move normally
	Piece::move(pos);
}

void King::undo(const Position& pos) {
	// Move the rook if castling
	int dy = pos.y - loc().y;
	if (std::abs(dy) == 2) {
		int sy = ((dy>0)-(dy<0));
		Piece* rook = owner().piece(pos - Position(0, sy));
		if (rook) rook->undo(Position(pos.x, (sy < 0) * 7));	
	}

	// Otherwise undo normally
	Piece::undo(pos);
}

bool King::valid(const Position& pos) {
	int dx = std::abs(pos.x - loc().x);
	int dy = std::abs(pos.y - loc().y);
	
	// The king may always move one square in any direction
	if (Piece::valid(pos) && dx <= 1 && dy <= 1) 
		return true;
	
	// The king may move twice horizontally to castle
	if (dx == 0 && dy == 2) {
		bool kingside = (pos.y - loc().y) > 0;
		Piece* rook   = owner().piece(Position(pos.x, kingside * 7));
		Position adj  = Position(0, kingside * 2 - 1); 
		
		// Castles are invalid if the king moves out of or through check
		if (has_moved() || !rook || rook->has_moved() || 
			owner().in_check(Move(loc(), loc())) || 
			owner().in_check(Move(loc(), loc() + adj)))
			return false;
	
		// Check that all positions between the king and rook are empty
		for (int i = 1; i <= 3 - kingside; i++)
			if (owner().piece(loc() + i * adj) ||
				owner().opponent()->piece(loc() + i * adj))
				return false;
		return true;
	}

	return false;
}

} // namespace chess
