
#include "kon/container/string.hpp"
#include "kon/container/array.hpp"
#include <gtest/gtest.h>
// #include <kon/core/core.hpp>
// #include <kon/core/allocator.hpp>
#include "kon/container/arraylist.hpp"

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

class foo {
public:
	foo() = default;

	foo(const foo &f) = default;

	foo(int _a)
		: a(_a) {}

	~foo() = default;

public:
	int a {1};
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

	// EXPECT_STREQ(arr[0].c_str(), "zero");
}


TEST(Container, ArrayList) {
	MemoryBlock block(1204);
	FreeListAllocator fa(&block);

	ArrayList<foo> array(&fa);
	array.add(foo(5));
	array.add(foo(2));
	array.add(foo(3));

	ArrayList<foo> newArray(array);
	newArray.add(foo(1));
	newArray.remove();
	newArray.add(foo(3));
	newArray[0].a = 4;

	EXPECT_EQ(array[0].a, 5);
	EXPECT_EQ(array.get_count(), 3);
	EXPECT_EQ(array.get_size(), 3);
	EXPECT_EQ(newArray.get_size(), 4);
	EXPECT_EQ(newArray[0].a, 4);
	EXPECT_EQ(newArray.get_count(), 4);
}



