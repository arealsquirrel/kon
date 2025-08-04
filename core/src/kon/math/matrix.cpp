
#include "matrix.hpp"
#include "kon/math/matrix2x2.hpp"
#include "kon/math/matrix3x3.hpp"
#include "kon/math/matrix4x4.hpp"
#include "kon/math/vector.hpp"
#include "kon/math/vector4.hpp"
#include <cmath>

namespace kon {

template<>
Matrix2x2 matrix_multiply(const Matrix2x2 &a, const Matrix2x2 &b) {
	Matrix2x2 result;
	result.mat[0][0] = a.mat[0][0]*b.mat[0][0] + a.mat[0][1]*b.mat[1][0];
	result.mat[0][1] = a.mat[0][0]*b.mat[0][1] + a.mat[0][1]*b.mat[1][1];

	result.mat[1][0] = a.mat[1][0]*b.mat[0][0] + a.mat[1][1]*b.mat[1][0];
	result.mat[1][1] = a.mat[1][0]*b.mat[0][1] + a.mat[1][1]*b.mat[1][1];
	return result;
}

template<>
Matrix3x3 matrix_multiply(const Matrix3x3 &a, const Matrix3x3 &b) {
	Matrix3x3 result;
	result.mat[0][0] = a.mat[0][0]*b.mat[0][0] + a.mat[0][1]*b.mat[1][0] + a.mat[0][2]*b.mat[2][0];
	result.mat[0][1] = a.mat[0][0]*b.mat[0][1] + a.mat[0][1]*b.mat[1][1] + a.mat[0][2]*b.mat[2][1];
	result.mat[0][2] = a.mat[0][0]*b.mat[0][2] + a.mat[0][1]*b.mat[1][2] + a.mat[0][2]*b.mat[2][2];

	result.mat[1][0] = a.mat[1][0]*b.mat[0][0] + a.mat[1][1]*b.mat[1][0] + a.mat[1][2]*b.mat[2][0];
	result.mat[1][1] = a.mat[1][0]*b.mat[0][1] + a.mat[1][1]*b.mat[1][1] + a.mat[1][2]*b.mat[2][1];
	result.mat[1][2] = a.mat[1][0]*b.mat[0][2] + a.mat[1][1]*b.mat[1][2] + a.mat[1][2]*b.mat[2][2];

	result.mat[2][0] = a.mat[2][0]*b.mat[0][0] + a.mat[2][1]*b.mat[1][0] + a.mat[2][2]*b.mat[2][0];
	result.mat[2][1] = a.mat[2][0]*b.mat[0][1] + a.mat[2][1]*b.mat[1][1] + a.mat[2][2]*b.mat[2][1];
	result.mat[2][2] = a.mat[2][0]*b.mat[0][2] + a.mat[2][1]*b.mat[1][2] + a.mat[2][2]*b.mat[2][2];
	return result;
}

template<>
Matrix4x4 matrix_multiply(const Matrix4x4 &a, const Matrix4x4 &b) {
	Matrix4x4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.mat[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				result.mat[i][j] += a.mat[i][k] * b.mat[k][j];
			}
		}
	}

	return result;
}

Vector<4,float> matrix_multiply_vec(const Matrix<4,4,float> &a, const Vector<4, float> &b) {
	Vector<4, float> result;

    for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
            result.vec[i] += a.mat[i][j] * b.vec[j];
        }
    }

	return result;
}

// ---------------- IDENTITY ---------------- //
template<>
Matrix2x2 matrix_identity() {
	return {{1,0}, {0,1}};
}

template<>
Matrix3x3 matrix_identity() {
	return {{1,0,0}, {0,1,0}, {0,0,1}};
}

template<>
Matrix4x4 matrix_identity() {
	return {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}};
}

// ---------------- NORM ---------------- //
template<>
float matrix_norm(const Matrix2x2 &matrix) {
	float sq = 0;
	for (int i = 0; i < 2; i++)
		for (int j = 0; j < 2; j++)
			sq += matrix.mat[i][j]*matrix.mat[i][j];

	return std::sqrt(sq);
}

template<>
float matrix_norm(const Matrix3x3 &matrix) {
	float sq = 0;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			sq += matrix.mat[i][j]*matrix.mat[i][j];

	return std::sqrt(sq);
}

template<>
float matrix_norm(const Matrix4x4 &matrix) {
	float sq = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			sq += matrix.mat[i][j]*matrix.mat[i][j];

	return std::sqrt(sq);
}

}

