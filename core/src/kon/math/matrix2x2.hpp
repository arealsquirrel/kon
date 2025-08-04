#ifndef KN_MATRIX2X2_HPP
#define KN_MATRIX2X2_HPP

#include "kon/math/vector2.hpp"
#include <kon/core/core.hpp>
#include <kon/math/matrix.hpp>

namespace kon {

template<typename Type>
class Matrix<2, 2, Type> {
public:
	static constexpr u8 size = 2;
	using VectorType = Vector<size,Type>;
	using VArray = Type[size];

public:
	/*
	 * default intilizer makes an empty matrix
	 */
	Matrix() = default;


	Matrix(VectorType row1, VectorType row2) {
		mat[0][0] = row1.vec[0];
		mat[0][1] = row1.vec[1];

		mat[1][0] = row2.vec[0];
		mat[1][1] = row2.vec[1];
	}

	Matrix(Type matrix[2][2]) 
			: mat(matrix) {}
public:
	Type mat[2][2];
};

using Matrix2x2 = Matrix<2, 2, float>;
using Matrix2x2Int = Matrix<2, 2, int>;

}

#endif

