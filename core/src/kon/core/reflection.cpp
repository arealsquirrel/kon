
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

/*
ReflectType::ReflectType(ShortString name, std::initializer_list<Pair<ReflectField, u32>> fields)
	: m_fields(&Reflection::get().m_allocator, fields.size()), m_name(name) {
	
	for(const Pair<ReflectField, u32> &f : fields) {
		m_fields.add(f);
	}
}

ReflectType::~ReflectType() = default;

void ReflectType::for_each_field(std::function<void(const ReflectField &field)> f) const {
	for(u32 i = 0; i < m_fields.get_size(); i++) {
		auto &pair = m_fieads[i];
		f(pair.first);
	} w
}

Reflection::Reflection() 
	: m_reflectionBlock(0),
	  m_allocator(&m_reflectionBlock) {}

Reflection::~Reflection() = default;

template<>
ReflectField Reflection::create_field<int>(ShortString name) {
	ReflectField field;
	
	field.name = name;
	field.isVariant = true;
	field.type = VariantType_int;

	return field;
}

template<>
ReflectField Reflection::create_field<float>(ShortString name) {
	ReflectField field;
	field.name = name;
	field.isVariant = true;
	field.type = VariantType_float;
	return field;
}

template<>
ReflectField Reflection::create_field<unsigned int>(ShortString name) {
	ReflectField field;
	field.name = name;
	field.isVariant = true;
	field.type = VariantType_uint;
	return field;
}

template<>
ReflectField Reflection::create_field<long unsigned int>(ShortString name) {
	ReflectField field;
	field.name = name;
	field.isVariant = true;
	field.type = VariantType_luint;
	return field;
}

template<>
ReflectField Reflection::create_field<double>(ShortString name) {
	ReflectField field;
	field.name = name;
	field.isVariant = true;
	field.type = VariantType_double;
	return field;
}

template<>
ReflectField Reflection::create_field<Color>(ShortString name) {
	ReflectField field;
	field.name = name;
	field.isVariant = true;
	field.type = VariantType_Color;
	return field;
}

template<>
ReflectField Reflection::create_field<ShortString>(ShortString name) {
	ReflectField field;
	field.name = name;
	field.isVariant = true;
	field.type = VariantType_ShortString;
	return field;
}
*/

}

