
project(testing_bencharking)

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

# ---------- BENCHMARKS ----------
file(GLOB_RECURSE BM_SRCS benchmarks/*.cpp)
add_executable(
	benchmarks
	${BM_SRCS}
)
target_link_libraries(
  benchmarks

  benchmark::benchmark_main
  kon-core
)

include(GoogleTest) # Required for gtest_discover_tests()
gtest_discover_tests(tests)
