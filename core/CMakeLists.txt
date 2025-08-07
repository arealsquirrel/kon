project (kon_core VERSION 1.0.0)

file(GLOB_RECURSE SRCS src/kon/*.cpp src/modules/*.cpp)
file(GLOB_RECURSE WINDOWS_SRCS src/platform/windows/*.cpp)
file(GLOB_RECURSE LINUX_SRCS    src/platform/linux/*.cpp)

if (WIN32)
	list(APPEND SRCS ${WINDOWS_SRCS})
elseif(UNIX)
	list(APPEND SRCS ${LINUX_SRCS})
endif (WIN32)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(KN_DEBUG ON)
	add_compile_options(-O0 -Wall -Wextra -fms-extensions)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_options(-O3 -DNDEBUG)
endif()

configure_file(${PROJECT_SOURCE_DIR}/src/config/config.hpp.in
			   ${PROJECT_SOURCE_DIR}/src/config/config.hpp)

# Add a library with the above sources
add_library(${PROJECT_NAME} ${SRCS} ${HDRS} vendor/implementation.cpp)

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/vendor
)

target_link_libraries(${PROJECT_NAME}
	nlohmann_json::nlohmann_json
	spdlog
	glfw
	vulkan

	imgui
	imgui-glfw
	imgui-vulkan
)


