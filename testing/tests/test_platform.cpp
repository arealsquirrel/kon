
#include "kon/container/arraylist.hpp"
#include "kon/core/directory.hpp"
#include "kon/debug/log.hpp"
#include <gtest/gtest.h>

using namespace kon;

TEST(DISABLED_Platform, Directory) {
	MemoryBlock block(1000);
	Allocator all(&block);

	Directory a(String("does not exist", &all));
	EXPECT_EQ(a.get_valid(), false);

	Directory b(String("./", &all));
	EXPECT_EQ(b.get_valid(), true);
	EXPECT_EQ(b.get_stat().directory, true);

	Directory c(String("Makefile", &all));
	EXPECT_EQ(c.get_valid(), true);
	EXPECT_EQ(c.get_stat().directory, false);

	ArrayList<Directory> d(&all);
	platform::iterate_directory(&all, "../", d);

	Directory some = b + d.get(5).get_string().c_str();
	EXPECT_EQ(some.get_valid(), true);

	some += "WRONG.txt";
	EXPECT_EQ(some.get_valid(), false);
}

