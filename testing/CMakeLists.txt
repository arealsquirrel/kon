
project(testing_bencharking)

# enable_testing()

# ---------- TESTING ----------
file(GLOB_RECURSE TEST_SRCS tests/*.cpp)
add_executable(
	tests
	${TEST_SRCS}
)
target_link_libraries(
  tests

  kon-core
  GTest::gtest_main
)

include(GoogleTest) # Required for gtest_discover_tests()
gtest_discover_tests(tests)

# ---------- BENCHMARKING ----------
file(GLOB_RECURSE BENCHMARKING_SRCS benchmarks/*.cpp)
add_executable(
	benchmarking
	${BENCHMARKING_SRCS}
)
target_link_libraries(
	benchmarking

	kon-core
	benchmark::benchmark_main
)
