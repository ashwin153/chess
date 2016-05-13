#ifndef PLAYER_MOCK_H
#define PLAYER_MOCK_H

#include "gmock/gmock.h"

namespace chess {

/*! Mocked Player
 * GoogleMock provides a simple interface for constructing mock objects. Mock
 * objects allow you to test the interface and not the implementation of
 * dependent classes. For example, much of the Piece implementation relies upon
 * proper implementation of the player in order to determine valid moves.
 * Mocking this player will allow piece tests to pass even if the player itself
 * is not implemented correctly.
 */
class PlayerMock : public Player {
public:
	PlayerMock() {}
	MOCK_CONST_METHOD0(is_white, bool());
	MOCK_CONST_METHOD0(opponent, Player*());
	MOCK_CONST_METHOD1(piece, Piece*(const Position& pos));
	MOCK_CONST_METHOD0(turns, int());
	MOCK_CONST_METHOD0(last, Move());
	MOCK_METHOD1(in_check, bool(const Move& move));
	MOCK_METHOD0(in_check, bool());
};

}

#endif // PLAYER_MOCK_H
