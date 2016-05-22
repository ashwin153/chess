#include "src/core/piece.h"
#include "src/core/player.h"
#include "src/core/game.h"
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
	MockPlayer() {}
	MOCK_CONST_METHOD0(is_white, bool());
	MOCK_METHOD1(in_check, bool(const Move& move));
	MOCK_CONST_METHOD0(opponent, Player*());
	MOCK_CONST_METHOD1(piece, Piece*(const Position& pos));
	MOCK_METHOD1(capture, Piece*(const Position& pos));
	MOCK_METHOD1(uncapture, Piece*(const Position& pos));
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
	testing::NiceMock<MockPlayer>* white;
	testing::NiceMock<MockPlayer>* black;

public:
	virtual void SetUp() {
		// Create the mocked players
		white = new testing::NiceMock<MockPlayer>();
		black = new testing::NiceMock<MockPlayer>();

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
	
		// Set the player to not be in check
		EXPECT_CALL(*white, in_check(testing::_))
			.WillRepeatedly(testing::Return(false));
		EXPECT_CALL(*black, in_check(testing::_))
			.WillRepeatedly(testing::Return(false));

		// Set the player to return nullptr for piece by default
		EXPECT_CALL(*white, piece(testing::_))
			.WillRepeatedly(testing::Return(nullptr));
		EXPECT_CALL(*black, piece(testing::_))
			.WillRepeatedly(testing::Return(nullptr));
		
		// Set the player to not capture pieces by default
		EXPECT_CALL(*white, capture(testing::_))
			.WillRepeatedly(testing::Return(nullptr));
		EXPECT_CALL(*black, piece(testing::_))
			.WillRepeatedly(testing::Return(nullptr));

		// Set the player to return nullptr for piece by default
		EXPECT_CALL(*white, uncapture(testing::_))
			.WillRepeatedly(testing::Return(nullptr));
		EXPECT_CALL(*black, uncapture(testing::_))
			.WillRepeatedly(testing::Return(nullptr));

	}

	virtual void TearDown() {
		delete white;
		delete black;
		white = nullptr;
		black = nullptr;
	}
};

class PawnTest : public PieceTest{};
class KnightTest : public PieceTest {};
class BishopTest : public PieceTest {};
class RookTest : public PieceTest {};
class QueenTest : public PieceTest {};
class KingTest : public PieceTest {};

// Piece Tests
TEST_F(PieceTest, IsValid_T) {
	Piece piece = Piece(*white, Position(0, 0));
	EXPECT_TRUE(piece.is_valid(Position(1, 0)));
	EXPECT_TRUE(piece.is_valid(Position(0, 1)));
}

TEST_F(PieceTest, IsValid_InvalidPosition_F) {
	Piece piece = Piece(*white, Position(0, 0));
	EXPECT_FALSE(piece.is_valid(Position(-1, 0)));
	EXPECT_FALSE(piece.is_valid(Position(0, -1)));
	EXPECT_FALSE(piece.is_valid(Position(8, 0)));
	EXPECT_FALSE(piece.is_valid(Position(0, 8)));
	EXPECT_FALSE(piece.is_valid(Position(-1, -1)));
	EXPECT_FALSE(piece.is_valid(Position(8, 8)));	
}

TEST_F(PieceTest, IsValid_AlliedPiece_F) {
	Piece piece = Piece(*white, Position(0, 0));
	EXPECT_CALL(*white, piece(Position(1, 0)))
		.WillOnce(testing::Return(&piece));	
	EXPECT_FALSE(piece.is_valid(Position(1, 0)));
}

TEST_F(PieceTest, IsValid_EnemyPiece_T) {
	Piece wpiece = Piece(*white, Position(0, 0));
	Piece bpiece = Piece(*black, Position(1, 0));
	EXPECT_CALL(*white, piece(Position(1, 0)))
		.WillOnce(testing::Return(&bpiece));	
	EXPECT_FALSE(wpiece.is_valid(Position(1, 0)));
}

// Pawn Tests
TEST_F(PawnTest, IsValid_ForwardMove_T) {
	Pawn wpawn = Pawn(*white, Position(6, 1));
	Pawn bpawn = Pawn(*black, Position(1, 1));	
	EXPECT_TRUE(wpawn.is_valid(Position(5, 1)));
	EXPECT_TRUE(bpawn.is_valid(Position(2, 1)));
}

