#ifndef KN_VECTOR3_HPP
#define KN_VECTOR3_HPP

#include <kon/math/vector.hpp>

namespace kon {

template<typename Type>
struct Vector<3, Type> {
public:
	union {
		Type vec[3];

		struct {
			Type x;
			Type y;
			Type z;
		};
	};

	bool operator==(const Vector<3, Type> &vec) const {
		return (x == vec.x && y == vec.y && z == vec.z);
	}
};

using Vector3 = Vector<3, float>;
using Vector3Int = Vector<3, int>;

}

#endif
