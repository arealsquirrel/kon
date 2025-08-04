
#include "transformations.hpp"
#include "kon/math/aproximations.hpp"
#include "kon/math/matrix4x4.hpp"
#include "kon/math/vector4.hpp"

namespace kon {

Matrix4x4 trfm_translation(Vector3 delta) {
	return Matrix4x4({1.0f,0,0,delta.x},
					 {0,1.0f,0,delta.y},
					 {0,0,1.0f,delta.z},
					 {0,0,0,1.0f});
}

Matrix4x4 trfm_scale(Vector3 delta) {
	return Matrix4x4({delta.x, 0, 0, 0}, 
					 {0, delta.y, 0, 0},
					 {0, 0, delta.z, 0},
					 {0, 0, 0, 1,});
}

Matrix4x4 trfm_rotate_x(float radians) {
	return Matrix4x4({1,0,0,0},
					 {0, kn_cos(radians), kn_sin(radians), 0},
					 {0, -kn_sin(radians), kn_cos(radians), 0},
					 {0,0,0,1});
}

Matrix4x4 trfm_rotate_y(float radians) {
	return Matrix4x4({kn_cos(radians), 0, -kn_sin(radians), 0},
					 {0, 1, 0, 0},
					 {kn_sin(radians), 0, kn_cos(radians), 0},
					 {0,0,0,1});
}

Matrix4x4 trfm_rotate_z(float radians) {
	return Matrix4x4({kn_cos(radians), kn_sin(radians), 0, 0},
					 {-kn_sin(radians), kn_cos(radians), 0, 0},
					 {0,0,1,0},
					 {0,0,0,1});
}

Matrix4x4 trfm_orthographic(float near, float far, float left, float right, float top, float bottom) {
	return Matrix4x4({2/(left-right), 0, 0, 0},
					 {0, 2/(top-bottom), 0, 0},
					 {0, 0, 2/(far-near), 0},
					 {-(right+left)/(right-left), -(top+bottom)/(top-bottom),-(far+near)/(far-near),1});
}

Matrix4x4 trfm_perspective(float near, float far, float left, float right, float top, float bottom) {
	return Matrix4x4();
}

}

