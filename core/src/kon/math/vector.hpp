
#ifndef KN_VECTOR_HPP
#define KN_VECTOR_HPP

#include <kon/core/core.hpp>

namespace kon {

struct Vector2 {
	float x;
	float y;
};

struct Vector2Int {
	int x;
	int y;
};

struct Vector3 {
	float x;
	float y;
	float z;
};

static bool operator ==(const Vector2 &a, const Vector2 &b) {
		return (a.x == b.x && a.y && b.y);
}

static bool operator ==(const Vector3 &a, const Vector3 &b) {
	return (a.x == b.x && b.y && b.y && a.z && b.z);
}

struct Vector4 {
	float x;
	float y;
	float z;
	float w;
};

u32 hash_vector(int x, int y);
u32 hash_vector(int x, int y, int z);

}

#endif

