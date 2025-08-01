
#include "matrix.hpp"
#include "kon/math/matrix2x2.hpp"
#include "kon/math/matrix3x3.hpp"
#include "kon/math/matrix4x4.hpp"
#include "kon/math/vector.hpp"
#include "kon/math/vector4.hpp"

namespace kon {

template<>
Matrix4x4 matrix_scale(const Matrix4x4 mat, const Vector4 vector) {
	Matrix4x4 scale({vector.x,0,0,0},
					{0,vector.y,0,0},
					{0,0,vector.z,0},
					{0,0,0,vector.w});

	return mat * scale;
}

template<>
Matrix4x4Int matrix_scale(const Matrix4x4Int mat, const Vector4Int vector) {
	Matrix4x4Int scale({vector.x,0,0,0},
					{0,vector.y,0,0},
					{0,0,vector.z,0},
					{0,0,0,vector.w});

	return mat * scale;
}

Matrix4x4 matrix_perspective(float right, float top, float near, float far) {
	Matrix4x4 mat;

	mat.mat[0][0] = near / right;
	mat.mat[1][1] = near / top;
	mat.mat[2][2] = -far/(far-near);
	mat.mat[2][3] = (-far * near) / (far - near);
	mat.mat[3][2] = -1;

	return mat;
}

}

