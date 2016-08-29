#include "piece_mock.h"
#include "player_mock.h"

#include "src/core/piece.h"
#include "src/core/move.h"
#include "src/core/player.h"

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <set>
#include <iostream>

namespace chess {

/*!
 * Defines a mocked white and black player to provide a simple interface for
 * accessing the player interface without exposing the player implementation.
 *
 * Test fixtures are a feature of GoogleTest that allows common data configs
 * across multiple tests to be abstracted away. Test fixtures contain a
 * constructor and destructor that are run once and a SetUp and TearDown
 * method that are run before and after each test respectively. Note: test
 * fixtures must be prefixed by TEST_F.
 */
class PieceTest : public testing::Test {
protected:
	testing::NiceMock<PlayerMock>* white;
	testing::NiceMock<PlayerMock>* black;

public:
	virtual void SetUp() {
		// Create the mocked players
		white = new testing::NiceMock<PlayerMock>();
		black = new testing::NiceMock<PlayerMock>();

		// Setup the colors of the players
		ON_CALL(*white, is_white())
			.WillByDefault(testing::Return(true));
		ON_CALL(*black, is_white())
			.WillByDefault(testing::Return(false));	

		// Setup the opponent relationships between players
		ON_CALL(*white, opponent())
			.WillByDefault(testing::Return(black));
		ON_CALL(*black, opponent())
			.WillByDefault(testing::Return(white));
	}

