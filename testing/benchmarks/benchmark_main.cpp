
#include "kon/container/hashmap.hpp"
#include "kon/container/set.hpp"
#include "kon/container/string.hpp"
#include "kon/core/variant.hpp"
#include "kon/debug/log.hpp"
#include "kon/math/transformations.hpp"
#include "kon/math/vector3.hpp"
#include "kon/math/vector4.hpp"
#include <benchmark/benchmark.h>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <kon/core/allocator.hpp>
#include <kon/math/matrix_operations.hpp>
#include <unordered_map>

using namespace kon;

/*

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

static void BENCHMARK_allocator(benchmark::State &state) {
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

static void BENCHMARK_hashmap(benchmark::State &state) {
	MemoryBlock block(1000 * 6 * sizeof(Pair<int,ShortString>));
	FreeListAllocator all(&block);
	HashMap<int, ShortString> map(&all, 100);

	for(auto _ : state) {
		for (int i = 0; i < 380; i++) {
			map.add({i*31, ShortString(std::to_string(i).c_str())});
		}

		for (int i = 0; i < 380; i++) {
			map.remove(i*31);
		}
	}
}

static void BENCHMARK_set(benchmark::State &state) {
	MemoryBlock block(1000 * 2 * sizeof(Pair<int,ShortString>));
	PageAllocator all(&block, sizeof(SetTreeNode<int,ShortString>));
	Set<int, ShortString> set(&all);

	for(auto _ : state) {
		for (int i = 0; i < 1000; i++) {
			set.add({i, ShortString(std::to_string(i).c_str())});
		}

		for (int i = 0; i < 1000; i++) {
			set.remove(i);
		}
	}
}

static void BENCHMARK_setLookup(benchmark::State &state) {
	MemoryBlock block(10000 * 2 * sizeof(Pair<int,ShortString>));
	PageAllocator all(&block, sizeof(SetTreeNode<int,ShortString>));
	Set<int, ShortString> set(&all);

	for (int i = 0; i < 10000; i++) {
		set.add({i, ShortString(std::to_string(i).c_str())});
	}

	u32 add = 0;
	for(auto _ : state) {
		for (int i = 0; i < 10000; i++) {
			add += set.find(i).first;
		}
	}

	KN_CORE_INFO("{}", add);
}

static void BENCHMARK_stdSetLookup(benchmark::State &state) {

	std::map<int, ShortString> set;

	for (int i = 0; i < 10000; i++) {
		set[i] = ShortString(std::to_string(i).c_str());
	}

	u32 add = 0;
	for(auto _ : state) {
		for (int i = 0; i < 10000; i++) {
			add += set.find(i)->first;
		}
	}

	KN_CORE_INFO("{}", add);
}

static void BENCHMARK_stdSet(benchmark::State &state) {
	std::map<int, ShortString> set;

	for(auto _ : state) {
		for (int i = 0; i < 1000; i++) {
			set[i] = ShortString(std::to_string(i).c_str());
		}

		for (int i = 0; i < 1000; i++) {
			set.erase(i);
		}
	}
}

static void BENCHMARK_stdhashmap(benchmark::State &state) {
	std::unordered_map<int, ShortString> map;

	for(auto _ : state) {
		// this is the .75 load factor
		for (int i = 0; i < 380; i++) {
			map[i*31] = ShortString(std::to_string(i).c_str());
		}

		for (int i = 0; i < 380; i++) {
			map.erase(i*31);
		}
	}
}

struct Foo {
	int x;
	float y;
};

struct FieldData {
	ShortString name;
	VariantType type;
	u32 offset;
};


BENCHMARK(BENCHMARK_setLookup);
BENCHMARK(BENCHMARK_stdSetLookup);
BENCHMARK(BENCHMARK_set);
BENCHMARK(BENCHMARK_stdSet);
BENCHMARK(BENCHMARK_hashmap);
BENCHMARK(BENCHMARK_stdhashmap);
BENCHMARK(BENCHMARK_mallocAllocator);
BENCHMARK(BENCHMARK_freeListAllocator);
*/

static void BENCHMARK_projection(benchmark::State &state) {
	Vector4 model;
	for(auto _ : state) {
		Vector3 point{1,5,3};
		model = matrix_multiply_vec(trfm_translation(Vector3{3,2,2}), {point.x, point.y, point.z, 1});
		model = matrix_multiply_vec(trfm_scale(Vector3{2,1,1}), model);
		model = matrix_multiply_vec(trfm_rotate_z(rand()), model);
		model = matrix_multiply_vec(trfm_orthographic(0.01, 100.0f, -10.0f, 10.0f, 10.0f, -10.0f), model);
	}

	KN_TRACE("{} {} {} {}", model.x, model.y, model.z, model.w);
}

/*
 * rotate = 10ns
 * scale = 10ns
 * ortho = 10ns
 * trans = 10ns
 */

BENCHMARK(BENCHMARK_projection);

