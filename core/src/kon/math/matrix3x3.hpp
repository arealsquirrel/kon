#ifndef KN_MATRIX3X3_HPP
#define KN_MATRIX3X3_HPP

#include "kon/math/vector.hpp"
#include "kon/math/vector3.hpp"
#include <kon/core/core.hpp>
#include <kon/math/matrix.hpp>

namespace kon {

template<typename Type>
class Matrix<3, 3, Type> {
public:
	static constexpr u8 size = 3;
	using VectorType = Vector<size,Type>;
	using VArray = Type[size];

public:
	/*
	 * default intilizer makes an empty matrix
	 */
	Matrix() = default;

	Matrix(Type row1[3], Type row2[3], Type row3[3]) {
		mat[0] = row1;
		mat[1] = row2;
		mat[2] = row3;
	}

	Matrix(VectorType row1, VectorType row2, VectorType row3) {
		mat[0][0] = row1.vec[0];
		mat[0][1] = row1.vec[1];
		mat[0][2] = row1.vec[2];

		mat[1][0] = row2.vec[0];
		mat[1][1] = row2.vec[1];
		mat[1][2] = row2.vec[2];

		mat[2][0] = row3.vec[0];
		mat[2][1] = row3.vec[1];
		mat[2][2] = row3.vec[2];
	}

	Matrix(Type matrix[3][3]) 
			: mat(matrix) {}

public:
	Type mat[3][3];
};

using Matrix3x3 = Matrix<3, 3, float>;
using Matrix3x3Int = Matrix<3, 3, int>;

}

#endif

