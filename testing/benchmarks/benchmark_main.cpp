
#include <benchmark/benchmark.h>
#include <iostream>
#include <string>
#include <kon/core/allocator.hpp>

using namespace kon;

class TestClass {
public:
	TestClass() {
		x += 2;
	}

	~TestClass() {
		x -= 2;
	}

	int foo() { return x + 1; }

	int x {2};
};

static void BENCHMARK_mallocAllocator(benchmark::State &state) {
	MemoryBlock block(1000);
	Allocator all(&block);

	for(auto _ : state) {
		TestClass *a = all.allocate<TestClass>(5);
		TestClass *b = all.allocate<TestClass>(10);
		TestClass *e = all.allocate<TestClass>(3);
		all.free(e, 3);
		TestClass *c = all.allocate<TestClass>(2);
		all.free(a, 5);
		TestClass *d = all.allocate<TestClass>(8);
		all.free(b, 10);
		all.free(d, 8);
		all.free(c, 2);
	}
}

static void BENCHMARK_freeListAllocator(benchmark::State &state) {
	MemoryBlock block(1000);
	FreeListAllocator all(&block);

	for(auto _ : state) {
		TestClass *a = all.allocate<TestClass>(5);
		TestClass *b = all.allocate<TestClass>(10);
		TestClass *e = all.allocate<TestClass>(3);
		all.free(e, 3);
		TestClass *c = all.allocate<TestClass>(2);
		all.free(a, 5);
		TestClass *d = all.allocate<TestClass>(8);
		all.free(b, 10);
		all.free(d, 8);
		all.free(c, 2);
	}
}

static void BENCHMARK_game(benchmark::State &state) {
    for (auto _ : state) {
        for (int i = 1; i <= 100; i++) {
			std::string s("hello");
        }
    }
}

BENCHMARK(BENCHMARK_mallocAllocator);
BENCHMARK(BENCHMARK_freeListAllocator);
BENCHMARK(BENCHMARK_game);

// BmNCHMARK_MAIN();
