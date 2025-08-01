
#include "kon/math/vector.hpp"
#include "kon/math/vector2.hpp"
#include <gtest/gtest.h>
#include <kon/core/core.hpp>

using namespace kon;

// Demonstrate some basic assertions.
TEST(Math, Vector) {
	Vector2 v{2,1};

	EXPECT_EQ(v.x, 2);
}

