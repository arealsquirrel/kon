#ifndef KN_TRANSFORMATIONS_HPP
#define KN_TRANSFORMATIONS_HPP

#include "kon/math/matrix4x4.hpp"
#include "kon/math/vector3.hpp"
#include <kon/core/core.hpp>

namespace kon {

/* 
 * returns a matrix that can be multiplied with a
 * vector to yield a translation
 */
Matrix4x4 trfm_translation(Vector3 delta);

/*
 * returns a matrix that can be multiplied with a vector
 * to yield a scaling transformation
 */
Matrix4x4 trfm_scale(Vector3 delta);

/*
 * all of these functions give a matrix
 * that defines a transformation that can rotate
 * a vector around a given axis
 */
Matrix4x4 trfm_rotate_x(float radians);
Matrix4x4 trfm_rotate_y(float radians);
Matrix4x4 trfm_rotate_z(float radians);
Matrix4x4 trfm_rotate_axis(Vector3 axis, float radians);

/*
 * These yield functions that define a transformation that can 
 * put your vectors into perspectives.
 *
 * they squese your points into a space with coordnate system x[left, right] and y[top, bottom]
 */
Matrix4x4 trfm_orthographic(float near, float far, float left, float right, float top, float bottom);
Matrix4x4 trfm_perspective(float near, float far, float left, float right, float top, float bottom);

}

#endif
