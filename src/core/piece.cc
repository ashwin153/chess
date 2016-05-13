#include "piece.h"
#include "game.h"
#include <vector>
#include <iostream>

namespace chess {

bool Piece::valid(const Position& pos) const {
	// Pieces may not move out of bounds, onto allied pieces
	return (!owner().piece(pos) && pos.x >= 0 && 
			pos.x < 8 && pos.y >= 0 && pos.y < 8);
}

std::set<Move> Pawn::moves() {
	std::set<Move> moves;
	
	// Forward Movement
	Position adj = owner().is_white() ? Position(-1, 0) : Position(1, 0);
	if (valid(loc() + adj) && !owner().opponent()->piece(loc() + adj))
		moves.insert(Move(MoveType::kDefault, loc(), loc() + adj));

	// Diagonal Capture
	for (int i = -1; i <= 1; i += 2) {
		Position diag = adj + Position(0, i);
		if (owner().opponent()->piece(loc() + diag))
			moves.insert(Move(MoveType::kDefault, loc(), loc() + diag));
	}

	// Pawn Promotion
	for (std::set<Move>::iterator it = moves.begin(); it != moves.end(); it++) {
		if (it->type == MoveType::kDefault && (it->nxt.x == 0 || it->nxt.x == 7)) {
			moves.insert(Move(MoveType::kPromoteQueen,  it->cur, it->nxt));
			moves.insert(Move(MoveType::kPromoteKnight, it->cur, it->nxt));
			moves.insert(Move(MoveType::kPromoteBishop, it->cur, it->nxt));
			moves.insert(Move(MoveType::kPromoteRook,   it->cur, it->nxt));
			moves.erase(it);
		}
	}

	// Double Forward Movement
	if (valid(loc() + 2 * adj) && !has_moved() &&
			!owner().opponent()->piece(loc() + adj) &&
			!owner().opponent()->piece(loc() + 2 * adj))
		moves.insert(Move(MoveType::kDefault, loc(), loc() + 2 * adj));

	// Enpassant
	for (int i = -1; i <= 1; i += 2) {
		Position enp = Position(0, i);
		if (valid(loc() + adj + enp) && 
				owner().opponent()->piece(loc() + enp) &&
				dynamic_cast<Pawn*>(owner().opponent()->piece(loc() + enp)) &&
				owner().opponent()->turns() > 0 &&
				owner().opponent()->last().cur == loc() + 2 * adj + enp)
			moves.insert(Move(MoveType::kEnpassant, loc(), loc() + adj + enp));
	}

	return moves;
}

std::set<Move> Knight::moves() {
	std::set<Move> moves;

	// L Move
	for (int i = 1; i <= 2; i ++) {
		std::vector<Position> adj = {
			Position(+i, 3-i), Position(+i, i-3),
			Position(-i, 3-i), Position(-i, i-3)};

		for (int j = 0; j < adj.size(); j++)
			if (valid(loc() + adj[j]))
				moves.insert(Move(MoveType::kDefault, loc(), loc() + adj[j]));
	}

	return moves;
}

std::set<Move> Bishop::moves() {
	std::set<Move> moves;

	// Diagonal Movement
	for (int i = -1; i <= 1; i += 2) {
		for (int j = -1; j <= 1; j+= 2) {
			Position diag = Position(i, j);
			while (valid(loc() + diag)) {
				moves.insert(Move(MoveType::kDefault, loc(), loc() + diag));
				diag += Position(i, j);
			}
		}
	}
	
	return moves;
}

std::set<Move> Rook::moves() {
	std::set<Move> moves;

	// Vertical Movement
	for (int i = -1; i <= 1; i += 2) {
		Position vert = Position(i, 0);
		while (valid(loc() + vert)) {
			moves.insert(Move(MoveType::kDefault, loc(), loc() + vert));
			vert += Position(i, 0);
		}
	}

	// Horizontal Movement
	for (int i = -1; i <= 1; i += 2) {
		Position hori = Position(0, i);
		while (valid(loc() + hori)) {
			moves.insert(Move(MoveType::kDefault, loc(), loc() + hori));
			hori += Position(0, i);
		}
	}

	return moves;
}

std::set<Move> Queen::moves() {
	std::set<Move> rmoves = Rook::moves();
	std::set<Move> bmoves = Bishop::moves();
	rmoves.insert(bmoves.begin(), bmoves.end());
	return rmoves;
}

std::set<Move> King::moves() {
	std::set<Move> moves;

	// Kingside Castling
	Piece* krook = owner().piece(Position(loc().x, 7));
	Position right = Position(0, 1);
	if (!has_moved() && krook && !krook->has_moved() &&
			valid(loc() + right) && valid(loc() + 2*right) &&
			!owner().opponent()->piece(loc() + right) &&
			!owner().opponent()->piece(loc() + 2*right) &&
			!owner().in_check() &&
			!owner().in_check(Move(MoveType::kDefault, loc(), loc() + right)))
		moves.insert(Move(MoveType::kCastleKingside, loc(), loc() + 2*right));

	// Queenside Castling
	Piece* qrook = owner().piece(Position(loc().x, 0));
	Position left = Position(0, -1);
	if(!has_moved() && qrook && !qrook->has_moved() && valid(loc() + left) && 
			valid(loc() + 2*left) && valid(loc() + 3*left) &&
			!owner().opponent()->piece(loc() + 1*left) &&
			!owner().opponent()->piece(loc() + 2*left) &&
			!owner().opponent()->piece(loc() + 3*left) &&
			!owner().in_check() &&
			!owner().in_check(Move(MoveType::kDefault, loc(), loc() + left)))
		moves.insert(Move(MoveType::kCastleQueenside, loc(), loc() + 2*left));
	
	// Adjacent Movement
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			Position adj = Position(i, j);
			if (valid(loc() + adj) && (i != 0 || j != 0))
				moves.insert(Move(MoveType::kDefault, loc(), loc() + adj));
		}
	}

	return moves;
}

} // namespace chess
