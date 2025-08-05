#ifndef KN_VECTOR_HPP
#define KN_VECTOR_HPP

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

}

#endif

