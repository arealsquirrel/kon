
#include "kon/container/string.hpp"
#include <kon/core/util.hpp>
#include <gtest/gtest.h>
#include <kon/core/reflection.hpp>

using namespace kon;

struct Foo {
	int x;
	float y;
};

KN_REFLECT(Foo, 
KN_REF_BEGIN_TYPES(
	KN_REF_TYPE(x, true),
	KN_REF_TYPE(y, true)
),
KN_REF_NO_FUNCTIONS
)

TEST(Reflection, Variant) {
	Foo foo {2, 1.2};

	EXPECT_EQ(Reflection::reflect(foo).get_type<int>("x"), 2);
	EXPECT_NEAR(Reflection::reflect(foo).get_type<float>("y"), 1.2, 0.1);
}

