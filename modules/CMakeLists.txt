
project (kon-modules VERSION 1.0.0)

file(GLOB_RECURSE EXAMPLE_SRCS example/src/*.cpp)
add_library(example_module ${EXAMPLE_SRCS})
target_link_libraries(example_module SHARED
	kon-core
)

file(GLOB_RECURSE GRAPHICS_SRCS example/src/*.cpp)
add_library(graphics_module ${EXAMPLE_SRCS})
target_link_libraries(graphics_module SHARED
	kon-core
)
