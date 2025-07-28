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

