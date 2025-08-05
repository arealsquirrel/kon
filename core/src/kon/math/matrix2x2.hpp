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
	Type mat[2][2];
};

using Matrix2x2 = Matrix<2, 2, float>;
using Matrix2x2Int = Matrix<2, 2, int>;

}

#endif

