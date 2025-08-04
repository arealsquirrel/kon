
#include "kon/debug/log.hpp"
#include "kon/math/matrix.hpp"
#include "kon/math/matrix2x2.hpp"
#include "kon/math/matrix3x3.hpp"
#include "kon/math/matrix4x4.hpp"
#include "kon/math/transformations.hpp"
#include "kon/math/vector.hpp"
#include "kon/math/vector2.hpp"
#include "kon/math/vector4.hpp"
#include <cmath>
#include <gtest/gtest.h>
#include <kon/core/core.hpp>

using namespace kon;

// Demonstrate some basic assertions.
TEST(Math, VectorNorm) {
	EXPECT_NEAR(std::sqrt(5), vector_norm(Vector2{1,2}), 0.001);
	EXPECT_NEAR(10.6901, vector_norm(Vector2{3.2,10.2}), 0.001);

	EXPECT_NEAR(10.6301, vector_norm(Vector3{3,2,10}), 0.001);
	EXPECT_NEAR(14.6931, vector_norm(Vector3{4.2,12.1,7.2}), 0.001);

	EXPECT_NEAR(std::sqrt(1105), vector_norm(Vector4{8,4,32,1}), 0.001);
	EXPECT_NEAR(std::sqrt(231.93), vector_norm(Vector4{3.2,10.2,6.2,8.9}), 0.001);
}

TEST(Math, VectorAdd) {
	EXPECT_TRUE(vector_add<Vector2>({1,2}, {2,3}) == Vector2({3,5}));
	EXPECT_TRUE(vector_add<Vector2>({1.8,2.1}, {2.3,3.2}) == Vector2({4.1,5.3}));

	EXPECT_TRUE(vector_add<Vector3>({1,2,8}, {2,3,30}) == Vector3({3,5,38}));
	EXPECT_TRUE(vector_add<Vector3>({1.8,2.1,9.5}, {2.3,3.2,9.3}) == Vector3({4.1,5.3,18.8}));

	EXPECT_TRUE(vector_add<Vector4>({1,2,8,4}, {2,3,30,22}) == Vector4({3,5,38,26}));
	EXPECT_TRUE(vector_add<Vector4>({1.8,2.1,9.5,18.2}, {2.3,3.2,9.3,7.5}) == Vector4({4.1,5.3,18.8,25.7}));
}

TEST(Math, VectorDot) {
}

TEST(Math, VectorCross) {
}

TEST(Math, Matrix2x2) {
	Vector2 v{2,1};
	Matrix2x2 mat({1,2}, {3,4});

	EXPECT_EQ(mat.mat[1][0], 3);

	mat = matrix_identity<Matrix2x2>();
	EXPECT_EQ(mat.mat[0][0], 1);
	EXPECT_EQ(mat.mat[1][1], 1);
	EXPECT_EQ(mat.mat[0][1], 0);

	Matrix2x2 result = matrix_multiply(Matrix2x2({1,3}, {3,2}), Matrix2x2({4,5}, {6,5}));
	EXPECT_EQ(result.mat[1][1], 25);
}

TEST(Math, Matrix3x3) {
	Matrix3x3 mat({1,2,0}, {}, {1,4,2});

	EXPECT_EQ(mat.mat[2][1], 4);
	mat = matrix_identity<Matrix3x3>();
	
	EXPECT_EQ(mat.mat[2][2], 1);
	EXPECT_EQ(mat.mat[2][0], 0);

	Matrix3x3 result = matrix_multiply(Matrix3x3({1,2,3}, {3,2,1}, {1,2,3}),
									   Matrix3x3({4,5,6}, {6,5,4}, {4,6,5}));
	EXPECT_EQ(result.mat[1][1], 31);
}

TEST(Math, Matrix4x4) {
	Matrix4x4 mat({1,2,0,2}, {}, {1,4,2,1}, {9,2,1,0});

	EXPECT_EQ(mat.mat[2][1], 4);
	mat = matrix_identity<Matrix4x4>();
	
	EXPECT_EQ(mat.mat[2][2], 1);
	EXPECT_EQ(mat.mat[2][0], 0);


	Matrix4x4 result = matrix_multiply(Matrix4x4({1,2,3,4}, {4,3,2,1}, {1,2,3,4}, {7,6,5,4}),
					   					  Matrix4x4({4,5,6,7}, {7,6,5,4}, {4,6,5,7}, {4,3,2,1}));

	EXPECT_EQ(result.mat[0][3], 40);
	Matrix4x4 res = trfm_scale({2,2,2});
	// EXPECT_EQ(res.mat[0][3], 8);

	res = trfm_perspective(0, 0, 10, 5, 0.01, 100);


	Vector3 point{1,5,3};
	Vector4 model = matrix_multiply_vec(trfm_translation(Vector3{3,2,2}), {point.x, point.y, point.z, 1});
	model = matrix_multiply_vec(trfm_scale(Vector3{2,1,1}), model);

	KN_TRACE("{} {} {} {}", model.x, model.y, model.z, model.w);
}
