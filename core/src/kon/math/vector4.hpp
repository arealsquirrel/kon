#ifndef KN_VECTOR4_HPP
#define KN_VECTOR4_HPP

#include <kon/math/vector.hpp>

namespace kon {

template<typename Type>
struct Vector<4, Type> {
public:
	union {
		Type vec[4] = {0,0,0,0};

		struct {
			Type x;
			Type y;
			Type z;
			Type w;
		};
	};

	bool operator==(const Vector<4, Type> &vec) const {
		return (x == vec.x && y == vec.y && z == vec.z && w == vec.w);
	}
};

using Vector4 = Vector<4, float>;
using Vector4Int = Vector<4, int>;

}

#endif
