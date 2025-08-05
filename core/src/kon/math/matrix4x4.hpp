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
	Type mat[4][4];
};

using Matrix4x4 = Matrix<4, 4, float>;
using Matrix4x4Int = Matrix<4, 4, int>;

}

#endif
