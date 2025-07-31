
#include "vector.hpp"

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


u32 hash_vector(int x, int y) {
	u32 seed = 2;

    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);

    y = ((y >> 16) ^ y) * 0x45d9f3b;
    y = ((y >> 16) ^ y) * 0x45d9f3b;
    y = (y >> 16) ^ y;
    seed ^= y + 0x9e3779b9 + (seed << 6) + (seed >> 2);

	return seed;
}

}


