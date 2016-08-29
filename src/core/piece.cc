#include "piece.h"
#include "game.h"
#include <vector>
#include <iostream>

namespace chess {

std::set<Move> Piece::filter(const std::set<Move>& moves) const {
	std::set<Move> filtered;
	for (Move move : moves)
		if (move.nxt >= Position(0, 0) && move.nxt <= Position(8, 8) &&
				!owner().at(move.nxt) && !owner().in_check(move))
			filtered.insert(move);
	return filtered;
}

std::set<Move> Pawn::moves() {
	std::set<Move> moves;
	
	// Forward Movement
	Position adj = owner().is_white() ? Position(-1, 0) : Position(1, 0);
	if (!owner().enemy()->at(loc() + adj))
		moves.insert(Move(MoveType::kDefault, loc(), loc() + adj));

	// Diagonal Capture
	for (int i = -1; i <= 1; i += 2) {
		Position diag = adj + Position(0, i);
		if (owner().enemy()->at(loc() + diag))
			moves.insert(Move(MoveType::kDefault, loc(), loc() + diag));
	}

	// Double Forward Movement
	if (!has_moved() &&
			!owner().enemy()->at(loc() + adj) &&
			!owner().enemy()->at(loc() + 2 * adj))
		moves.insert(Move(MoveType::kDefault, loc(), loc() + 2 * adj));

	// Enpassant
	for (int i = -1; i <= 1; i += 2) {
		Position enp = Position(0, i);
		if (owner().enemy()->at(loc() + enp) &&
				dynamic_cast<Pawn*>(owner().enemy()->at(loc() + enp)) &&
				owner().enemy()->at(loc() + enp)->has_moved() &&
				owner().enemy()->last().cur == loc() + 2 * adj + enp)	
			moves.insert(Move(MoveType::kEnpassant, loc(), loc() + adj + enp));
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

	return filter(moves);
}

std::set<Move> Knight::moves() {
	std::set<Move> moves;

	// L Move
	for (int i = 1; i <= 2; i ++) {
		std::vector<Position> adj = {
			Position(+i, 3-i), Position(+i, i-3),
			Position(-i, 3-i), Position(-i, i-3)};

		for (int j = 0; j < adj.size(); j++)
			moves.insert(Move(MoveType::kDefault, loc(), loc() + adj[j]));
	}

	return filter(moves);
}

std::set<Move> Bishop::moves() {
	std::set<Move> moves;

	// Diagonal Movement
	for (int i = -1; i <= 1; i += 2) {
		for (int j = -1; j <= 1; j+= 2) {
			Position diag = loc() + Position(i, j);
			while (diag >= Position(0, 0) && diag <= Position(8, 8) &&
					!owner().enemy()->at(diag - Position(i, j)) &&
					!owner().at(diag)) {
				moves.insert(Move(MoveType::kDefault, loc(), diag));
				diag += Position(i, j);
			}
		}
	}
	
	return filter(moves);
}

std::set<Move> Rook::moves() {
	std::set<Move> moves;

	// Vertical Movement
	for (int i = -1; i <= 1; i += 2) {
		Position vert = loc() + Position(i, 0);
		while (vert >= Position(0, 0) && vert <= Position(8, 8) &&
				!owner().enemy()->at(vert - Position(i, 0)) &&
				!owner().at(vert)) {
			moves.insert(Move(MoveType::kDefault, loc(), vert));
			vert += Position(i, 0);
		}
	}

	// Horizontal Movement
	for (int i = -1; i <= 1; i += 2) {
		Position hori = Position(0, i);
		while (hori >= Position(0, 0) && hori <= Position(8, 8) &&
				!owner().enemy()->at(hori - Position(0, i)) &&
				!owner().at(hori)) {
			moves.insert(Move(MoveType::kDefault, loc(), hori));
			hori += Position(0, i);
		}
	}

	return filter(moves);
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
	Piece* krook = owner().at(Position(loc().x, 7));
	Position right = Position(0, 1);
	if (!has_moved() && krook && !krook->has_moved() &&
			!owner().at(loc() + right) && 
			!owner().at(loc() + 2 * right) &&
			!owner().enemy()->at(loc() + right) &&
			!owner().enemy()->at(loc() + 2 * right) &&
			!owner().in_check() &&
			!owner().in_check(Move(MoveType::kDefault, loc(), loc() + right)))
		moves.insert(Move(MoveType::kCastleKingside, loc(), loc() + 2 * right));

	// Queenside Castling
	Piece* qrook = owner().at(Position(loc().x, 0));
	Position left = Position(0, -1);
	if(!has_moved() && qrook && !qrook->has_moved() && 
			!owner().at(loc() + left) && 
			!owner().at(loc() + 2 * left) && 
			!owner().at(loc() + 3 * left) &&
			!owner().enemy()->at(loc() + 1 * left) &&
			!owner().enemy()->at(loc() + 2 * left) &&
			!owner().enemy()->at(loc() + 3 * left) &&
			!owner().in_check() &&
			!owner().in_check(Move(MoveType::kDefault, loc(), loc() + left)))
		moves.insert(Move(MoveType::kCastleQueenside, loc(), loc() + 2 * left));
	
	// Adjacent Movement
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			Position adj = Position(i, j);
			if (i != 0 || j != 0)
				moves.insert(Move(MoveType::kDefault, loc(), loc() + adj));
		}
	}

	return filter(moves);
}

} // namespace chess