TEST_F(PawnTest, is_valid_2ForwardMove_T) {
	Pawn wpawn = Pawn(*white, Position(6, 1));
	Pawn bpawn = Pawn(*black, Position(1, 1));
	EXPECT_TRUE(wpawn.is_valid(Position(4, 1)));
	EXPECT_TRUE(bpawn.is_valid(Position(3, 1)));
}

TEST_F(PawnTest, is_valid_2ForwardMoveAfterFirst_F) {
	Pawn wpawn = Pawn(*white, Position(6, 1));
	Pawn bpawn = Pawn(*black, Position(1, 1));
	wpawn.move(Position(5, 1));
	bpawn.move(Position(2, 1));
	EXPECT_FALSE(wpawn.is_valid(Position(3, 1)));
	EXPECT_FALSE(bpawn.is_valid(Position(4, 1)));
}

TEST_F(PawnTest, is_valid_NonForwardMove_F) {
	Pawn wpawn = Pawn(*white, Position(6, 1));
	Pawn bpawn = Pawn(*black, Position(1, 1));
	EXPECT_FALSE(wpawn.is_valid(Position(7, 1)));
	EXPECT_FALSE(bpawn.is_valid(Position(0, 1)));
	for (int x = -1; x <= 1; x++) {
		EXPECT_FALSE(wpawn.is_valid(Position(6+x, 0)));
		EXPECT_FALSE(wpawn.is_valid(Position(6+x, 2)));
		EXPECT_FALSE(bpawn.is_valid(Position(1+x, 0)));
		EXPECT_FALSE(bpawn.is_valid(Position(1+x, 2)));
	}
}

TEST_F(PawnTest, is_valid_Enpassant_F) {
	Pawn wpawn = Pawn(*white, Position(6, 1));
	Pawn bpawn = Pawn(*black, Position(1, 1));

	EXPECT_CALL(*black, piece(Position(4, 2)))
		.WillRepeatedly(testing::Return(&bpawn));
	EXPECT_CALL(*white, piece(Position(3, 0)))
		.WillRepeatedly(testing::Return(&wpawn));

	EXPECT_FALSE(wpawn.is_valid(Position(4, 1)));
	EXPECT_FALSE(bpawn.is_valid(Position(3, 1)));	
}

TEST_F(PawnTest, is_valid_DiagonalCapture_T) {
	Pawn wpawn = Pawn(*white, Position(6, 1));
   	Pawn bpawn = Pawn(*black, Position(1, 1));

	EXPECT_CALL(*black, piece(Position(5, 0)))
		.WillRepeatedly(testing::Return(&bpawn));
	EXPECT_CALL(*white, piece(Position(2, 0)))
		.WillRepeatedly(testing::Return(&wpawn));
	EXPECT_CALL(*black, piece(Position(5, 2)))
		.WillRepeatedly(testing::Return(&bpawn));
	EXPECT_CALL(*white, piece(Position(2, 2)))
		.WillRepeatedly(testing::Return(&wpawn));
	
	EXPECT_TRUE(wpawn.is_valid(Position(5, 0)));
	EXPECT_TRUE(wpawn.is_valid(Position(5, 2)));
	EXPECT_TRUE(bpawn.is_valid(Position(2, 0)));
	EXPECT_TRUE(bpawn.is_valid(Position(2, 2)));	
}

TEST_F(PawnTest, is_valid_NonDiagonalCapture_F) {
	Pawn wpawn = Pawn(*white, Position(6, 1));
	Pawn bpawn = Pawn(*black, Position(1, 1));

	EXPECT_CALL(*black, piece(Position(5, 1)))
		.WillRepeatedly(testing::Return(&bpawn));
	EXPECT_CALL(*white, piece(Position(2, 1)))
		.WillRepeatedly(testing::Return(&wpawn));
	EXPECT_CALL(*black, piece(Position(7, 0)))
		.WillRepeatedly(testing::Return(&bpawn));
	EXPECT_CALL(*white, piece(Position(0, 2)))
		.WillRepeatedly(testing::Return(&wpawn));

	EXPECT_FALSE(wpawn.is_valid(Position(5, 1)));
	EXPECT_FALSE(wpawn.is_valid(Position(7, 0)));
	EXPECT_FALSE(bpawn.is_valid(Position(2, 1)));
	EXPECT_FALSE(bpawn.is_valid(Position(0, 2)));
}


