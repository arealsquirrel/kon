#ifndef KN_REFLECTION_HPP
#define KN_REFLECTION_HPP

#include "kon/container/array.hpp"
#include "kon/container/arraylist.hpp"
#include "kon/container/string.hpp"
#include "kon/core/allocator.hpp"
#include "kon/core/util.hpp"
#include <functional>
#include <kon/core/core.hpp>

namespace kon {

/*
 * enum for implied reflected types.
 */
enum ReflectedType {
	ReflectedType_Null = 0,
	ReflectedType_ReflectClass,
	ReflectedType_int,
	ReflectedType_float,
	ReflectedType_void
};

class ReflectClass;

struct ReflectField {
	ShortString name;
	u32 offset;
	ReflectedType type;
	const ReflectClass* m_type;
};

struct ReflectFunction {
	ShortString name;
};

class ReflectField;

struct ReflectClass {
	ShortString m_name;
	UnCopyable<ArrayList<ReflectField>> m_fields;
	UnCopyable<ArrayList<ReflectFunction>> m_funtions;
};

/*
 * defines static functions for reflecting types, and an
 * instance implementation for accessing arbitrary types by field name
 */
class Reflection {
public:
	~Reflection() = default;

	const ReflectClass *get_class() const { return m_reflectedClass; }

	ReflectField get_field(ShortString name) {
		for(u32 i = 0; i < m_reflectedClass->m_fields->get_count(); i++) {
			if(m_reflectedClass->m_fields->get(i).name.hash() == name.hash()) {
				return m_reflectedClass->m_fields->get(i);
			}
		}

		return ReflectField{"_nulltype", 0, ReflectedType_Null, nullptr};
	}

	template<typename T>
	T &get_type(ShortString name) {
		ReflectField field = get_field(name);
		return *reinterpret_cast<T*>(m_classMem+field.offset);
	}

	void for_each_field(std::function<void(const ReflectField &field)> f) {
		for(u32 i = 0; i < m_reflectedClass->m_fields->get_count(); i++) {
			f(m_reflectedClass->m_fields->get(i));
		}
	}

private:
	char *m_classMem;
	const ReflectClass *m_reflectedClass;

public:
	// ------------ STATIC STUFF ------------ //
	template<class T>
	static ReflectField reflect_field_type(ShortString fieldName, u32 offset);

	template<class T>
	static const ReflectClass *reflect_class();

	template<class T>
	static Reflection reflect(T &t) {
		Reflection r;
		r.m_classMem = KN_MEM_POINTER(&t);
		r.m_reflectedClass = reflect_class<T>();
		return r;
	}

private:
	static Allocator s_allocator;
};

#define KN_REF_BEGIN_TYPES(...) ArrayList<ReflectField>(&s_allocator, {__VA_ARGS__})
#define KN_REF_BEGIN_FUNCTIONS(...) ArrayList<ReflectFunction>(&s_allocator, {__VA_ARGS__}),
#define KN_REF_TYPE(name) reflect_field_type<decltype(T::name)>(#name, offsetof(T, name))
#define KN_REF_FUNCTION(function) ReflectFunction{#function}
#define KN_REF_NO_FUNCTIONS KN_REF_BEGIN_FUNCTIONS() 
#define KN_REFLECT(CLASS, ...) \
template<> \
const ReflectClass *Reflection::reflect_class<Foo>() { \
	using T = Foo; \
	static ReflectClass c{#CLASS, __VA_ARGS__}; \
	return &c; \
} \
template<> \
ReflectField Reflection::reflect_field_type<Foo>(ShortString fieldName, u32 offset) { \
	return ReflectField{ \
		fieldName, \
		offset, \
		ReflectedType_ReflectClass, \
		reflect_class<CLASS>() \
	}; \
}

}

#endif
