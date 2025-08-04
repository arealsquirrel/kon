
#include "vector.hpp"
#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"
#include <cmath>
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

/* ---------- VECTOR NORM ---------- */
template<>
float vector_norm(Vector2 vector) {
	return std::sqrt(vector.x*vector.x + vector.y*vector.y);
}

template<>
float vector_norm(Vector3 vector) {
	return std::sqrt(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z);
}

template<>
float vector_norm(Vector4 vector) {
	return std::sqrt(vector.x*vector.x + vector.y*vector.y +
			 		 vector.z*vector.z + vector.w*vector.w);
}

/* ---------- VECTOR ADD ---------- */
template<>
Vector2 vector_add(Vector2 va, Vector2 vb) {
	return Vector2{va.x+vb.x, va.y+vb.y};
}

template<>
Vector3 vector_add(Vector3 va, Vector3 vb) {
	return Vector3{va.x+vb.x, va.y+vb.y, va.z+vb.z};
}

template<>
Vector4 vector_add(Vector4 va, Vector4 vb) {
	return Vector4{va.x+vb.x, va.y+vb.y, va.z+vb.z, va.w+vb.w};
}

/* ---------- VECTOR DOT ---------- */
template<>
float vector_dot(Vector2 va, Vector2 vb) {
	return va.x*vb.x + va.y*vb.y;
}

template<>
float vector_dot(Vector3 va, Vector3 vb) {
	return va.x*vb.x + va.y*vb.y + va.z*vb.z;
}

template<>
float vector_dot(Vector4 va, Vector4 vb) {
	return va.x*vb.x + va.y*vb.y + va.z*vb.z + va.w*vb.w;
}

/* ---------- VECTOR CROSS ---------- */
Vector<3,float> vector_cross(Vector<3,float> va, Vector<3,float> vb) {
	return {va.y*vb.z - va.z*vb.y,
			va.z*vb.x - va.x*va.z,
			va.x*vb.y - va.y*vb.z};
}

}


