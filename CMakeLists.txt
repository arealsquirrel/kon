
cmake_minimum_required(VERSION 4.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

# download CPM.cmake
file(
  DOWNLOAD
  https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.8/CPM.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
  EXPECTED_HASH SHA256=78ba32abdf798bc616bab7c73aac32a17bbd7b06ad9e26a6add69de8f3ae4791
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

CPMAddPackage("gh:nlohmann/json@3.12.0")

CPMAddPackage("gh:gabime/spdlog@1.15.3")
CPMAddPackage("gh:google/googletest@1.17.0")
CPMAddPackage("gh:google/benchmark@1.5.2")

set(BENCHMARK_ENABLE_TESTING OFF)
set(JSON_BuildTests OFF)

project(kon LANGUAGES CXX)

find_package(Threads REQUIRED)
find_package(glfw3 REQUIRED)
#find_package(Vulkan REQUIRED)

add_subdirectory(core)
add_subdirectory(editor)
add_subdirectory(testing)

