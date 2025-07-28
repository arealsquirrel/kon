#include "kon/container/array.hpp"
#include "kon/container/string.hpp"
#include <gtest/gtest.h>
#include <kon/core/core.hpp>
#include <kon/core/allocator.hpp>

using namespace kon;

static int s_what = 0;

class TestClass {
public:
	TestClass() {
		x += 2;
		s_what += 4;
	}

	~TestClass() {
		s_what -= 2;
	}

	int foo() { return x + 1; }

	int x {2};
}; 

TEST(Container, ShortString) {
	ShortString s;
	s = "wow";
	EXPECT_STREQ(s.c_str(), "wow");
	
	s.append(" thats so cool");
	EXPECT_STREQ(s.c_str(), "wow thats so cool");
	EXPECT_EQ(s.index_of_char('o'), 1);
	EXPECT_EQ(s.index_of_string("thats"), 4);	
}

TEST(Container, String) {
	MemoryBlock block(1000);
	Allocator all(&block);

	String s(&all);
	s = "wow";
	EXPECT_STREQ(s.c_str(), "wow");
	
	s.append(" thats so cool");
	EXPECT_STREQ(s.c_str(), "wow thats so cool");
	EXPECT_EQ(s.index_of_char('o'), 1);
	EXPECT_EQ(s.index_of_string("thats"), 4);	
}

TEST(Container, Array) {
	MemoryBlock block(1000);
	Allocator all(&block);

	Array<ShortString, 5> arr(&all);
	arr[0] = ShortString("zero");
	arr[1] = ShortString("one");
	arr[2] = ShortString("two");
	arr[3] = ShortString("three");
	arr[4] = ShortString("four");

	EXPECT_STREQ(arr[0].c_str(), "zero");
}

