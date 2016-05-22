#include "src/core/position.h"
#include "gtest/gtest.h"

namespace chess {

class PositionTest : public testing::Test {
protected:
	Position* origin;
	Position* a;
	Position* b;

public:
	virtual void SetUp() {
		origin = new Position(0, 0);
		a = new Position(3, 2);
		b = new Position(-1, 4);
	}

	virtual void TearDown() {
		delete origin;
		delete a;
		delete b;
		origin = nullptr;
		a = nullptr;
		b = nullptr;
	}
};

TEST_F(PositionTest, AlgebraicNotation) {
	EXPECT_EQ(Position(0, 0), Position('a', 8));
	EXPECT_EQ(Position(4, 4), Position('e', 4));
	EXPECT_EQ(Position(7, 7), Position('h', 1));
}

TEST_F(PositionTest, Dist) {
	EXPECT_EQ(0, origin->dist(*origin));
	EXPECT_EQ(1, origin->dist(Position(0, 1)));
	EXPECT_EQ(2, origin->dist(Position(1, 1)));
	EXPECT_EQ(1, origin->dist(Position(1, 0)));
	EXPECT_EQ(2, origin->dist(Position(1, -1)));
	EXPECT_EQ(1, origin->dist(Position(0, -1)));
	EXPECT_EQ(2, origin->dist(Position(-1, -1)));
	EXPECT_EQ(1, origin->dist(Position(-1, 0)));
	EXPECT_EQ(2, origin->dist(Position(-1, 1)));
}

TEST_F(PositionTest, AdditionOperator) {
	EXPECT_EQ(a->x + b->x, (*a + *b).x);
	EXPECT_EQ(a->y + b->y, (*a + *b).y);
}

TEST_F(PositionTest, SubtractionOperator) {
	EXPECT_EQ(a->x - b->x, (*a - *b).x);
	EXPECT_EQ(a->y - b->y, (*a - *b).y);
}

TEST_F(PositionTest, EqualOperator) {
	EXPECT_TRUE(*a == *a);
	EXPECT_FALSE(*a == *b);
}

TEST_F(PositionTest, NotEqualOperator) {
	EXPECT_TRUE(*a != *b);
	EXPECT_FALSE(*a != *a);
}

TEST_F(PositionTest, PlusEqualOperator) {
	int x = a->x + b->x;
	int y = a->y + b->y;
	*a += *b;
	
	EXPECT_EQ(x, a->x);
	EXPECT_EQ(y, a->y);
}

TEST_F(PositionTest, MinusEqualOperator) {
	int x = a->x - b->x;
	int y = a->y - b->y;
	*a -= *b;

	EXPECT_EQ(x, a->x);
	EXPECT_EQ(y, a->y);
}

}
