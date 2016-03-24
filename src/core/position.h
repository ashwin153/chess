#ifndef CORE_POSITION_H
#define CORE_POSITION_H

#include <cstdlib>

namespace chess {

/*! Represents a chess position.
 * Wraps a chess position into a 2 integer pair in which the x
 * coordinate represents the row and the y coordinate represents
 * the column in a 2D array.
 */
struct Position {
	int x, y;

	/*! Specify a position in x (row), y (col) coordinates. */
	Position(int x, int y) : x(x), y(y) {}

	/*! Specify a position in algebraic chess notation (e.g. e4) */
	Position(char file, int rank) : x(rank-1), y(file-'a') {}

	/*! Create a position from an existing position */
	Position(const Position& pos) : x(pos.x), y(pos.y) {}
	
	/*! Manhattan distance between positions. */
	inline int dist(const Position& pos) const {
		return std::abs(pos.x - x) + std::abs(pos.y - y);
	}

	/*! Overload the equal operator. */
	inline Position& operator=(const Position& rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	/*! Overload the addition operator. */
	inline Position& operator+=(const Position& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	/*! Overload the subtraction operator. */
	inline Position& operator-=(const Position& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;	
	}

	/*! Overload the equals operator. */
	inline bool operator==(const Position& pos) const {
		return x == pos.x && y == pos.y;
	}

	/*! Overload the not equals operator. */
	inline bool operator!=(const Position& pos) const { 
		return !(*this == pos);
	}
};

inline Position operator+(Position lhs, const Position& rhs) {
	return lhs += rhs;
}

inline Position operator-(Position lhs, const Position& rhs) {
	return lhs -= rhs;
}

}

#endif
