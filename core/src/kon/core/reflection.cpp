
#include "reflection.hpp"
#include "kon/container/array.hpp"
#include "kon/core/allocator.hpp"
#include "kon/core/util.hpp"
#include "kon/core/variant.hpp"
#include <cstddef>

namespace kon {

Allocator Reflection::s_allocator(nullptr);

template<>
ReflectField Reflection::reflect_field_type<int>(ShortString fieldName, u32 offset) {
	return ReflectField{
		fieldName,
		offset,
		ReflectedType_int,
		nullptr
	};
}

template<>
ReflectField Reflection::reflect_field_type<float>(ShortString fieldName, u32 offset) {
	return ReflectField{
		fieldName,
		offset,
		ReflectedType_float,
		nullptr
	};
}

}

