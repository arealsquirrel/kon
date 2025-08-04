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
	Type mat[3][3];
};

using Matrix3x3 = Matrix<3, 3, float>;
using Matrix3x3Int = Matrix<3, 3, int>;

}

#endif

