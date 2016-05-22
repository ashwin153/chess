#include "src/core/piece.h"
#include "src/core/player.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

namespace chess {

/*! Defines a mocked player.
 * GoogleMock provides a simple interface for constructing
 * mock objects. Mock objects allow you to test the interface
 * and not the implementation of dependent classes. For example,
 * much of the piece implementation relies upon the player
 * implementation of the "piece" method to determine where it is
 * relative to the other pieces on the board.
 */
class MockPlayer : public Player {
public:
	MOCK_METHOD0(is_white, bool());
	MOCK_METHOD0(opponent, Player*());
	MOCK_METHOD1(piece, Piece*(const Position& pos));
};

/*! Base fixture for all tests using a mocked player. 
 * Defines a mocked white and black player to provide
 * a simple interface way of accessing the player interface
 * without exposing the player implementation.
 *
 * Test fixtures are a feature of GoogleTest that allows
 * common data configuration  in multiple tests to be 
 * abstracted away. Test fixtures contain a constructor
 * and destructor that are run once and a SetUp and TearDown
 * method that are run before and after each test respectively.
 * Test fixture tests must be prefixed by TEST_F.
 */
class PieceTest : public testing::Test {
protected:
	MockPlayer* white;
	MockPlayer* black;

public:
	virtual void SetUp() {
		// Create the mocked players
		white = new MockPlayer();
		black = new MockPlayer();

		// Setup the colors of the players
		EXPECT_CALL(*white, is_white())
			.WillRepeatedly(testing::Return(true));
		EXPECT_CALL(*black, is_white())
			.WillRepeatedly(testing::Return(false));

		// Setup the opponent relationships between players
		EXPECT_CALL(*white, opponent())
			.WillRepeatedly(testing::Return(black));
		EXPECT_CALL(*black, opponent())
			.WillRepeatedly(testing::Return(white));
	}

	virtual void TearDown() {
		// Delete the mocked players
		delete white;
		delete black;
		white = nullptr;
		black = nullptr;
	}
};

class PawnTest : public PieceTest {};
class KnightTest : public PieceTest {};
class BishopTest : public PieceTest {};
class RookTest : public PieceTest {};
class KingTest : public PieceTest {};

// Piece Tests
TEST_F(PieceTest, IsValid_T) {
	Piece piece = Piece(*white, Position(0, 0));
	EXPECT_TRUE(piece.isValid(Position(1, 0)));
	EXPECT_TRUE(piece.isValid(Position(0, 1)));
}

TEST_F(PieceTest, IsValid_InvalidPosition_F) {
	Piece piece = Piece(*white, Position(0, 0));
	EXPECT_FALSE(piece.isValid(Position(-1, 0)));
	EXPECT_FALSE(piece.isValid(Position(0, -1)));
	EXPECT_FALSE(piece.isValid(Position(8, 0)));
	EXPECT_FALSE(piece.isValid(Position(0, 8)));
	EXPECT_FALSE(piece.isValid(Position(-1, -1)));
	EXPECT_FALSE(piece.isValid(Position(8, 8)));	
}

TEST_F(PieceTest, IsValid_AlliedPiece_F) {
	Piece piece = Piece(*white, Position(0, 0));
	EXPECT_CALL(*white, piece(Position(1, 0)))
		.Times(1)
		.WillOnce(testing::Return(&piece));	
	EXPECT_FALSE(piece.isValid(Position(1, 0)));
}

TEST_F(PieceTest, IsValid_EnemyPiece_F) {

}

TEST_F(PieceTest, IsValid_InCheck_F) {

}

// Pawn Tests
// Movement Tests
TEST_F(PawnTest, IsValid_ForwardMove_T) {
	Pawn wpawn = Pawn(*white, Position(5, 1));
	Pawn bpawn = Pawn(*black, Position(2, 1));
	EXPECT_TRUE(wpawn.isValid(Position(4, 1)));
	EXPECT_TRUE(bpawn.isValid(Position(3, 1)));	
}

TEST_F(PawnTest, isValid_2ForwardMove_T) {

}

TEST_F(PawnTest, isValid_NonForwardMove_F) {
	Pawn wpawn = Pawn(*white, Position(5, 1));
	Pawn bpawn = Pawn(*black, Position(5, 1));

	EXPECT_CALL(*white, piece(testing::_))
		.WillRepeatedly(testing::Return(nullptr));
	EXPECT_CALL(*black, piece(testing::_))
		.WillRepeatedly(testing::Return(nullptr));
	
	// Pawns may only move forward
	EXPECT_FALSE(wpawn.isValid(Position(6, 1)));
	EXPECT_FALSE(bpawn.isValid(Position(4, 1)));

	for (int x = 4; x <= 6; x++) {
		EXPECT_FALSE(wpawn.isValid(Position(x, 0)));
		EXPECT_FALSE(wpawn.isValid(Position(x, 2)));
		EXPECT_FALSE(bpawn.isValid(Position(x, 0)));
		EXPECT_FALSE(bpawn.isValid(Position(x, 2)));
	}
}

TEST_F(PawnTest, isValid_Enpassant_F) {

}

// Capturing Tests
TEST_F(PawnTest, isValid_successDiagonalCapture) {

}

TEST_F(PawnTest, isValid_failNonDiagonalCapture) {

}

/*
// KnightTest
// Movement Tests
TEST_F(KnightTest, isValid_successValidMove) {

}

TEST_F(KnightTest, isValid_failInvalidMove) {

}

// BishopTest
// Movement Tests
TEST(BishopTest, isValid_successDiagonalMove) {

}

TEST(BishopTest, isValid_failAllyObstructedMove) {

}

TEST(BishopTest, isValid_failEnemyObstructedMove) {

}

TEST(BishopTest, isValid_failNonDiagonalMove) {

}

// RookTest
// Movement Tests
TEST_F(RookTest, isValid_successStraightMove) {
	Rook rook = Rook(*white, Position(3, 3));
	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 8; y++)
			if (x == 3 != y == 3)
				EXPECT_TRUE(rook.isValid(Position(x, y)));
}

TEST_F(RookTest, isValid_failNonStraightMove) {
	Rook rook = Rook(*white, Position(3, 3));
	for (int x = 0; x < 8; x++)
		for (int y = 0; y < 8; y++)
			if(x != 3 && y != 3)
				EXPECT_FALSE(rook.isValid(Position(x, y)))
}

TEST_F(RookTest, isValid_failAllyObstructedMove) {

}

TEST_F(RookTest, isValid_failEnemyObstructedMove) {

}

// KingTest
// Movement Tests
TEST_F(KingTest, isValid_successAdjacentMove) {
	King king = King(*white, Position(3, 3));
	for (int x = 2; x <= 4; x++)
		for (int y = 2; y <= 4; y++)
			if (x != 3 && y != 3)
				EXPECT_TRUE(king.isValid(Position(x, y)));
}

TEST_F(KingTest, isValid_failNonAdjacentMove) {

}

// Castling Tests (King Side + Queen Side)
TEST_F(KingTest, isValid_successCastle) {
			
}

TEST_F(KingTest, isValid_failCastleAfterMoving) {

}

TEST_F(KingTest, isValid_failCastleAllyObstructed) {

}

TEST_F(KingTest, isValid_failCastleEnemyObstructed) {

}

TEST_F(KingTest, isValid_failCastleOutOfCheck) {

}

TEST_F(KingTest, isValid_failCastleThroughCheck) {

}

TEST_F(KingTest, isValid_failCastleIntoCheck) {

}

*/

}
