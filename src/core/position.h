#ifndef CORE_POSITION_H
#define CORE_POSITION_H

#include <cstdlib>
#include <string>

namespace chess {

/*! Represents a chess position.
 * Wraps a chess position into a 2 integer pair in which the x
 * coordinate represents the row and the y coordinate represents
 * the column in a 2D array.
 */
struct Position {
	int x, y;

	/*! By default set x and y to 0 */
	Position() : Position(0, 0) {}

	/*! Specify a position in x (row), y (col) coordinates. */
	Position(int x, int y) : x(x), y(y) {}

	/*! Specify a position in algebraic chess notation (e.g. e4) */
	Position(char file, int rank) : x(8-rank), y(file-'a') {}

	/*! Specify a position in algebraic chess notation as string */
	Position(std::string pos) : Position(pos.at(0), pos.at(1)-'0') {}

	/*! Create a position from an existing position */
	Position(const Position& pos) : x(pos.x), y(pos.y) {}

	inline int rank() const {
		return 8 - x;
	}

	inline char file() const {
		return 'a' + y;
	}

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

	/*! Overload the scalar multiplication operator. */
	inline Position& operator*=(const double rhs) {
		x *= rhs;
		y *= rhs;
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

/* Why define certain operator overloads as non-member functions?
 * Because the compiler interprets member operator overloads as
 * member functions (s + t -> s.operator+(t)). Defining operators
 * as non-member functions solves this ordering problem. Note, if the
 * operator needs access to private variables, then it must be declared
 * as a friend function.
 */
inline Position operator+(Position lhs, const Position& rhs) {
	return lhs += rhs;
}

inline Position operator-(Position lhs, const Position& rhs) {
	return lhs -= rhs;
}

inline Position operator*(Position lhs, const double rhs) {
	return lhs *= rhs;
}

inline Position operator*(const double rhs, Position lhs) {
	return lhs *= rhs;
}

} // namespace chess

#endif // CORE_POSITION_H