	virtual void TearDown() {
		delete white;
		delete black;
		white = nullptr;
		black = nullptr;
	}
};

typedef PieceTest PawnTest;
typedef PieceTest RookTest;
typedef PieceTest KingTest;
typedef PieceTest QueenTest;
typedef PieceTest KnightTest;
typedef PieceTest BishopTest;

TEST_F(PawnTest, Moves_WhiteForward) {
	Pawn pawn(*white, Position("e2"));	
	EXPECT_THAT(pawn.moves(), testing::UnorderedElementsAre(
		Move(MoveType::kDefault, pawn.loc(), Position("e3")),
		Move(MoveType::kDefault, pawn.loc(), Position("e4"))));
}

TEST_F(PawnTest, Moves_BlackForward) {
	Pawn pawn(*black, Position("e7"));
	EXPECT_THAT(pawn.moves(), testing::UnorderedElementsAre(
		Move(MoveType::kDefault, pawn.loc(), Position("e6")),
		Move(MoveType::kDefault, pawn.loc(), Position("e5"))));
}

/*
TEST_F(PawnTest, Valid_2ForwardMove_T) {
	Pawn wpawn = Pawn(*white, Position(6, 1));
	Pawn bpawn = Pawn(*black, Position(1, 1));
	EXPECT_TRUE(wpawn.valid(Position(4, 1)));
	EXPECT_TRUE(bpawn.valid(Position(3, 1)));
}

TEST_F(PawnTest, Valid_2ForwardMoveAfterFirst_F) {
	Pawn wpawn = Pawn(*white, Position(6, 1));
	Pawn bpawn = Pawn(*black, Position(1, 1));
	wpawn.move(Position(5, 1));
	bpawn.move(Position(2, 1));
	EXPECT_FALSE(wpawn.valid(Position(3, 1)));
	EXPECT_FALSE(bpawn.valid(Position(4, 1)));
}

TEST_F(PawnTest, Valid_NonForwardMove_F) {
	Pawn wpawn = Pawn(*white, Position(6, 1));
	Pawn bpawn = Pawn(*black, Position(1, 1));
	EXPECT_FALSE(wpawn.valid(Position(7, 1)));
	EXPECT_FALSE(bpawn.valid(Position(0, 1)));
	for (int x = -1; x <= 1; x++) {
		EXPECT_FALSE(wpawn.valid(Position(6+x, 0)));
		EXPECT_FALSE(wpawn.valid(Position(6+x, 2)));
		EXPECT_FALSE(bpawn.valid(Position(1+x, 0)));
		EXPECT_FALSE(bpawn.valid(Position(1+x, 2)));
	}
}

TEST_F(PawnTest, Valid_Enpassant_F) {
	Pawn wpawn = Pawn(*white, Position(6, 1));
	Pawn bpawn = Pawn(*black, Position(1, 1));

	EXPECT_CALL(*black, piece(Position(4, 2)))
		.WillRepeatedly(testing::Return(&bpawn));
	EXPECT_CALL(*white, piece(Position(3, 0)))
		.WillRepeatedly(testing::Return(&wpawn));

	EXPECT_FALSE(wpawn.valid(Position(4, 1)));
	EXPECT_FALSE(bpawn.valid(Position(3, 1)));	
}

TEST_F(PawnTest, Valid_DiagonalCapture_T) {
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
	
	EXPECT_TRUE(wpawn.valid(Position(5, 0)));
	EXPECT_TRUE(wpawn.valid(Position(5, 2)));
	EXPECT_TRUE(bpawn.valid(Position(2, 0)));
	EXPECT_TRUE(bpawn.valid(Position(2, 2)));	
}

TEST_F(PawnTest, Valid_NonDiagonalCapture_F) {
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

	EXPECT_FALSE(wpawn.valid(Position(5, 1)));
	EXPECT_FALSE(wpawn.valid(Position(7, 0)));
	EXPECT_FALSE(bpawn.valid(Position(2, 1)));
	EXPECT_FALSE(bpawn.valid(Position(0, 2)));
}

// KnightTest
TEST_F(KnightTest, Valid_ValidMove_T) {
	Knight knight = Knight(*white, Position("f2"));
	std::set<Move> moves = knight.moves();
	

	EXPECT_NE(moves.end(), moves.find(
		Move(MoveType::kDefault, Position("f2"), Position("h3"))));
	EXPECT_NE(moves.end(), moves.find(
		Move(MoveType::kDefault, Position("f2"), Position(""))));
}

TEST_F(KnightTest, Valid_InvalidMove_F) {
	Knight knight = Knight(*white, Position(4, 3));
	EXPECT_FALSE(knight.valid(Position(1, 3)));
	EXPECT_FALSE(knight.valid(Position(6, 5)));
}

// BishopTest
TEST_F(BishopTest, Valid_DiagonalMove_T) {
	Bishop bishop = Bishop(*white, Position(4, 4));
	EXPECT_TRUE(bishop.valid(Position(0, 0)));
	EXPECT_TRUE(bishop.valid(Position(7, 1)));
	EXPECT_TRUE(bishop.valid(Position(1, 7)));
	EXPECT_TRUE(bishop.valid(Position(7, 7)));
}

TEST_F(BishopTest, Valid_AllyObstructedMove_F) {
	Bishop bishop = Bishop(*white, Position(4, 4));
	EXPECT_CALL(*white, piece(Position(2, 2)))
		.WillRepeatedly(testing::Return(&bishop));
	EXPECT_FALSE(bishop.valid(Position(0, 0)));
}

TEST_F(BishopTest, Valid_EnemyObstructedMove_F) {
	Bishop bishop = Bishop(*white, Position(4, 4));
	EXPECT_CALL(*black, piece(Position(2, 2)))
}

// Piece Tests
TEST_F(PieceTest, Valid_T) {
	PieceMock piece(*white, Position(0, 0));
	EXPECT_TRUE(piece.valid(Position(1, 0)));
	EXPECT_TRUE(piece.valid(Position(0, 1)));
}

TEST_F(PieceTest, Valid_InvalidPosition_F) {
	PieceMock piece(*white, Position(0, 0));
	EXPECT_FALSE(piece.valid(Position(-1, 0)));
	EXPECT_FALSE(piece.valid(Position(0, -1)));
	EXPECT_FALSE(piece.valid(Position(8, 0)));
	EXPECT_FALSE(piece.valid(Position(0, 8)));
	EXPECT_FALSE(piece.valid(Position(-1, -1)));
	EXPECT_FALSE(piece.valid(Position(8, 8)));	
}

TEST_F(PieceTest, Valid_AlliedPiece_F) {
	PieceMock piece(*white, Position(0, 0));
	EXPECT_CALL(*white, piece(Position(1, 0)))
		.WillOnce(testing::Return(&piece));	
	EXPECT_FALSE(piece.valid(Position(1, 0)));
}

TEST_F(PieceTest, Valid_EnemyPiece_T) {
	PieceMock wpiece(*white, Position(0, 0));
	PieceMock bpiece(*black, Position(1, 0));
	EXPECT_CALL(*white, piece(Position(1, 0)))
		.WillOnce(testing::Return(&bpiece));	
	EXPECT_FALSE(wpiece.valid(Position(1, 0)));
}



// Pawn Tests
TEST_F(PawnTest, Moves_WhiteForward) {
	PawnMock wpawn(*white, Position("e2"));
	ON_CALL(wpawn, has_moved())
		.WillByDefault(testing::Return(true));

	// White pawns that have moved previously and are not in a position to
	// capture, may only move to a position of increasing rank.
	std::set<Move> wmoves = wpawn.moves();
	ASSERT_EQ(1, wmoves.size());
	EXPECT_NE(wmoves.end(), wmoves.find(
		Move(MoveType::kDefault, Position("e2"), Position("e3"))));
}

TEST_F(PawnTest, Valid_2ForwardMove_T) {
	Pawn wpawn = Pawn(*white, Position(6, 1));
	Pawn bpawn = Pawn(*black, Position(1, 1));
	EXPECT_TRUE(wpawn.valid(Position(4, 1)));
	EXPECT_TRUE(bpawn.valid(Position(3, 1)));
}

TEST_F(PawnTest, Valid_2ForwardMoveAfterFirst_F) {
	Pawn wpawn = Pawn(*white, Position(6, 1));
	Pawn bpawn = Pawn(*black, Position(1, 1));
	wpawn.move(Position(5, 1));
	bpawn.move(Position(2, 1));
	EXPECT_FALSE(wpawn.valid(Position(3, 1)));
	EXPECT_FALSE(bpawn.valid(Position(4, 1)));
}

TEST_F(PawnTest, Valid_NonForwardMove_F) {
	Pawn wpawn = Pawn(*white, Position(6, 1));
	Pawn bpawn = Pawn(*black, Position(1, 1));
	EXPECT_FALSE(wpawn.valid(Position(7, 1)));
	EXPECT_FALSE(bpawn.valid(Position(0, 1)));
	for (int x = -1; x <= 1; x++) {
		EXPECT_FALSE(wpawn.valid(Position(6+x, 0)));
		EXPECT_FALSE(wpawn.valid(Position(6+x, 2)));
		EXPECT_FALSE(bpawn.valid(Position(1+x, 0)));
		EXPECT_FALSE(bpawn.valid(Position(1+x, 2)));
	}
}

TEST_F(PawnTest, Valid_Enpassant_F) {
	Pawn wpawn = Pawn(*white, Position(6, 1));
	Pawn bpawn = Pawn(*black, Position(1, 1));

	EXPECT_CALL(*black, piece(Position(4, 2)))
		.WillRepeatedly(testing::Return(&bpawn));
	EXPECT_CALL(*white, piece(Position(3, 0)))
		.WillRepeatedly(testing::Return(&wpawn));

	EXPECT_FALSE(wpawn.valid(Position(4, 1)));
	EXPECT_FALSE(bpawn.valid(Position(3, 1)));	
}

TEST_F(PawnTest, Valid_DiagonalCapture_T) {
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
	
	EXPECT_TRUE(wpawn.valid(Position(5, 0)));
	EXPECT_TRUE(wpawn.valid(Position(5, 2)));
	EXPECT_TRUE(bpawn.valid(Position(2, 0)));
	EXPECT_TRUE(bpawn.valid(Position(2, 2)));	
}

TEST_F(PawnTest, Valid_NonDiagonalCapture_F) {
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

	EXPECT_FALSE(wpawn.valid(Position(5, 1)));
	EXPECT_FALSE(wpawn.valid(Position(7, 0)));
	EXPECT_FALSE(bpawn.valid(Position(2, 1)));
	EXPECT_FALSE(bpawn.valid(Position(0, 2)));
}

// KnightTest
TEST_F(KnightTest, Valid_ValidMove_T) {
	Knight knight = Knight(*white, Position("f2"));
	std::set<Move> moves = knight.moves();
	

	EXPECT_NE(moves.end(), moves.find(
		Move(MoveType::kDefault, Position("f2"), Position("h3"))));
	EXPECT_NE(moves.end(), moves.find(
		Move(MoveType::kDefault, Position("f2"), Position(""))));
}

TEST_F(KnightTest, Valid_InvalidMove_F) {
	Knight knight = Knight(*white, Position(4, 3));
	EXPECT_FALSE(knight.valid(Position(1, 3)));
	EXPECT_FALSE(knight.valid(Position(6, 5)));
}

// BishopTest
TEST_F(BishopTest, Valid_DiagonalMove_T) {
	Bishop bishop = Bishop(*white, Position(4, 4));
	EXPECT_TRUE(bishop.valid(Position(0, 0)));
	EXPECT_TRUE(bishop.valid(Position(7, 1)));
	EXPECT_TRUE(bishop.valid(Position(1, 7)));
	EXPECT_TRUE(bishop.valid(Position(7, 7)));
}

TEST_F(BishopTest, Valid_AllyObstructedMove_F) {
	Bishop bishop = Bishop(*white, Position(4, 4));
	EXPECT_CALL(*white, piece(Position(2, 2)))
		.WillRepeatedly(testing::Return(&bishop));
	EXPECT_FALSE(bishop.valid(Position(0, 0)));
}

TEST_F(BishopTest, Valid_EnemyObstructedMove_F) {
	Bishop bishop = Bishop(*white, Position(4, 4));
	EXPECT_CALL(*black, piece(Position(2, 2)))
		.WillRepeatedly(testing::Return(&bishop));
	EXPECT_FALSE(bishop.valid(Position(0, 0)));
}

TEST_F(BishopTest, Valid_NonDiagonalMove_F) {
	Bishop bishop = Bishop(*white, Position(4, 4));
	EXPECT_FALSE(bishop.valid(Position(3, 4)));
	EXPECT_FALSE(bishop.valid(Position(5, 4)));
	EXPECT_FALSE(bishop.valid(Position(7, 0)));
	EXPECT_FALSE(bishop.valid(Position(0, 7)));
}

// RookTest
TEST_F(RookTest, Valid_StraightMove_T) {
	Rook rook = Rook(*white, Position(3, 3));
	EXPECT_TRUE(rook.valid(Position(0, 3)));
	EXPECT_TRUE(rook.valid(Position(7, 3)));
	EXPECT_TRUE(rook.valid(Position(3, 0)));
	EXPECT_TRUE(rook.valid(Position(3, 7)));
}

TEST_F(RookTest, Valid_NonStraightMove_F) {
	Rook rook = Rook(*white, Position(3, 3));
	EXPECT_FALSE(rook.valid(Position(4, 2)));
	EXPECT_FALSE(rook.valid(Position(1, 2)));
	EXPECT_FALSE(rook.valid(Position(4, 4)));
	EXPECT_FALSE(rook.valid(Position(1, 4)));
}

TEST_F(RookTest, Valid_AllyObstructedMove_F) {
	Rook rook = Rook(*white, Position(3, 3));
	EXPECT_CALL(*white, piece(Position(2, 3)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_FALSE(rook.valid(Position(0, 3)));
}

TEST_F(RookTest, Valid_EnemyObstructedMove_F) {
	Rook rook = Rook(*white, Position(3, 3));
	EXPECT_CALL(*black, piece(Position(2, 3)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_FALSE(rook.valid(Position(0, 3)));
}

// KingTest
TEST_F(KingTest, Valid_AdjacentMove_T) {
	King king = King(*white, Position(3, 3));
	EXPECT_TRUE(king.valid(Position(2, 2)));
	EXPECT_TRUE(king.valid(Position(2, 3)));
	EXPECT_TRUE(king.valid(Position(2, 4)));
	EXPECT_TRUE(king.valid(Position(3, 2)));
	EXPECT_TRUE(king.valid(Position(3, 4)));
	EXPECT_TRUE(king.valid(Position(4, 2)));
	EXPECT_TRUE(king.valid(Position(4, 3)));
	EXPECT_TRUE(king.valid(Position(4, 4)));
}

TEST_F(KingTest, Valid_NonAdjacentMove_F) {
	King king = King(*white, Position(3, 3));
	EXPECT_FALSE(king.valid(Position(5, 3)));
	EXPECT_FALSE(king.valid(Position(1, 1)));
}

// Castling Tests (King Side + Queen Side)
TEST_F(KingTest, Move_KingsideCastle_T) {
	King king = King(*white, Position(7, 4));
	Rook rook = Rook(*white, Position(7, 7));
	EXPECT_CALL(*white, piece(Position(7, 7)))
		.WillRepeatedly(testing::Return(&rook));
	king.move(Position(7, 6));
	EXPECT_EQ(Position(7, 5), rook.loc());
}

TEST_F(KingTest, Move_QueensideCastle_T) {
	King king = King(*white, Position(7, 4));
	Rook rook = Rook(*white, Position(7, 0));
	EXPECT_CALL(*white, piece(Position(7, 0)))
		.WillRepeatedly(testing::Return(&rook));
	king.move(Position(7, 2));
	EXPECT_EQ(Position(7, 3), rook.loc());
}

TEST_F(KingTest, Valid_ValidCastle_T) {
	King king  = King(*white, Position(7, 4));
	Rook krook = Rook(*white, Position(7, 7));
	Rook qrook = Rook(*white, Position(7, 0));	
	EXPECT_CALL(*white, piece(Position(7, 7)))
		.WillRepeatedly(testing::Return(&krook));
	EXPECT_CALL(*white, piece(Position(7, 0)))
		.WillRepeatedly(testing::Return(&qrook));
	EXPECT_TRUE(king.valid(Position(7, 6)));
	EXPECT_TRUE(king.valid(Position(7, 2)));
}

TEST_F(KingTest, Valid_CastleAfterKingMove_F) {
	King king = King(*white, Position(7, 5));
	Rook rook = Rook(*white, Position(7, 7));
	king.move(Position(7, 4));
	EXPECT_CALL(*white, piece(Position(7, 7)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_FALSE(king.valid(Position(7, 6)));
}

TEST_F(KingTest, Valid_CastleAfterRookMove_F) {
	King king = King(*white, Position(7, 4));
	Rook rook = Rook(*white, Position(7, 6));
	rook.move(Position(7, 7));
	EXPECT_CALL(*white, piece(Position(7, 7)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_FALSE(king.valid(Position(7, 6)));
}

TEST_F(KingTest, Valid_CastleAllyObstructed_F) {
	King king = King(*white, Position(7, 4));
	Rook rook = Rook(*white, Position(7, 0));
	EXPECT_CALL(*white, piece(Position(7, 0)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_CALL(*white, piece(Position(7, 1)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_FALSE(king.valid(Position(7, 2)));
}

TEST_F(KingTest, Valid_CastleEnemyObstructed_F) {
	King king = King(*white, Position(7, 4));
	Rook rook = Rook(*white, Position(7, 7));
	EXPECT_CALL(*white, piece(Position(7, 7)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_CALL(*black, piece(Position(7, 5)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_FALSE(king.valid(Position(7, 6)));
}

TEST_F(KingTest, Valid_CastleOutOfCheck_F) {
	King king = King(*white, Position(7, 4));
	Rook rook = Rook(*white, Position(7, 8));
	EXPECT_CALL(*white, piece(Position(7, 7)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_CALL(*white, in_check(testing::_))
		.WillOnce(testing::Return(true));
	EXPECT_FALSE(king.valid(Position(7, 6)));
}

TEST_F(KingTest, Valid_CastleThroughCheck_F) {
	King king = King(*white, Position(7, 4));
	Rook rook = Rook(*white, Position(7, 8));
	EXPECT_CALL(*white, piece(Position(7, 7)))
		.WillRepeatedly(testing::Return(&rook));
	EXPECT_CALL(*white, in_check(testing::_))
		.WillOnce(testing::Return(false))
		.WillOnce(testing::Return(true));
	EXPECT_FALSE(king.valid(Position(7, 6)));
	
}
*/
} // namespace chess