// KnightTest
TEST_F(KnightTest, is_valid_ValidMove_T) {
	Knight knight = Knight(*white, Position(4, 3));
	EXPECT_TRUE(knight.is_valid(Position(2, 2)));
	EXPECT_TRUE(knight.is_valid(Position(6, 4)));
	EXPECT_TRUE(knight.is_valid(Position(3, 1)));
	EXPECT_TRUE(knight.is_valid(Position(5, 4)));	
}

TEST_F(KnightTest, is_valid_InvalidMove_F) {
	Knight knight = Knight(*white, Position(4, 3));
	EXPECT_FALSE(knight.is_valid(Position(1, 3)));
	EXPECT_FALSE(knight.is_valid(Position(6, 5)));
}

// BishopTest
TEST_F(BishopTest, is_valid_DiagonalMove_T) {
	Bishop bishop = Bishop(*white, Position(4, 4));
	EXPECT_TRUE(bishop.is_valid(Position(0, 0)));
	EXPECT_TRUE(bishop.is_valid(Position(7, 1)));
	EXPECT_TRUE(bishop.is_valid(Position(1, 7)));
	EXPECT_TRUE(bishop.is_valid(Position(7, 7)));
}

TEST_F(BishopTest, is_valid_AllyObstructedMove_F) {
	Bishop bishop = Bishop(*white, Position(4, 4));
	EXPECT_CALL(*white, piece(Position(2, 2)))
		.WillRepeatedly(testing::Return(&bishop));
	EXPECT_FALSE(bishop.is_valid(Position(0, 0)));
}

TEST_F(BishopTest, is_valid_EnemyObstructedMove_F) {
	Bishop bishop = Bishop(*white, Position(4, 4));
	EXPECT_CALL(*black, piece(Position(2, 2)))
		.WillRepeatedly(testing::Return(&bishop));
	EXPECT_FALSE(bishop.is_valid(Position(0, 0)));
}

TEST_F(BishopTest, is_valid_NonDiagonalMove_F) {
	Bishop bishop = Bishop(*white, Position(4, 4));
	EXPECT_FALSE(bishop.is_valid(Position(3, 4)));
	EXPECT_FALSE(bishop.is_valid(Position(5, 4)));
	EXPECT_FALSE(bishop.is_valid(Position(7, 0)));
	EXPECT_FALSE(bishop.is_valid(Position(0, 7)));
}

// RookTest
TEST_F(RookTest, is_valid_StraightMove_T) {
	Rook rook = Rook(*white, Position(3, 3));
	EXPECT_TRUE(rook.is_valid(Position(0, 3)));
	EXPECT_TRUE(rook.is_valid(Position(7, 3)));
	EXPECT_TRUE(rook.is_valid(Position(3, 0)));
	EXPECT_TRUE(rook.is_valid(Position(3, 7)));
}

TEST_F(RookTest, is_valid_NonStraightMove_F) {
	Rook rook = Rook(*white, Position(3, 3));
	EXPECT_FALSE(rook.is_valid(Position(4, 2)));
	EXPECT_FALSE(rook.is_valid(Position(1, 2)));
	EXPECT_FALSE(rook.is_valid(Position(4, 4)));
	EXPECT_FALSE(rook.is_valid(Position(1, 4)));
}

