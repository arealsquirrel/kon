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

/*
 * multiplies two matricies together
 */
template<class M>
M matrix_multiply(const M &a, const M &b);

// template<u8 xSize, u8 ySize, typename Type>
Vector<4,float> matrix_multiply_vec(const Matrix<4,4,float> &a, const Vector<4, float> &b);

/*
 * returns the identity matrix of matrix type M
 */
template<class M>
M matrix_identity();

/*
 * calcalates the gausian norm of a matrix
 */
template<class M>
float matrix_norm(const M &matrix);



// ----------- TO BE IMPLEMENTED ----------- //


/*
 * solves a system of equations using the 
 * gausian method.
 *
 * this method is very cheap on the CPU, but has HORRIBLE ERROR
 */
template<u8 xSize, u8 ySize, typename Type>
inline Vector<xSize, Type> matrix_solve_gausian(
		const Matrix<xSize, ySize, Type> mat,
		const Vector<xSize, Type> vector);

/*
 * solves a system of equations using the 
 * gausian JORDAN method.
 *
 * this method is a little more expensive, but it is very precise
 */
template<u8 xSize, u8 ySize, typename Type>
inline Vector<xSize, Type> matrix_solve_gauss_jordan(
		const Matrix<xSize, ySize, Type> &mat,
		const Vector<xSize, Type> &vector);

/*
 * calcalates the determinant of a matrix
 */
template<class M>
float matrix_determinant(const M &matrix);

/*
 * Calculates the inverse of a matrix
 *
 * returned-matrix*matrix = Identity;
 */
template<class M>
M matrix_inverse(const M &matrix);


}

#endif
