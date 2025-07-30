#ifndef KN_VARIANT_HPP
#define KN_VARIANT_HPP

#include "kon/container/string.hpp"
#include "kon/core/util.hpp"
#include <kon/core/core.hpp>

namespace kon {

enum VariantType {
	VariantType_u32
};

/*
 * defines an object that can be some set of values,
 * for strings, it only does short strings
 */
class Variant {
public:

private:
	union {
		int i;
		unsigned int ui;
		long unsigned int lui;
		float f;
		double d;
		ShortString s;
		Color col;
	};
};

}

#endif
