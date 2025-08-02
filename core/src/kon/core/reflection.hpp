#ifndef KN_REFLECTION_HPP
#define KN_REFLECTION_HPP

#include "kon/container/arraylist.hpp"
#include "kon/container/string.hpp"
#include "kon/core/allocator.hpp"
#include "kon/core/util.hpp"
#include "kon/core/variant.hpp"
#include <functional>
#include <initializer_list>
#include <kon/core/core.hpp>

namespace kon {

class ReflectType;

/*
 * represents a field. 
 * A field details a variant or a refrance to
 * ReflectType that can be used to store other reflected types
 */
struct ReflectField {
	ShortString name;
	bool isVariant;

	union {
		VariantType type;
		const ReflectType *next;
	};
};

/*
 * represents a type that can be reflected
 * a type is composed of fields and functions
 */
class ReflectType {
public:
	ReflectType(ShortString name, std::initializer_list<Pair<ReflectField, u32>> fields);
	~ReflectType();

public:
	void for_each_field(std::function<void(const ReflectField &field)> f) const;

	template<typename T>
	void reflect_fields(T &t,
			std::function<void(const ReflectField &field, char *mem)> f) const {
		
		for(u32 i = 0; i < m_fields.get_size(); i++) {
			auto &pair = m_fields[i];
			f(pair.first, KN_MEM_POINTER(&t)+pair.second);
		}
	}

	ShortString get_type_name() const { return m_name; }

private:

	/* if you copy this field, i will fucking kill you */
	const ArrayList<Pair<ReflectField, u32>> &get_fields() const { return m_fields; }


	/* holds a list of fields, and their offsets within their types */
	ArrayList<Pair<ReflectField, u32>> m_fields;
	ShortString m_name;
};

class Reflection {
public:
	static Reflection &get() {
		static Reflection r;
		return r;
	}

public:
	const ReflectType *get_type(ShortString name);

	template<typename T>
	static const ReflectType *reflect_type();
	
private:
	Reflection();
	~Reflection();

	/*
	 * takes a template and a name, and creates a 
	 * reflect field class
	 */
	template<typename T>
	static ReflectField create_field(ShortString name);

private:
	MemoryBlock m_reflectionBlock;
	Allocator m_allocator;

	friend ReflectType;
};

#define KN_REF_FIELD(name) {Reflection::create_field<decltype(T::name)>(#name), offsetof(T, name)}

#define KN_REFLECT(TypeName, ...) template<> \
const ReflectType *Reflection::reflect_type<TypeName>() { \
	using T = TypeName; \
	static ReflectType type(#TypeName, __VA_ARGS__); \
	return &type; \
} \
template<> \
ReflectField Reflection::create_field<TypeName>(ShortString name) { \
	ReflectField field; \
	field.name = name; \
	field.isVariant = false; \
	field.next = Reflection::get().reflect_type<TypeName>(); \
	return field; \
}

}

#endif
