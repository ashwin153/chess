#include "src/core/player.h"
#include "src/core/piece.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

namespace chess {

TEST(PlayerTest, piece_DefaultWhitePlayer_NotNull) {
	Player player(true);
	for (int x = 6; x <= 7; x++)
		for (int y = 0; y <= 7; y++)
			EXPECT_TRUE(player.piece(Position(x, y)));
}

}
