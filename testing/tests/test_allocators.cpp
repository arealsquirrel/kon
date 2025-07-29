
#include "kon/debug/log.hpp"
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

TEST(Allocator, BaseAllocator) {
	s_what = 0;
	MemoryBlock block(1000);
	Allocator all(&block);
	TestClass *t = all.allocate<TestClass>(5);
	EXPECT_EQ(t[0].x, 4);
	EXPECT_EQ(t[1].foo(), 5);
	all.free(t, 5);
	
	EXPECT_EQ(s_what, 10);
}

TEST(Allocator, StackAllocator) {
	s_what = 0;
	MemoryBlock block(4000);
	StackAllocator all(&block);
	TestClass *t = all.allocate<TestClass>(5);
	
	EXPECT_EQ(t[0].x, 4);
	EXPECT_EQ(t[1].foo(), 5);
	all.free(t, 5);
	
	EXPECT_EQ(s_what, 10);
}

TEST(Allocator, FreeListAllocator) {
	s_what = 0;
	MemoryBlock block(1000);
	FreeListAllocator all(&block);
	TestClass *t = all.allocate<TestClass>(5);
	EXPECT_EQ(t[0].x, 4);
	EXPECT_EQ(t[1].foo(), 5);
	all.free(t, 5);
	
	EXPECT_EQ(s_what, 10);
}

TEST(Allocator, PageAllocator) {
	s_what = 0;
	MemoryBlock block(600);
	PageAllocator all(&block, 100);
	TestClass *a = all.allocate<TestClass>();
	TestClass *b = all.allocate<TestClass>();
	TestClass *c = all.allocate<TestClass>();
	all.free(a);
	all.free(b);
	TestClass *d = all.allocate<TestClass>();
	TestClass *e = all.allocate<TestClass>();
	TestClass *f = all.allocate<TestClass>();

	EXPECT_EQ(d->x, 4);
	EXPECT_EQ(c->foo(), 5);
	all.free(e, 5);
}

