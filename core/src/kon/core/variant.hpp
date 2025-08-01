#ifndef KN_VARIANT_HPP
#define KN_VARIANT_HPP

#include "kon/container/string.hpp"
#include "kon/core/util.hpp"
#include <kon/core/core.hpp>

namespace kon {

enum VariantType {
	VariantType_None,
	VariantType_int,
	VariantType_uint,
	VariantType_luint,
	VariantType_float,
	VariantType_double,
	VariantType_Color,
	VariantType_String,
	VariantType_ShortString
};

/*
 * defines an object that can be some set of values,
 * for strings, it only does short strings
 */
class Variant {
public:
	Variant(VariantType type);
	~Variant();

public:
	VariantType get_type() const { return m_type; }

	template<typename T>
	T &get();

	template<typename T>
	void set(const T &t);

private:
	union {
		int i;
		unsigned int ui;
		long unsigned int lui;
		float f;
		double d;
		String s;
		ShortString ss;
		Color col;
	};

	VariantType m_type;
};

/*
 * template specilizations are in the source file.
 * takes a type and outputs the coresponding variant type.
 * if it is not a valid type, then it returns variant_none
 */
template<typename T>
constexpr VariantType type_to_variant_type() { return VariantType_None; }

}

#endif
