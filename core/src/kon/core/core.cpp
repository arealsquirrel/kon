
#include "core.hpp"

namespace kon {

static_assert(sizeof(u8) == 1, "u8 is not the right size");
static_assert(sizeof(i8) == 1, "i8 is not the right size");
static_assert(sizeof(u16) == 2, "u16 is not the right size");
static_assert(sizeof(i16) == 2, "i16 is not the right size");
static_assert(sizeof(u32) == 4, "u32 is not the right size");
static_assert(sizeof(i32) == 4, "i32 is not the right size");
static_assert(sizeof(u64) == 8, "u64 is not the right size");
static_assert(sizeof(i64) == 8, "i64 is not the right size");

int test_function() {
	return 2;
}
	
}

