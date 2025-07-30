
cmake_minimum_required(VERSION 3.30)

option(KN_UNIX_DRIVER "x11 or wayland" wayland)
option(KN_BUILD_BENCHMARKS "build benchmarks" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

include(cmake/get_cpm.cmake)
include(cmake/get_packages.cmake)

project(kon LANGUAGES CXX)

enable_testing()

add_subdirectory(core)
add_subdirectory(modules)
add_subdirectory(editor)
add_subdirectory(testing)

