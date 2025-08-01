#ifndef KN_MATRIX_HPP
#define KN_MATRIX_HPP

#include "kon/debug/log.hpp"
#include "kon/math/vector.hpp"
#include <iostream>
#include <kon/core/core.hpp>

namespace kon {

/*
 * impl definition of matrix not created
 * :(
 *
 * MATRICIES ARE STORED ROW MAJOR
 */
template<u8 xSize, u8 ySize, typename Type>
class Matrix {
};

template<u8 xSize, u8 ySize, typename Type>
void log_matrix(const Matrix<xSize, ySize, Type> &matrix) {
	for(int x = 0; x < xSize; x++) {
		for(int y = 0; y < ySize; y++) {
			std::cout << matrix.mat[x][y] << ", ";
		}
		std::cout << std::endl;
	}
}

template<u8 xSize, u8 ySize, typename Type>
Matrix<xSize, ySize, Type> matrix_scale(
		const Matrix<xSize, ySize, Type> mat,
		const Vector<xSize, Type> vector);

Matrix<4,4,float> matrix_perspective(float right, float top, float near, float far);

}

#endif
