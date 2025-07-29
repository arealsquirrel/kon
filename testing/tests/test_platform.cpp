
#include "kon/core/directory.hpp"
#include <gtest/gtest.h>
// #include <kon/core/core.hpp>


using namespace kon;

TEST(DISABLED_Platform, Directory) {
	MemoryBlock block(1000);
	Allocator all(&block);

	Directory a(String("does not exist", &all));
	EXPECT_EQ(a.get_valid(), false);

	Directory b(String("core", &all));
	EXPECT_EQ(b.get_valid(), true);
	EXPECT_EQ(b.get_stat().directory, true);

	Directory c(String("Makefile", &all));
	EXPECT_EQ(c.get_valid(), true);
	EXPECT_EQ(c.get_stat().directory, false);
}

