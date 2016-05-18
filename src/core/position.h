#ifndef CORE_POSITION_H
#define CORE_POSITION_H

#include <cstdlib>
#include <string>
#include <tuple>

namespace chess {

/*!
 * This struct wraps a chess position into a 2 integer pair in which the x
 * coordinate represents the row and the y coordinate represents the column in 
 * a 2D array. This struct also defines the various conversions between chess
 * positions (rank and file) to the integer values (x, y).
 */
struct Position {
	int x, y;

	/*! 
	 * Construct a default position, in which the x and y coordinates are both
	 * set to zero. This corresponds to position a8 on a chess board. 
	 */
	Position() : Position(0, 0) {}

	/*! 
	 * Construct a position with the specified x, y coordinates.
	 * @param[in] x Row coordinate.
	 * @param[in] y Column coordinate.
	 */
	Position(int x, int y) : x(x), y(y) {}

	/*!
	 * Construct a position from algebraic chess notation (e.g. e4). Converts 
	 * the algebraic chess position to the internal x,y coordinate system.
	 * @param[in] file Corresponds to y coordinate.
	 * @param[in] rank Corresponds to x coordinate.
	 */
	Position(char file, int rank) : x(8-rank), y(file-'a') {}

	/*! 
	 * Construct a position from an algebraic chess notation string. Extracts
	 * the character file and integer rank from the string.
	 * @param[in] string Algebraic chess notation string.
	 */
	Position(std::string pos) : Position(pos.at(0), pos.at(1)-'0') {}

	/*! 
	 * Construct a position from the specified template position.
	 * @param[in] pos Position to copy.
	 */
	Position(const Position& pos) : x(pos.x), y(pos.y) {}

	/*!
	 * Returns the rank of the chess position. The rank corresponds to the x
	 * coordinate of the position.
	 * @return Rank of position (1-8).
	 */
	inline int rank() const {
		return 8 - x;
	}

	/*!
	 * Returns the file of the chess position. The file corresponds to the y
	 * coordinate of the position.
	 * @return File of the position (a-h).
	 */
	inline char file() const {
		return 'a' + y;
	}

	/*! 
	 * Returns the manhattan distance between this position and the specified
	 * position. The manhattan distance is the number of tiles between the
	 * positions (e.g. dist 1,2 and 3, 3 is 3)
	 * @return Manhattan distance
	 */
	inline int dist(const Position& pos) const {
		return std::abs(pos.x - x) + std::abs(pos.y - y);
	}

	/*! Overload Equal Operator */
	inline Position& operator=(const Position& rhs) {
		x = rhs.x;
		y = rhs.y;
		return *this;
	}

	/*! Overload Addition Operator */
	inline Position& operator+=(const Position& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	/*! Overload Subtraction Operator */
	inline Position& operator-=(const Position& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;	
	}

	/*! Overload Scalar Multiplication Operator */
	inline Position& operator*=(const double rhs) {
		x *= rhs;
		y *= rhs;
		return *this;
	}

	/*! Overload Equals Operator */
	inline bool operator==(const Position& pos) const {
		return x == pos.x && y == pos.y;
	}

	/*! Overload Less Than Operator */
	inline bool operator<(const Position& pos) const {
		return std::tie(x, y) < std::tie(pos.x, pos.y);
	}

	/*! Overload Less Than Equal To Operator */
	inline bool operator<=(const Position& pos) const {
		return *this < pos || *this == pos;
	}

	/*! Overload Greater Than Equal To Operator */
	inline bool operator>=(const Position& pos) const {
		return !(*this < pos);
	}

	/*! Overload Greater Than Operator */
	inline bool operator>(const Position& pos) const {
		return !(*this <= pos);
	}

	/*! Overload Not Equal Operator */
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
