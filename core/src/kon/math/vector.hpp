#ifndef KN_VECTOR_HPP
#define KN_VECTOR_HPP

#include "kon/core/assert.hpp"
#include "kon/core/util.hpp"
#include <functional>
#include <kon/core/core.hpp>

namespace kon {

/*
 * template definition of a vector of variable size
 * all vectors will hold a Type vec that can be accessed as an array
 *
 * the vector template definitions should only detail how the vector is stored
 * i dont wanna see any implementation in there
 *
 * i swear to god future me.
 */
template<u8 size, typename Type>
struct Vector {
public:
	Type vec[size];
};

template<class V>
u64 hash_vector(V v);

/*
 * returns the norm or the magnitude of the vector
 * the p value is always 2
 */
template<class V>
float vector_norm(V vector);

template<class V>
V vector_add(V va, V vb);

template<class V>
float vector_dot(V va, V vb);

Vector<3,float> vector_cross(Vector<3,float> va, Vector<3,float> vb);

}

#endif

