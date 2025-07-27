
#include <benchmark/benchmark.h>
#include <iostream>
#include <string>

static void BENCHMARK_game(benchmark::State &state) {
    for (auto _ : state) {
        for (int i = 1; i <= 100; i++) {
			std::string s("hello");
        }
    }
}

BENCHMARK(BENCHMARK_game);

// BmNCHMARK_MAIN();
