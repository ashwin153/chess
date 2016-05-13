#ifndef PIECE_MOCK_H
#define PIECE_MOCK_H

#include "gmock/gmock.h"

namespace chess {

/*! Mocked Piece
 * This class mocks the pure virtual methods in the piece interface to allow us
 * to test other aspects of the class. It also will prove useful when testing
 * the player and game implementations, because it will remove the inter-
 * dependence on the piece implementation.
 */
class PieceMock : public Piece {
public:
	PieceMock(Player& player, const Position& loc) : Piece(player, loc) {}
	MOCK_CONST_METHOD0(to_string, std::string());
	MOCK_METHOD0(moves, std::set<Move>());
};

class PawnMock : public Pawn {
public:
	PawnMock(Player& player, const Position& loc) : Pawn(player, loc) {}
	MOCK_CONST_METHOD0(has_moved, bool());
};

} // namespace chess

#endif // PIECE_MOCK_H
