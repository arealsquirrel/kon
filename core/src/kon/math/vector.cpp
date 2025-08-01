
#include "vector.hpp"
#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"
#include <functional>

namespace kon {

u32 hash_vector(int x, int y, int z) {
	u32 seed = 3;

    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    y = ((y >> 16) ^ y) * 0x45d9f3b;
    y = ((y >> 16) ^ y) * 0x45d9f3b;
    y = (y >> 16) ^ y;
    seed ^= y + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    z = ((z >> 16) ^ z) * 0x45d9f3b;
    z = ((z >> 16) ^ z) * 0x45d9f3b;
    z = (z >> 16) ^ z;
    seed ^= z + 0x9e3779b9 + (seed << 6) + (seed >> 2);

	return seed;
}

template<>
u64 hash_vector(Vector2 v) {
	u32 seed = 2;

	for(int i = 0; i < 2; i++) {
		u64 x = std::hash<float>{}(v.vec[i]);
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = (x >> 16) ^ x;
		seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	return seed;
}

template<>
u64 hash_vector(Vector2Int v) {
	u32 seed = 2;

	for(int i = 0; i < 2; i++) {
		u64 x = v.vec[i];
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = (x >> 16) ^ x;
		seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	return seed;
}

template<>
u64 hash_vector(Vector3 v) {
	u32 seed = 3;

	for(int i = 0; i < 3; i++) {
		u64 x = std::hash<float>{}(v.vec[i]);
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = (x >> 16) ^ x;
		seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	return seed;
}

template<>
u64 hash_vector(Vector3Int v) {
	u32 seed = 3;

	for(int i = 0; i < 3; i++) {
		u64 x = v.vec[i];
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = (x >> 16) ^ x;
		seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	return seed;
}

template<>
u64 hash_vector(Vector4 v) {
	u32 seed = 4;

	for(int i = 0; i < 4; i++) {
		u64 x = std::hash<float>{}(v.vec[i]);
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = (x >> 16) ^ x;
		seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	return seed;
}

template<>
u64 hash_vector(Vector4Int v) {
	u32 seed = 4;

	for(int i = 0; i < 4; i++) {
		u64 x = v.vec[i];
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = ((x >> 16) ^ x) * 0x45d9f3b;
		x = (x >> 16) ^ x;
		seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	return seed;
}

}


