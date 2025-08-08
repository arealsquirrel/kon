
#include "reflection.hpp"
#include "kon/core/allocator.hpp"
#include "kon/core/util.hpp"
#include "kon/math/vector2.hpp"
#include "kon/math/vector3.hpp"
#include "kon/math/vector4.hpp"

namespace kon {

Allocator Reflection::s_allocator(nullptr);

template<>
ReflectField Reflection::reflect_field_type<int>(ShortString fieldName, u32 offset, bool isMutable) {
	return ReflectField{
		fieldName,
		offset,
		ReflectedType_int,
		nullptr,
		isMutable
	};
}

template<>
ReflectField Reflection::reflect_field_type<float>(ShortString fieldName, u32 offset, bool isMutable) {
	return ReflectField{
		fieldName,
		offset,
		ReflectedType_float,
		nullptr,
		isMutable
	};
}

template<>
ReflectField Reflection::reflect_field_type<Vector4>(ShortString fieldName, u32 offset, bool isMutable) {
	return ReflectField{
		fieldName,
		offset,
		ReflectedType_vec4,
		nullptr,
		isMutable
	};
}

template<>
ReflectField Reflection::reflect_field_type<Vector3>(ShortString fieldName, u32 offset, bool isMutable) {
	return ReflectField{
		fieldName,
		offset,
		ReflectedType_vec3,
		nullptr,
		isMutable
	};
}

template<>
ReflectField Reflection::reflect_field_type<Vector2>(ShortString fieldName, u32 offset, bool isMutable) {
	return ReflectField{
		fieldName,
		offset,
		ReflectedType_vec2,
		nullptr,
		isMutable
	};
}

template<>
ReflectField Reflection::reflect_field_type<Color>(ShortString fieldName, u32 offset, bool isMutable) {
	return ReflectField{
		fieldName,
		offset,
		ReflectedType_color,
		nullptr,
		isMutable
	};
}

}

