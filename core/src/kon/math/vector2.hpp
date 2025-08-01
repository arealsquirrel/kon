#ifndef KN_VECTOR2_HPP
#define KN_VECTOR2_HPP

#include <kon/math/vector.hpp>

namespace kon {

template<typename Type>
struct Vector<2, Type> {
public:
	union {
		Type vec[2];

		struct {
			Type x;
			Type y;
		};
	};

	bool operator==(const Vector<2, Type> &vec) const {
		return (x == vec.x && y == vec.y);
	}
};

using Vector2 = Vector<2, float>;
using Vector2Int = Vector<2, int>;

}

#endif
