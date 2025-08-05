
cmake_minimum_required(VERSION 3.30)

option(KN_BUILD_BENCHMARKS "build benchmarks" OFF)

# by default, these options are off, but if you do a debug build
# then cmake will generate the macros to enable these systems
option(KN_ENABLE_LOGGING "enables logging" OFF)
option(KN_ENABLE_PROFILING "enables profiling" OFF)
option(KN_BUILD_TESTS "build tests" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

include(cmake/get_cpm.cmake)
include(cmake/get_packages.cmake)

project(kon LANGUAGES CXX)

find_package(Vulkan REQUIRED)

add_subdirectory(core)
add_subdirectory(editor)
add_subdirectory(testing)

