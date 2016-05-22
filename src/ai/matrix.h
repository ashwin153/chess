#ifndef AI_MATRIX_H
#define AI_MATRIX_H

namespace ai {

/*! Simple matrix class.
 * Performs simple calculations involving matricies. Supports
 * basic matrix operations. Design of this class is intended
 * as both a review of linear algebra and an introduction into
 * efficient computation of matrix properties.
 */
class Matrix {
private:

public:
	Matrix(int nrow, int ncol);
	~Matrix();

	int rank();
	double det();
	double trace();

	Matrix* inverse();
	Matrix* transpose();

/*
	Matrix operator+(const Matrix& rhs);
	Matrix operator-(const Matrix& rhs);
	Matrix operator*(Matrix lhs, const double& scalar);
	Matrix operator*(const double& scalar, Matrix rhs);
	Matrix operator*(Matrix lhs, Matrix rhs);
	Matrix operator+=(const Matrix& rhs);
	Matrix operator-=(const Matrix& rhs);
	Matrix operator*=(const Matrix& rhs);
	Matrix operator*=(const double& scalar);
*/
};

}

#endif
