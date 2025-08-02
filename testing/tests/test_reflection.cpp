
#include "kon/container/string.hpp"
#include "kon/core/core.hpp"
#include "kon/core/variant.hpp"
#include "kon/debug/log.hpp"
#include <cstddef>
#include <kon/core/util.hpp>
#include <gtest/gtest.h>
#include <kon/core/reflection.hpp>

using namespace kon;

struct Foo {
	int x;
	float y;
};

KN_REFLECT(Foo, {
	KN_REF_FIELD(x),
	KN_REF_FIELD(y)
})

struct Bar {
	double d;
	Color color;
	ShortString str;
};

KN_REFLECT(Bar, {
	KN_REF_FIELD(d),
	KN_REF_FIELD(color),
	KN_REF_FIELD(str),
})

TEST(Reflection, Variant) {
	Bar bar;
	bar.d = 2.7;
	bar.color = Color{21,27,100,255};
	bar.str = "hello!";

	Reflection::reflect_type<Bar>()->reflect_fields(bar, 
	[](const auto &field, char *mem){
		switch (field.type) {
		case VariantType_double: 
			KN_CORE_INFO("double {}", *KN_CAST_MEM(mem, double));
			break;

		case VariantType_Color:  
			KN_CORE_INFO("color {}",  KN_CAST_MEM(mem, Color)->rgba[0]);
			break;

		case VariantType_ShortString:  
			KN_CORE_INFO("short string {}",  KN_CAST_MEM(mem, ShortString)->c_str());
			break;

		default:
			return;
		}
	});
}

