#include "player.h"
#include "piece.h"
#include <vector>

namespace chess {

Player::Player(bool is_white) {
	_is_white = is_white;
	_opponent = nullptr;

	_live.push_back(new Rook( 	 *this, Position(is_white*7, 0)));
	_live.push_back(new Knight(	 *this, Position(is_white*7, 1)));
	_live.push_back(new Bishop(	 *this, Position(is_white*7, 2)));
	_live.push_back(new Queen(	 *this, Position(is_white*7, 3)));
	_live.push_back(new King(	 *this, Position(is_white*7, 4)));
	_live.push_back(new Bishop(	 *this, Position(is_white*7, 5)));
	_live.push_back(new Knight(	 *this, Position(is_white*7, 6)));
	_live.push_back(new Rook(	 *this, Position(is_white*7, 7)));
	
	for (int i = 0; i < 8; i++)
		_live.push_back(new Pawn(*this, Position(is_white*5+1, i)));
}

Player::Player(Player* opponent) : Player(!opponent->is_white()) {
	_opponent = opponent;
	opponent->_opponent = this;
}

Player::~Player() {

}

Piece* Player::piece(const Position& pos) {
	for (auto piece : _live)
		if (piece->loc() == pos)
			return piece;
	return nullptr;
}

bool Player::capture(Piece* piece) {
	if (piece == nullptr && !_live.empty()) {
		_live.erase(std::remove(_live.begin(), _live.end(), piece), _live.end());
		_dead.push_back(piece);
	}
}

} // namespace chess
