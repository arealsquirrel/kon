
#include "kon/math/matrix.hpp"
#include "kon/math/matrix2x2.hpp"
#include "kon/math/matrix3x3.hpp"
#include "kon/math/matrix4x4.hpp"
#include "kon/math/vector.hpp"
#include "kon/math/vector2.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <kon/core/core.hpp>

using namespace kon;

// Demonstrate some basic assertions.
TEST(Math, Vector) {
	Vector2 v{2,1};

	EXPECT_EQ(v.x, 2);
}

TEST(Math, Matrix2x2) {
	Vector2 v{2,1};
	Matrix2x2Int mat({1,2}, {3,4});

	EXPECT_EQ(mat.mat[1][0], 3);

	mat = Matrix2x2Int::identity();
	EXPECT_EQ(mat.mat[0][0], 1);
	EXPECT_EQ(mat.mat[1][1], 1);
	EXPECT_EQ(mat.mat[0][1], 0);

	Matrix2x2 result = Matrix2x2({1,3}, {3,2}) * Matrix2x2({4,5}, {6,5});
	EXPECT_EQ(result.mat[1][1], 25);
}

TEST(Math, Matrix3x3) {
	Matrix3x3 mat({1,2,0}, {}, {1,4,2});

	EXPECT_EQ(mat.mat[2][1], 4);
	mat = Matrix3x3::identity();
	
	EXPECT_EQ(mat.mat[2][2], 1);
	EXPECT_EQ(mat.mat[2][0], 0);

	Matrix3x3 result = Matrix3x3({1,2,3}, {3,2,1}, {1,2,3}) * Matrix3x3({4,5,6}, {6,5,4}, {4,6,5});
	EXPECT_EQ(result.mat[1][1], 31);
}

TEST(Math, Matrix4x4) {
	Matrix4x4 mat({1,2,0,2}, {}, {1,4,2,1}, {9,2,1,0});

	EXPECT_EQ(mat.mat[2][1], 4);
	mat = Matrix4x4::identity();
	
	EXPECT_EQ(mat.mat[2][2], 1);
	EXPECT_EQ(mat.mat[2][0], 0);


	Matrix4x4Int result = Matrix4x4Int({1,2,3,4}, {4,3,2,1}, {1,2,3,4}, {7,6,5,4}) *
					   Matrix4x4Int({4,5,6,7}, {7,6,5,4}, {4,6,5,7}, {4,3,2,1});

	EXPECT_EQ(result.mat[0][3], 40);
	Matrix4x4 a = Matrix4x4({1,2,3,4}, {4,3,2,1}, {1,2,3,4}, {7,6,5,4});
	Matrix4x4 res = matrix_scale(a, {2,2,2,1});
	EXPECT_EQ(res.mat[0][3], 8);

	res = matrix_perspective(10, 5, 0.01, 100);
	log_matrix(res);
}