TEST_F(RookTest, is_valid_AllyObstructedMove_F) {
	Rook rook = Rook(*white, Position(3, 3));
	EXPECT_CALL(*white, piece(Position(2, 3)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_FALSE(rook.is_valid(Position(0, 3)));
}

TEST_F(RookTest, is_valid_EnemyObstructedMove_F) {
	Rook rook = Rook(*white, Position(3, 3));
	EXPECT_CALL(*black, piece(Position(2, 3)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_FALSE(rook.is_valid(Position(0, 3)));
}

// KingTest
TEST_F(KingTest, is_valid_AdjacentMove_T) {
	King king = King(*white, Position(3, 3));
	EXPECT_TRUE(king.is_valid(Position(2, 2)));
	EXPECT_TRUE(king.is_valid(Position(2, 3)));
	EXPECT_TRUE(king.is_valid(Position(2, 4)));
	EXPECT_TRUE(king.is_valid(Position(3, 2)));
	EXPECT_TRUE(king.is_valid(Position(3, 4)));
	EXPECT_TRUE(king.is_valid(Position(4, 2)));
	EXPECT_TRUE(king.is_valid(Position(4, 3)));
	EXPECT_TRUE(king.is_valid(Position(4, 4)));
}

TEST_F(KingTest, is_valid_NonAdjacentMove_F) {
	King king = King(*white, Position(3, 3));
	EXPECT_FALSE(king.is_valid(Position(5, 3)));
	EXPECT_FALSE(king.is_valid(Position(1, 1)));
}

// Castling Tests (King Side + Queen Side)
TEST_F(KingTest, move_KingsideCastle_T) {
	King king = King(*white, Position(7, 4));
	Rook rook = Rook(*white, Position(7, 7));
	EXPECT_CALL(*white, piece(Position(7, 7)))
		.WillRepeatedly(testing::Return(&rook));
	king.move(Position(7, 6));
	EXPECT_EQ(Position(7, 5), rook.loc());
}

TEST_F(KingTest, move_QueensideCastle_T) {
	King king = King(*white, Position(7, 4));
	Rook rook = Rook(*white, Position(7, 0));
	EXPECT_CALL(*white, piece(Position(7, 0)))
		.WillRepeatedly(testing::Return(&rook));
	king.move(Position(7, 2));
	EXPECT_EQ(Position(7, 3), rook.loc());
}

TEST_F(KingTest, is_valid_ValidCastle_T) {
	King king  = King(*white, Position(7, 4));
	Rook krook = Rook(*white, Position(7, 7));
	Rook qrook = Rook(*white, Position(7, 0));	
	EXPECT_CALL(*white, piece(Position(7, 7)))
		.WillRepeatedly(testing::Return(&krook));
	EXPECT_CALL(*white, piece(Position(7, 0)))
		.WillRepeatedly(testing::Return(&qrook));
	EXPECT_TRUE(king.is_valid(Position(7, 6)));
	EXPECT_TRUE(king.is_valid(Position(7, 2)));
}

TEST_F(KingTest, is_valid_CastleAfterKingMove_F) {
	King king = King(*white, Position(7, 5));
	Rook rook = Rook(*white, Position(7, 7));
	king.move(Position(7, 4));
	EXPECT_CALL(*white, piece(Position(7, 7)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_FALSE(king.is_valid(Position(7, 6)));
}

TEST_F(KingTest, is_valid_CastleAfterRookMove_F) {
	King king = King(*white, Position(7, 4));
	Rook rook = Rook(*white, Position(7, 6));
	rook.move(Position(7, 7));
	EXPECT_CALL(*white, piece(Position(7, 7)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_FALSE(king.is_valid(Position(7, 6)));
}

TEST_F(KingTest, is_valid_CastleAllyObstructed_F) {
	King king = King(*white, Position(7, 4));
	Rook rook = Rook(*white, Position(7, 0));
	EXPECT_CALL(*white, piece(Position(7, 0)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_CALL(*white, piece(Position(7, 1)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_FALSE(king.is_valid(Position(7, 2)));
}

TEST_F(KingTest, is_valid_CastleEnemyObstructed_F) {
	King king = King(*white, Position(7, 4));
	Rook rook = Rook(*white, Position(7, 7));
	EXPECT_CALL(*white, piece(Position(7, 7)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_CALL(*black, piece(Position(7, 5)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_FALSE(king.is_valid(Position(7, 6)));
}

TEST_F(KingTest, is_valid_CastleOutOfCheck_F) {
	King king = King(*white, Position(7, 4));
	Rook rook = Rook(*white, Position(7, 8));
	EXPECT_CALL(*white, piece(Position(7, 7)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_CALL(*white, in_check(testing::_))
		.WillOnce(testing::Return(true));
	EXPECT_FALSE(king.is_valid(Position(7, 6)));
}

TEST_F(KingTest, is_valid_CastleThroughCheck_F) {
	King king = King(*white, Position(7, 4));
	Rook rook = Rook(*white, Position(7, 8));
	EXPECT_CALL(*white, piece(Position(7, 7)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_CALL(*white, in_check(testing::_))
		.WillOnce(testing::Return(false))
		.WillOnce(testing::Return(true));
	EXPECT_FALSE(king.is_valid(Position(7, 6)));
	
}

} // namespace chess
