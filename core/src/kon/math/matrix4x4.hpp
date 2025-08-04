#ifndef KN_MATRIX4X4_HPP
#define KN_MATRIX4X4_HPP

#include "kon/math/vector.hpp"
#include <cstring>
#include <kon/core/core.hpp>
#include <kon/math/matrix.hpp>
#include <kon/math/vector4.hpp>

namespace kon {

template<typename Type>
class Matrix<4, 4, Type> {
public:
	static constexpr u8 size = 4;
	using VectorType = Vector<size,Type>;
	using VArray = Type[size];

public:
	/*
	 * default intilizer makes an empty matrix
	 */
	Matrix() {
		memset(&mat[0][0], 0, 16 * sizeof(Type));
	}

	Matrix(VectorType row1, VectorType row2, VectorType row3, VectorType row4) {
		memset(&mat[0][0], 0, 16 * sizeof(Type));
		
		mat[0][0] = row1.vec[0];
		mat[0][1] = row1.vec[1];
		mat[0][2] = row1.vec[2];
		mat[0][3] = row1.vec[3];

		mat[1][0] = row2.vec[0];
		mat[1][1] = row2.vec[1];
		mat[1][2] = row2.vec[2];
		mat[1][3] = row2.vec[3];

		mat[2][0] = row3.vec[0];
		mat[2][1] = row3.vec[1];
		mat[2][2] = row3.vec[2];
		mat[2][3] = row3.vec[3];

		mat[3][0] = row4.vec[0];
		mat[3][1] = row4.vec[1];
		mat[3][2] = row4.vec[2];
		mat[3][3] = row4.vec[3];
	}

	Matrix(Type matrix[4][4]) 
			: mat(matrix) {}

public:
	Type mat[4][4];
};

using Matrix4x4 = Matrix<4, 4, float>;
using Matrix4x4Int = Matrix<4, 4, int>;

}

#endif
