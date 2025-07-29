
#include "kon/container/circular_buffer.hpp"
#include "kon/container/hashmap.hpp"
#include "kon/container/set.hpp"
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

TEST(Container, Hashmap) {
	MemoryBlock block(1204);
	Allocator fa(&block);

	HashMap<u32, String> map(&fa, 2);

	map.add({1, String("one", &fa)});
	map.add({2, String("two", &fa)});
	map.add({3, String("three", &fa)});
	map.add({4, String("four", &fa)});
	map.remove(3);
	map.add({5, String("five", &fa)});

	EXPECT_EQ(map.has_key(3), false);
	EXPECT_EQ(map.has_key(2), true);
	EXPECT_STREQ("one", map[1].second.c_str());
	EXPECT_STREQ("five", map[5].second.c_str());
}

TEST(Container, CircleQueue) {
	MemoryBlock block(1204);
	Allocator fa(&block);

	CircleBuffer<int> queue(&fa, 3);
	queue.enqueue(1);
	queue.enqueue(2);
	queue.enqueue(3);

	EXPECT_EQ(queue.get_front(), 1);
	EXPECT_EQ(queue.get_rear(), 3);
	EXPECT_EQ(queue.dequeue(), 1);
	EXPECT_EQ(queue.get_front(), 2);
}

TEST(Container, Set) {
	MemoryBlock block(1204);
	Allocator fa(&block);

	Set<int, ShortString> set(&fa);
	set.add({1, "one"});
	set.add({2, "two"});
	set.add({3, "three"});
	set.add({4, "four"});

	set.add({4, "four"});
	
	set.remove(3);
	set.add({5, "two"});

	EXPECT_EQ(set.has_key(3), false);
	EXPECT_EQ(set.has_key(1), true);
	EXPECT_STREQ(set.find(2).second.c_str(), "two");
}

