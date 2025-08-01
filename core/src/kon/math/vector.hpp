#ifndef KN_VECTOR_HPP
#define KN_VECTOR_HPP

#include "kon/core/assert.hpp"
#include "kon/core/util.hpp"
#include <functional>
#include <kon/core/core.hpp>

namespace kon {

/*
 * template definition of a vector of variable size
 */
template<u8 size, typename Type>
struct Vector {
public:
	constexpr u8 get_size() { return size; }

	Type v[size];

public:
	Type operator[](u8 index) const { 
		KN_ASSERT(index <= size, "larger index than vector size")
		return v[index]; 
	}

	constexpr void for_each(ForEachFunction<Type> function) {
		for(int i = 0; i < size; i++) {
			function(v[i], i);
		}
	}

	u64 hash() const {
		u64 seed = size;

		// for_each([&](Type e, u32) {
		for(int i = 0; i < size; i++) {
			u64 x = std::hash<Type>{}(v[i]);
			x = ((x >> 16) ^ x) * 0x45d9f3b;
    		x = ((x >> 16) ^ x) * 0x45d9f3b;
    		x = (x >> 16) ^ x;
    		seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}

		return seed;
	}

	bool operator ==(const Vector<size, Type> &vec) const {
		for(int i = 0; i < size; i++) {
			if(vec[i] != v[i]) return false;
		}

		return true;
	}
};

template<class V>
u64 hash_vector(V v);

}

#endif

