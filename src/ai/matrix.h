#ifndef AI_MATRIX_H
#define AI_MATRIX_H

namespace chess {

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

	int rank() const;
	double det() const;
	double trace() const;

	Matrix* inverse();
	Matrix* transpose();

	Matrix operator+=(const Matrix& rhs);
	Matrix operator-=(const Matrix& rhs);
	Matrix operator*=(const Matrix& rhs);
	Matrix operator*=(const double& scalar);

	bool operator==(const Matrix& rhs) const;
	bool operator!=(const Matrix& rhs) const;
};

Matrix operator+(Matrix lhs, const Matrix& rhs);
Matrix operator-(Matrix lhs, const Matrix& rhs);
Matrix operator*(Matrix lhs, const Matrix& rhs);
Matrix operator*(const double& lhs, Matrix rhs);
Matrix operator*(Matrix lhs, const double& rhs);

} // namespace chess

#endif
