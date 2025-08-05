
project(testing_bencharking)

# ---------- TESTING ----------
if(KN_BUILD_TESTS OR (CMAKE_BUILD_TYPE STREQUAL "Debug"))
	file(GLOB_RECURSE TEST_SRCS tests/*.cpp)
	add_executable(
		tests
		${TEST_SRCS}
	)
	target_link_libraries(
	  tests

	  kon_core
	  GTest::gtest_main
	)


	include(GoogleTest) # Required for gtest_discover_tests()
	gtest_discover_tests(tests)
endif()

# ---------- BENCHMARKS ----------
if(KN_BUILD_BENCHMARKS)
	file(GLOB_RECURSE BM_SRCS benchmarks/*.cpp)
	add_executable(
		benchmarks
		${BM_SRCS}
	)
	target_link_libraries(
	  benchmarks

	  benchmark::benchmark_main
	  kon_core
	)
endif()

